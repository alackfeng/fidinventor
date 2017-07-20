#include "wrap.h"


namespace fid {

static std::string dbInstance = "fidchain";
static std::string user = "root";
static std::string passwd = "";
static std::string dbAdress = "127.0.0.1";

CFidTask CFidRpcWrap::switchParams(CFidTask& task, string &command)
{
	// parse command: gettransaction 1323r543r5wsfsfsf
	std::vector<std::string> strCommand;
	boost::split(strCommand, command, boost::is_any_of(" "));
	if (strCommand.size() < 1) {
		return task;
	}
	task.cmd = strCommand[0];

	// convert params to array
	std::vector<std::string> strParams(strCommand.begin() + 1, strCommand.end());
	//std::vector<std::string> strParams(&strCommand[1]);
	json_spirit::Array params = RPCConvertValues(strCommand[0], strParams);
	task.params = params;

	//std::cout << "rpc params: " << task.type << ":" << task.cmd << ":";
	//BOOST_FOREACH(json_spirit::Array item, params)
	//	std::cout << item << ":";
	std::cout << std::endl;
	return task;
}
int CFidRpcWrap::callRPC(CFidTask &task, string &message)
{
	int nRet = 0;

	if(task.cmd == "") {
		return -1;
	}
	task.reply = this->callRPC(task.cmd, task.params);
	
	// Parse reply: eg. {"result":3531,"error":null,"id":1}
	const json_spirit::Value& result = find_value(task.reply, "result");
	const json_spirit::Value& error  = find_value(task.reply, "error");
	if (error.type() != json_spirit::null_type)
	{
		message = "error: " + write_string(error, false);
		int code = find_value(error.get_obj(), "code").get_int();
		nRet = abs(code);
	} else {
		if (result.type() == json_spirit::null_type)
			message = "";
		else if (result.type() == json_spirit::str_type)
			message = result.get_str();
		else if (result.type() == json_spirit::int_type)
			message = strprintf("%d", result.get_int());
		else
			message = write_string(result, true);
	}
	return nRet;
}

json_spirit::Object CFidRpcWrap::callRPC(string cmd, json_spirit::Array params)
{
	json_spirit::Object reply = CallRPC(cmd, params);
	return reply;
}



int CFidDbWrap::insertTx(const CFidTransaction &tx)
{
	CMysqlDb* myDB = CMysqlDb::getinitance();
	// test
	//myDB->queryTest();
	
	myDB->initDb(dbInstance,user,passwd,dbAdress);

	return myDB->insertTx(tx);
}

int CFidDbWrap::insertBk(const CFidBlock &block)
{

	CMysqlDb* myDB = CMysqlDb::getinitance();
	myDB->initDb(dbInstance,user,passwd,dbAdress);
	return myDB->insertBlock(block);
}






int64_t CFidHandler::getBlockCount()
{
	std::cout << "CFidHandler::getBlockCount() call... height " << height << std::endl;
	//int64_t height = -1;
	string message;
	string cmd = "getblockcount";
	CFidTask ftask(TASK_CALLRPC, cmd);
	switchParams(ftask, ftask.command);

	if(0 != callRPC(ftask, message)) {
		std::cout << ftask.cmd << " ~ CMD call RPC failed, msg: " << message << std::endl;
	} else 
	std::cout << ftask.cmd << " ~ CMD call RPC OK, msg: " << static_cast<void*>(&message) << std::endl;
	this->height =atoi64(message);
	return height;
}
      
vector<string> CFidHandler::getBlockByNumber(int height)
{
	std::cout << "CFidHandler::getBlockByNumber() call..., height " << height << std::endl;

	string message;
        string cmd = "getblockbynumber " + strprintf("%d", height);
        CFidTask ftask(TASK_CALLRPC, cmd);
        switchParams(ftask, ftask.command);

	if(0 != callRPC(ftask, message)) {
                std::cout << ftask.cmd << " ~ CMD call RPC failed, msg: " << message << std::endl;
        } else 
        std::cout << ftask.cmd << " ~ CMD call RPC OK, msg: " << static_cast<void*>(&message) << std::endl;

	return switchBk(ftask, this->block);
}

int CFidHandler::getTransaction(string txid)
{
	int iRet = -1;
	std::cout << "CFidHandler::getTransaction() call..., txid " << txid << std::endl;
	
	string message;
        string cmd = "gettransaction " + strprintf("%d", txid);
        CFidTask ftask(TASK_CALLRPC, cmd);
        switchParams(ftask, ftask.command);

	if(0 != callRPC(ftask, message)) {
                std::cout << ftask.cmd << " ~ CMD call RPC failed, msg: " << message << std::endl;
        } else
        std::cout << ftask.cmd << " ~ CMD call RPC OK, msg: " << static_cast<void*>(&message) << std::endl;

	CFidTransaction tx;
	iRet = switchTx(ftask, tx);
	vtx.push_back(tx);
	return iRet;
}


// switch rpc reply to transaction
int CFidHandler::switchTx(CFidTask &task, CFidTransaction &tx)
{
	json_spirit::Object reply = task.reply;

        const json_spirit::Value& result = find_value(reply, "result");
        if (result.type() != json_spirit::obj_type)
                return -1;
        const json_spirit::Object& obj = result.get_obj();
	
	getJsonObjValue(obj, "txid", tx.txid);
	getJsonObjValue(obj, "version", tx.version);
	getJsonObjValue(obj, "time", tx.time);
	getJsonObjValue(obj, "locktime", tx.locktime);
	getJsonObjValue(obj, "amount", tx.amount);
	getJsonObjValue(obj, "blockhash", tx.blockhash);
	getJsonObjValue(obj, "confirmations", tx.confirmations);
	getJsonObjValue(obj, "timereceived", tx.timereceived);
	getJsonObjValue(obj, "details", tx.details);
	getJsonObjValue(obj, "vin", tx.vin);
	getJsonObjValue(obj, "vout", tx.vout);

	// tx vin list details to  CFidTxVin
	json_spirit::Array vinArray; 
	getJsonObjValue(obj, "vin", vinArray);
	
	// tx vout list details to  CFidTxVout
	json_spirit::Array voutArray;
	getJsonObjValue(obj, "vout", voutArray);

	for(int i=0; i<vinArray.size(); i++) {
		const json_spirit::Object vinObj = vinArray[i].get_obj();
		CFidTxVin txvin;
		// pow sequence or coinbase
		getJsonObjValue(vinObj, "sequence", txvin.sequence); // or pow sequence
		if(i==0 && 0 == getJsonObjValue(vinObj, "coinbase", txvin.txid) && txvin.txid != "") {
			txvin.type = 'W';
			tx.vins.push_back(txvin);
			continue;
		}
		// other pos
		txvin.type = 'S';
		getJsonObjValue(vinObj, "txid", txvin.txid); // or pow coinbase
		getJsonObjValue(vinObj, "vout", txvin.vout);
		
		json_spirit::Object script;
		getJsonObjValue(vinObj, "scriptSig", script); 
		getJsonObjValue(script, "hex", txvin.scriptSig.hex);
		getJsonObjValue(script, "asm", txvin.scriptSig.sasm);
		tx.vins.push_back(txvin);
	}
	

	for(int j=0; j<voutArray.size(); j++) {
		const json_spirit::Object voutObj = voutArray[j].get_obj();
		CFidTxVout txvout;
		getJsonObjValue(voutObj, "value", txvout.value);
		if(j==0 && txvout.value == 0.00000000)
			txvout.type = 'W';
		else if(j==1 && txvout.value != 0.00000000)
			txvout.type = 'S';
		else	txvout.type = 'T';
		
		getJsonObjValue(voutObj, "n", txvout.n);

		json_spirit::Object script;
		getJsonObjValue(voutObj, "scriptPubKey", script);
		getJsonObjValue(script, "asm", txvout.scriptPubKey.sasm);
		getJsonObjValue(script, "reqSigs", txvout.scriptPubKey.reqSigs);
		getJsonObjValue(script, "type", txvout.scriptPubKey.type);

		json_spirit::Array address;
		getJsonObjValue(script, "addresses", address);
		if(address.size() >= 1)
			txvout.scriptPubKey.addresses = address[0].get_str();
		//getJsonObjValue(script, "addresses", txvout.scriptPubKey.addresses);
		tx.vouts.push_back(txvout);
	}

	// IsCoinBase() : (vin.size() == 1 && vin[0].prevout.IsNull() && vout.size() >= 1)	
	// IsCoinStake() : (vin.size() > 0 && (!vin[0].prevout.IsNull()) && vout.size() >= 2 && vout[0].IsEmpty())
	if(tx.vins.size() > 0 && !(tx.vouts[0].value == 0.00000000) && tx.vouts.size() >=2 && tx.vouts[0].value == 0.00000000)
		tx.type = 'S';
	if(tx.vins.size() == 1 && tx.vouts[0].value == 0.00000000 && tx.vouts.size() >=1)
		tx.type = 'W';

	return 0;
}

/*
 * switch rpc relpy to block
getblockbynumber ~ CMD call RPC OK, msg: {
    "hash" : "56792130b2c20fe0968c11601fb608943d287d0c304e6c1216175d36c0862a01",
    "confirmations" : 1,
    "size" : 176,
    "height" : 3555,
    "version" : 7,
    "merkleroot" : "4f24d81a9bd721f506d400bd49f0853134fcde71622cdf2481f18997d0015be5",
    "mint" : 37500.00000000,
    "time" : 1499935269,
    "nonce" : 286118,
    "bits" : "1e7fc29f",
    "difficulty" : 0.00003057,
    "blocktrust" : "200f5",
    "chaintrust" : "1bd8900c",
    "previousblockhash" : "2905d392179e4424e82249fe277cd3eeb228ce7ceb79d78a5bb368aba6bd2df6",
    "flags" : "proof-of-work",
    "proofhash" : "00004e880b2666866b5d99edf2c6d71779eb39d1ead771e62706173dac0cd62c",
    "entropybit" : 1,
    "modifier" : "26ec8ec252661a30",
    "tx" : [
        "4f24d81a9bd721f506d400bd49f0853134fcde71622cdf2481f18997d0015be5"
    ]
}
*/
vector<string> CFidHandler::switchBk(CFidTask &task, CFidBlock &bk)
{
	vector<string> vtx;

	json_spirit::Object reply = task.reply;

        const json_spirit::Value& result = find_value(reply, "result");
        if (result.type() != json_spirit::obj_type)
                return vtx;

        const json_spirit::Object& obj = result.get_obj();

        // hash
        const json_spirit::Value& hashid = find_value(obj, "hash");
        if(hashid.type() == json_spirit::str_type) {
                //std::cout << "hashid " << hashid.get_str() << std::endl;
                bk.hash = hashid.get_str();
        }

	// 
        bk.confirmations      	= find_value(obj, "confirmations").get_uint64();
        bk.size     		= find_value(obj, "size").get_int();
        bk.height     		= find_value(obj, "height").get_uint64();
        bk.version      	= find_value(obj, "version").get_int();

        bk.time         	= find_value(obj, "time").get_int64();
        bk.nonce         	= find_value(obj, "nonce").get_int();
        bk.entropybit         	= find_value(obj, "entropybit").get_int();

	// merkleroot
	const json_spirit::Value& merkleroot = find_value(obj, "merkleroot");
        if(merkleroot.type() == json_spirit::str_type) {
                //std::cout << "merkleroot " << merkleroot.get_str() << std::endl;
                bk.merkleroot = merkleroot.get_str();
        }

	// mint
        const json_spirit::Value& mint = find_value(obj, "mint");
        if(mint.type() == json_spirit::real_type) {
                bk.mint = mint.get_real();
        }


        // bits
        const json_spirit::Value& bits = find_value(obj, "bits");
        if(bits.type() == json_spirit::str_type) {
                std::cout << "bits " << bits.get_str() << std::endl;
                bk.bits = bits.get_str();
        }

	// difficulty
        const json_spirit::Value& difficulty = find_value(obj, "difficulty");
        if(difficulty.type() == json_spirit::real_type) {
                bk.difficulty = difficulty.get_real();
        }
	

        // blocktrust
        const json_spirit::Value& blocktrust = find_value(obj, "blocktrust");
        if(blocktrust.type() == json_spirit::str_type) {
                //std::cout << "blocktrust " << blocktrust.get_str() << std::endl;
                bk.blocktrust = blocktrust.get_str();
        }


	// chaintrust
        const json_spirit::Value& chaintrust = find_value(obj, "chaintrust");
        if(chaintrust.type() == json_spirit::str_type) {
                //std::cout << "chaintrust " << chaintrust.get_str() << std::endl;
                bk.chaintrust = chaintrust.get_str();
        }

	// previousblockhash
        const json_spirit::Value& previousblockhash = find_value(obj, "previousblockhash");
        if(previousblockhash.type() == json_spirit::str_type) {
                //std::cout << "previousblockhash " << previousblockhash.get_str() << std::endl;
                bk.previousblockhash = previousblockhash.get_str();
        }

	// nextblockhash
        const json_spirit::Value& nextblockhash = find_value(obj, "nextblockhash");
        if(nextblockhash.type() == json_spirit::str_type) {
                //std::cout << "nextblockhash " << nextblockhash.get_str() << std::endl;
                bk.nextblockhash = nextblockhash.get_str();
        }

	// flags
        const json_spirit::Value& flags = find_value(obj, "flags");
        if(flags.type() == json_spirit::str_type) {
                //std::cout << "flags " << flags.get_str() << std::endl;
                bk.flags = flags.get_str();
        }


	// proofhash
        const json_spirit::Value& proofhash = find_value(obj, "proofhash");
        if(flags.type() == json_spirit::str_type) {
                //std::cout << "proofhash " << proofhash.get_str() << std::endl;
                bk.proofhash = proofhash.get_str();
        }

	// modifier
        const json_spirit::Value& modifier = find_value(obj, "modifier");
        if(modifier.type() == json_spirit::str_type) {
                //std::cout << "modifier " << modifier.get_str() << std::endl;
                bk.modifier = modifier.get_str();
        }

	// tx array
        const json_spirit::Value& txarray = find_value(obj, "tx");
        if(txarray.type() == json_spirit::array_type) {
                const json_spirit::Array& array = txarray.get_array();
                bk.tx = json_spirit::write_string(txarray, true);

		for(int i=0; i<array.size(); i++) {
			vtx.push_back(array[i].get_str());
		}
		bk.vtx = vtx;
        }
	return vtx;
}

int CFidHandler::insertDBBlockTrans()
{
	insertDBBlock();
	insertDBTrans();
	return 0;
}

int CFidHandler::insertDBBlock()
{
	// check block and insert db 
        if(block.hash == "" || !block.checkValidy()) {
		return -1;
        }
        return insertBk(block);
}
int CFidHandler::insertDBTrans()
{
	int iRet = 0;
	// check transaction and insert db
        vector<CFidTransaction>::iterator it = vtx.begin();
        for(; it != vtx.end(); it++) {
		if((*it).txid == "" || !(*it).checkValidy()) {
			continue;
		}
                iRet |= insertTx(*it);
        }
	return iRet;
}

void CFidHandler::reset()
{
	block.clear();
	vector<CFidTransaction>().swap(vtx);
}



}


