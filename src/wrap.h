#ifndef FIDINVENTOR_WRAP_RPC_OR_MYSQL_H
#define FIDINVENTOR_WRAP_RPC_OR_MYSQL_H 

#include "common.h"
#include "transaction.h"
#include "block.h"
#include "taskschedule.h"

#include "mysqldb/mysqldb.h"

namespace fid {

class CFidTask;
class CFidRpcInterface {
	virtual int callRPC(CFidTask &task, string &message) = 0;
};

class CFidRpcWrap: public CFidRpcInterface {
public:
        virtual CFidTask switchParams(CFidTask &task, string &command);
        virtual int callRPC(CFidTask &task, string &message);
private:
        virtual json_spirit::Object callRPC(string cmd, json_spirit::Array params);
};

class CFidDbInterface {
	virtual int insertTx(const CFidTransaction &tx) = 0;
	virtual int insertBk(const CFidBlock &block) = 0;
};

class CFidDbWrap: public CFidDbInterface {
public:
        virtual int insertTx(const CFidTransaction &tx);
        virtual int insertBk(const CFidBlock &block);
};



/*
 * implement rpc or db handle, for more task 
*/
class CFidHandler: public CFidRpcWrap, public CFidDbWrap
{
public:
	CFidHandler(CFidTask &task):task(task), height(-1)  {}

	// handle for rpc
	int64_t getBlockCount(); // return block last height
	vector<string> getBlockByNumber(int height);
	int getTransaction(string txid);
	int insertDBBlockTrans();
	int insertDBBlock();
	int insertDBTrans();

	void reset();
	int getBkTime() {
		return block.time;
	}
protected:
	int switchTx(CFidTask &task, CFidTransaction &tx);	// switch rpc reply to transaction
	vector<string> switchBk(CFidTask &task, CFidBlock &bk);		// switch rpc relpy to block
private:
	CFidTask 	task;
	int64_t		height; 	// block height
	CFidBlock		block;		// block info
	vector<CFidTransaction> vtx;	// block transaction list
};

}

#endif
