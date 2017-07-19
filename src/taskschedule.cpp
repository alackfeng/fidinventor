
#include "taskschedule.h"

#include "wrap.h"

namespace fid {


static int max_height = 100; //max height 500, init gensis block
int CFidTask::call()
{
        std::cout << "CFidTask::call() type " << this->type << std::endl;
	CFidHandler fidHand(*this);
	

        switch (this->type) {
	case TASK_BLOCKGENSIS:
	{
		int64_t height = atoi64(this->command);
		if(height <= 0) { // height=0, from rpc block best height
			height = fidHand.getBlockCount();
		}

		// max height 500
		//height = height>max_height?max_height:height;

		//height = 5; // for test
		this->height = height; // set current task block best height, return caller
		std::cout << "CFidTask::call() type " << this->type << ", block height " << height << std::endl;

		// from 0 to bestheight
		for(int64_t i=0; i<=height; i++) {
			// reset block and vtx
			fidHand.reset();

			vector<string> vtx = fidHand.getBlockByNumber(i);
			fidHand.insertDBBlock();
			vector<string>::iterator it = vtx.begin();
			for(; it != vtx.end(); it++) {
				fidHand.getTransaction((*it));
			}
			fidHand.insertDBTrans();
				
		}	
		std::cout << "CFidTask::call() type " << this->type << ", block height over===================" << this->height << std::endl;
	}
	break;
        case TASK_BLOCKINFO:
        break;
        case TASK_BLOCKHEIGHT:
        break;
	case TASK_BLOCK:
	{
		int64_t height = atoi64(this->command);
		fidHand.getBlockByNumber(height > 0 ? height:0);
		fidHand.insertDBBlock();
	}
	break;
	case TASK_TRANSACTION:
	{
		string txid = "23db66e0fad7d37fde886bb7d319ab0831d54122bbde0bcc2739db69971b0af1";
		fidHand.getTransaction(txid);
		fidHand.insertDBTrans();
	}
	break;
        case TASK_BLOCK_OR_TRANS:
	{
		// first ftask->height(defualt=-1), second ftask->command(caller set), last getblockcount rpc
		int64_t height = ((this->height != -1) ? this->height : atoi64(this->command));
		// 1. getblockcount  
		int64_t bkheight = fidHand.getBlockCount();
		if(height <= 0) { // height=0, from rpc block best height
                        height = bkheight; //  fidHand.getBlockCount();
                } else if(height > bkheight){
			this->status = 0; //  return  // RPC: {"code":-1,"message":"Block number out of range."},"id":1}
			return 0;
		}
		this->height = height; // set current task block best height, return caller
		// 2. getblockbynumber 3463
		vector<string> vtx = fidHand.getBlockByNumber(height);
		// 3. gettransaction 23db66e0fad7d37fde886bb7d319ab0831d54122bbde0bcc2739db69971b0af1 , more [tx]
		vector<string>::iterator it = vtx.begin();
		for(; it != vtx.end(); it++) {
			fidHand.getTransaction((*it));
		}
		// 4. insert db ?
		fidHand.insertDBBlockTrans();	

		// 5. set status ok
		this->status = 1;
		this->bktime = fidHand.getBkTime(); // for adjust thread while() call time

	}
        break;
        default:
        break;
        }
        return 0;
}


/* CFidTaskSchedule class for tasks setting call getblock or gettransaction etc */

int CFidTaskSchedule::sequence = 1;
int getSeq() {
	return CFidTaskSchedule::sequence++;
}


void CFidTaskSchedule::work() 
{
	int iRet = 0;
	int adjusttime = 1000; // milliseconds
	int bktime = 0;
	int64_t t1 = GetTime();
	std::cout << "CFidTaskSchedule work() call..." << m_thread << ", now " << t1 << std::endl;
 
	while(1) {
		std::cout << "CFidTaskSchedule::Thread run... " << tasks.size() << " count" << std::endl;
		
		int bktimelast = 0;		
		int bContinue = 0;

		list<CFidTask>::iterator it = tasks.begin();
		for(; it != tasks.end(); it++) {
			CFidTask ftask = (*it);
			std::cout << "CFidTaskSchedule::Thread callTask seq: " << ftask.sequence << ". begin.." << std::endl;
			iRet = ftask.call(); // handle real task according to tasktype
			if(iRet != 0) {
				std::cout << "CFidTaskSchedule::Thread callTask error code: " << iRet << std::endl;
			}
			std::cout << "CFidTaskSchedule::Thread callTask seq: " << ftask.sequence << ", status " << ftask.status << ", bktime " << ftask.bktime << ". end...." << std::endl;
			if(ftask.status == 1) { // status ok
				(*it).height = ftask.height + 1;
				bktimelast = ftask.bktime;
				bContinue = 0;
			}
			else {
				(*it).height = ftask.height;	// set to task *it, importment
				//bktime = 0;
				bContinue = 1;	
			}
			
		}

		if(onceRun) { //iRet != 0) {
			std::cout << "CFidTaskSchedule::Thread callTask error code(return): " << iRet << std::endl;
			break;
		}
		int diff = (bktimelast!=0 && bktime!=0)?((bktimelast-bktime)*1000*4/5):adjusttime; // - (t2-t1);
		std::cout << "CFidTaskSchedule::Thread callTask diff: " << diff << ", bktime " << bktime << ", last " << bktimelast << ". end...." << std::endl;
		
		//if((GetTime()-t1) > 64*1000 ) {
		//	diff = adjusttime;
		//}
		if(bktimelast!=0) {
			bktime = bktimelast;
			t1 = GetTime(); //boost::posix_time::second_clock::local_time();
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(bContinue?adjusttime:diff)); // last block 2/3 first, later adjusttime
	}
}
int CFidTaskSchedule::registerTask(TaskType_t type, const string &command)
{
	//CFidTask task = this->convertParams(const_cast<string&>(command));
	CFidTask task(type, const_cast<string&>(command));
	task.sequence = getSeq(); // get sequence unique for forigen
	tasks.push_back(task);

	return task.sequence;
}
int CFidTaskSchedule::registerTask(TaskType_t type, const char *command)
{
	return this->registerTask(type, string(command));
}

CFidTask CFidTaskSchedule::getTask(int seq)
{
	
	CFidTask task;
	list<CFidTask>::iterator it = tasks.begin();
	for(; it != tasks.end(); it++) {
		task = (*it);
		//std::cout <<"========================" << seq << ":" << task.sequence << ":" << task.height << std::endl;
		if(seq != task.sequence) 
			continue;
		return (*it);
	}
	return task;
}
list<CFidTask> CFidTaskSchedule::getTaskList()
{
	return tasks;
}


}
