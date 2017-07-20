#ifndef FIDINVENTOR_WRAP_RPC_OR_MYSQL_H
#define FIDINVENTOR_WRAP_RPC_OR_MYSQL_H 

#include "common.h"
#include "config-in.h"
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
protected:
	inline int getJsonObjValue(const json_spirit::Object& obj, const char* key, json_spirit::Array& ret) { // json_spirit::array_type
		const json_spirit::Value& value = find_value(obj, key);
		ret = value.type() == json_spirit::array_type ? value.get_array() : json_spirit::Array();
		return 0;
	}
	inline int getJsonObjValue(const json_spirit::Object& obj, const char* key, json_spirit::Object& ret) { // json_spirit::obj_type
		const json_spirit::Value& value = find_value(obj, key);
		ret = value.type() == json_spirit::obj_type ? value.get_obj() : json_spirit::Object();
		return 0;
	}
	inline int getJsonObjValue(const json_spirit::Object& obj, const char* key, string& ret) { // json_spirit::str_type
		//std::cout << "getJsonObjValue()================= " << key << ":string->" << std::endl;
		const json_spirit::Value& value = find_value(obj, key);
		if(value.type() == json_spirit::array_type) { //array to string
			ret = json_spirit::write_string(value, true);
		} else if(value.type() == json_spirit::str_type) {
			ret = value.type() == json_spirit::str_type ? value.get_str() : "";
	//	std::cout << "getJsonObjValue()================= " << key << ":string->" << ret << std::endl;
			
		}
		return 0;
	}
	inline int getJsonObjValue(const json_spirit::Object& obj, const char* key, bool& ret) { // json_spirit::bool_type
		const json_spirit::Value& value = find_value(obj, key);
		ret = value.type() == json_spirit::bool_type ? value.get_bool() : 0;
		return 0;
	}
	inline int getJsonObjValue(const json_spirit::Object& obj, const char* key, int& ret) { // json_spirit::int_type
	//	std::cout << "getJsonObjValue()================= " << key << ":int" << std::endl;
		const json_spirit::Value& value = find_value(obj, key);
		ret = value.type() == json_spirit::int_type ? value.get_int() : 0;
		return 0;
	}
	inline int getJsonObjValue(const json_spirit::Object& obj, const char* key, double& ret) { // json_spirit::real_type
	//j	std::cout << "getJsonObjValue()================= " << key << ":real" << std::endl;
		const json_spirit::Value& value = find_value(obj, key);
		ret = value.type() == json_spirit::real_type ? value.get_real() : 0.00000000;
		return 0;
	}
	inline int getJsonObjValue(const json_spirit::Object& obj, const char* key, fidint& ret) { // json_spirit::int_type fidint
	//	std::cout << "getJsonObjValue()================= " << key << ":fidint" << std::endl;
		const json_spirit::Value& value = find_value(obj, key);
		ret = value.type() == json_spirit::int_type ? value.get_uint64() : 0;
		return 0;
	}
	inline int getJsonObjValue(const json_spirit::Object& obj, const char* key, fidtime& ret) { // json_spirit::int_type fidtime
	//	std::cout << "getJsonObjValue()================= " << key << ":fidtime" << std::endl;
		const json_spirit::Value& value = find_value(obj, key);
		ret = value.type() == json_spirit::int_type ? value.get_int64() : 0;
		return 0;
	}
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
