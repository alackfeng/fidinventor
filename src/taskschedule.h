
#ifndef _FIDINVENTOR_TASKSCHEDULE_H
#define _FIDINVENTOR_TASKSCHEDULE_H

#include "common.h"

//#include "transaction.h"
//#include "rpcclient.h"


#define UNI_SEQ	int

namespace fid {

typedef enum TaskType_e{
	TASK_NULL,
	TASK_BLOCKGENSIS,	// from gensis block 0 ~ bestheight
	TASK_BLOCKINFO,
	TASK_BLOCKHEIGHT,
	TASK_BLOCK,
	TASK_TRANSACTION,
	TASK_BLOCK_OR_TRANS,

	TASK_CALLRPC,
	TASK_CALLSQL,
	TASK_ALL
}TaskType_t;


class CFidTask {
public:
	CFidTask(): type(TASK_NULL), status(0), sequence(-1), height(-1) {}
	CFidTask(TaskType_t type, string &command): status(0), message(""), sequence(-1), height(-1)
	{ 
		this->type = type; 
		this->command = command; 
	}

	int call();

	int			height;		// current task best block height
	int			bktime;		// block time

	TaskType_t		type;		// task type
	string			command;	// task original command string
	int			sequence;	// task unique sequence

	int			status;		// task status
	string			message;	// task message
	void*			result;		// task return result

	string			cmd;		// task rpc command
	json_spirit::Array	params;		// task rpc params
	json_spirit::Object	reply;		// task rpc reply result
};

class CFidTaskSchedule: public Thread
{
public:
	CFidTaskSchedule(): onceRun(0) {} //
	CFidTaskSchedule(int once): onceRun(once) {}	// task thread run only once 
	
	void work(); // implement thread work handle

	int registerTask(TaskType_t type, const char *command);
	CFidTask getTask(int seq);
	list<CFidTask> getTaskList();

	/* mysql db block info  */
	
protected:
	int registerTask(TaskType_t type, const string &command);
private:
	list<CFidTask>		tasks;
	int 			onceRun; // task thread run only once
public:
	static int		sequence; // task unique sequence
};

}

#endif
