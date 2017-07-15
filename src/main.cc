
#include <iostream>

#include <signal.h>

//#include "rpcclient.h"
#include "taskschedule.h"
#include "mysqldb/mysqldb.h"
//#include "thread.h"

static int bQuit = 1;

static std::string dbInstance = "fidchain";
static std::string user = "root";
static std::string passwd = "";
static std::string dbAdress = "127.0.0.1";

fid::CFidTaskSchedule* pTaskSched = NULL;
fid::CFidTaskSchedule* pTaskSchedInit = NULL;
CMysqlDb* myDB = NULL;
int height = 0;		// block height
int once = 1;		// only run once, tash thread for init block gensis to bestblock

map<string, string> mapArgs;
int initConfig(int argc, char* argv[]) 
{
	if(argc != 4) {
		std::cout << "Help cmd: fidinventor fidchainrpc20 DSBs3pKSdpiTAPDms 139.224.13.20 " << std::endl;
		return -1;
	}
	// fidinventor fidchainrpc20 DSBs3pKSdpiTAPDms 139.224.13.20
	mapArgs.insert(std::make_pair<std::string, std::string>("-rpcuser", std::string(argv[1]))); //"fidchainrpc20"));
        mapArgs.insert(std::make_pair<std::string, std::string>("-rpcpassword", std::string(argv[2]))); //"DSBs3pKSdpiTAPDms"));
        mapArgs.insert(std::make_pair<std::string, std::string>("-rpcconnect", std::string(argv[3]))); //"139.224.13.20"));
	return 0;
}

static void HandleSIGTERM(int)
{
    std::cout << "===================HandleSIGTERM() ~ " << std::endl;
}
static void HandleSIGHUP(int)
{
    std::cout << "===================HandleSIGHUP() ~ " << std::endl;
}
static void registerSignalHandler(int signal, void(*handler)(int))
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signal, &sa, NULL);
}

int initGensisBlock() 
{
	if(NULL == pTaskSchedInit 
	&& NULL == (pTaskSchedInit = new fid::CFidTaskSchedule(once))) {
		std::cout << "initGensisBlock() ~ fid::CFidTaskSchedule MALLOC failed, exit 1" << std::endl;
		return -1;	
	}
	
	myDB = CMysqlDb::getinitance();
        myDB->initDb(dbInstance,user,passwd,dbAdress);
	height =  myDB->queryBlockCount();
	std::cout << "initGensisBlock() ~ block count: " << height << std::endl;

	// mysql count !=0, not init
	if(height > 0) {
		
		if(pTaskSchedInit) 
			delete pTaskSchedInit;
		//if(myDB)
		//	delete myDB;
		return ++height;
	}


	// init gensis 0 to bestblock=height
	int seq = pTaskSchedInit->registerTask(fid::TASK_BLOCKGENSIS, i64tostr(height).c_str());
	pTaskSchedInit->run();

	//std::cout << "initGensisBlock() ~ Sleep ~ 1, block height " << height << std::endl;
	while(1) {
		//usleep(100000);
		//std::cout << "initGensisBlock() ~ Sleep ~ 1, block height 1 " << height << std::endl;
		// get task block best height
		CFidTask t = pTaskSchedInit->getTask(seq);
		//std::cout << "initGensisBlock() ~ Sleep ~ 1, block height 2 " << t.height << std::endl;
		if(t.height == -1)
			continue;
		height = ++t.height;
		std::cout << "initGensisBlock() ~ Sleep ~ 1, block height " << height << std::endl;
		break;
	}
	//pTaskSchedInit->catchUp();
	std::cout << "initGensisBlock() ~ Over, block height " << height << std::endl;
	return 0;
}

int main(int argc, char* argv[]) 
{
	std::cout << "FidInventor main begin..." << std::endl;
	
	if(-1 == initConfig(argc, argv)) {
		exit(1);
	}
	std::cout << "FidInventor main pTaskSchedInit call..." << std::endl;
	if(-1 == initGensisBlock()) {
		//exit(1);	
	}
	
	std::cout << "FidInventor main pTaskSched call..." << std::endl;

	// run mutilple thread for taskschedule
	pTaskSched = new fid::CFidTaskSchedule();
	if (pTaskSched == NULL) {
		std::cout << "fid::CFidTaskSchedule MALLOC failed, exit 1" << std::endl;
		exit(1); 
	}
	int seq = pTaskSched->registerTask(fid::TASK_BLOCK_OR_TRANS, i64tostr(height).c_str());
	pTaskSched->run();

	// reg signal
	registerSignalHandler(SIGHUP, HandleSIGHUP);
	registerSignalHandler(SIGINT, HandleSIGTERM);
	registerSignalHandler(SIGHUP, HandleSIGHUP);
	signal(SIGPIPE, SIG_IGN);


	while(bQuit) {
		
		//std::cout << "FidInventor main ret: block height(" << height << "  )over " << std::endl;

		CFidTask t = pTaskSched->getTask(seq);
		if(t.height == -1) {
			//bQuit = 0;
                        continue;
		}
                height = t.height;
		std::cout << "===========FidInventor main task seq : " << seq << ", block height " << height << std::endl;
		//boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
		usleep(2000000);
	}

	std::cout << "FidInventor main over ..." << std::endl;
	fid::deleteThreadSafe(pTaskSchedInit);
	fid::deleteThreadSafe(pTaskSched);
	return 0;
}
