#include "mysqldb.h"
#include <pthread.h>




CMysqlDb* CMysqlDb::m_p = NULL;

pthread_mutex_t CMysqlDb::m_mutex;

#define SQl_BUFFER_MAX_COUNT 65523


CMysqlDb::CMysqlDb(): inited(0)
{
	// construction
	mysqlpp::Connection m_con(false);
    	cout<<"CMysqlDb() call..."<<endl;
}

CMysqlDb::CMysqlDb(const CMysqlDb&): inited(0)
{
    	cout<<"CMysqlDb::CMysqlDb() call..."<<endl;
}

CMysqlDb::~CMysqlDb()
{
  cout<<"~CMysqlDb() called"<<endl;
}

CMysqlDb* CMysqlDb::getinitance()
{
	if (m_p == NULL)
	  {
		  pthread_mutex_lock(&m_mutex);
		  if (m_p == NULL)
			  m_p = new CMysqlDb();
		  pthread_mutex_unlock(&m_mutex);
	  }
	  return m_p;
}

int CMysqlDb::initDb(const string &dbInstance,const string& user,const string &passwd,const string& dbaddress )
{
	if(inited) {
		return 1;
	}

	m_con.set_option(new mysqlpp::SetCharsetNameOption("gbk"));
	
	if(!m_con.connect(dbInstance.c_str(),dbaddress.c_str(),user.c_str(),passwd.c_str()))
	{
	   cout<< "CMysqlDb::initDb() connect mysqldb failed!!" <<endl;
	   return -1;
	}
   	cout<< "CMysqlDb::initDb() connect mysqldb initDB ok!!" <<endl;
	inited = 1;
	return 0;
}


int CMysqlDb::insertBlock(const CFidBlock &block)
{

	try {
		cout << "CMysqlDb::insertBlock() ~ Insert <fidchain.blockinfo> table..." << endl;
                //block.toString();

		mysqlpp::Query query = m_con.query();

                query << "insert into %20:table(bk_hash, bk_confirmations, bk_size, bk_height, bk_version, bk_merkleroot, bk_mint, bk_time, bk_nonce, bk_bits, "
			"bk_difficulty, bk_blocktrust, bk_chaintrust, bk_previousblockhash, bk_nextblockhash, bk_flags, bk_proofhash, bk_entropybit, bk_modifier, bk_vtx) values " <<
                        "(%0q:hash, %1:confirmations, %2:size, %3:height, %4:version, %5q:merkleroot, %6:mint, %7q:time, %8:nonce, %9q:bits, %10:difficulty, "
			"%11q:blocktrust, %12q:chaintrust, %13q:previousblockhash, %14q:nextblockhash, %15q:flags, %16q:proofhash, %17:entropybit, %18q:modifier, %19q:vtx) ";

		query.parse();
                query.template_defaults["table"] = "blockinfo";
                query.template_defaults["hash"] = block.hash.c_str() == "" ? "need block hash" : block.hash.c_str();
                query.template_defaults["confirmations"] = block.confirmations;
                query.template_defaults["size"] = block.size;
                query.template_defaults["height"] = block.height;
                query.template_defaults["version"] = block.version;
                query.template_defaults["merkleroot"] = block.merkleroot.c_str();
                query.template_defaults["mint"] = block.mint;
                query.template_defaults["time"] = DateTimeStrFormat(block.time).c_str();
                query.template_defaults["nonce"] = block.nonce;
                query.template_defaults["bits"] = block.bits.c_str();
                query.template_defaults["difficulty"] = block.difficulty;
                query.template_defaults["blocktrust"] = block.blocktrust.c_str();
                query.template_defaults["chaintrust"] = block.chaintrust.c_str();
                query.template_defaults["previousblockhash"] = block.previousblockhash.c_str();
                query.template_defaults["nextblockhash"] = block.nextblockhash.c_str();
                query.template_defaults["flags"] = block.flags.c_str();
                query.template_defaults["proofhash"] = block.proofhash.c_str();
                query.template_defaults["entropybit"] = block.entropybit;
                query.template_defaults["modifier"] = block.modifier.c_str();
                query.template_defaults["vtx"] = block.tx.c_str();

		query.execute();
                cout << "CMysqlDb::insertBlock() ~ inserted... " << m_con.count_rows("blockinfo") << " rows." << endl;
		

	} catch (const mysqlpp::BadQuery& er) {
                // Handle any query errors
                cerr << endl << "Query error: " << er.what() << endl;
                return 1;
        } catch (const mysqlpp::BadConversion& er) {
                // Handle bad conversions
                cerr << endl << "Conversion error: " << er.what() << endl <<
                                "\tretrieved data size: " << er.retrieved <<
                                ", actual size: " << er.actual_size << endl;
                return 2;
        } catch (const mysqlpp::Exception& er) {
                // Catch-all for any other MySQL++ exceptions
                cerr << endl << "Error: " << er.what() << endl;
                return 3;
        }

        return 0;

}

int CMysqlDb::insertTx(const CFidTransaction &tx)
{

	try {

		cout << "CMysqlDb::insertTx() ~ Insert <fidchain.blocktransaction> table..." << endl;
		mysqlpp::Query query = m_con.query();	
		
		query << "insert into %14:table(tx_id, tx_version, tx_time, tx_locktime, tx_iscoinbase, tx_vin, tx_vout, tx_amount, tx_confirmations, tx_generated, tx_blockhash, tx_blocktime, tx_timereceived, tx_details) values " <<
			"(%0q:id, %1:version, %2q:time, %3q:locktime, %4:iscoinbase, %5q:vin, %6q:vout, %7:amount, %8:confirmations, %9:generated, %10q:blockhash, %11q:blocktime, %12q:timereceived, %13q:details)";
		query.parse();
		query.template_defaults["table"] = "blocktransaction";
		query.template_defaults["id"] = tx.txid.c_str() == "" ? "needed tx hash" : tx.txid.c_str();
		query.template_defaults["version"] = tx.version;
		query.template_defaults["time"] = DateTimeStrFormat(tx.time).c_str();
		query.template_defaults["locktime"] = DateTimeStrFormat(tx.locktime+1).c_str();
		query.template_defaults["iscoinbase"] = 0;
		query.template_defaults["vin"] = tx.vin.c_str();
		query.template_defaults["vout"] = tx.vout.c_str();
		query.template_defaults["amount"] = tx.amount;
		query.template_defaults["confirmations"] = tx.confirmations;
		query.template_defaults["generated"] = mysqlpp::null;
		query.template_defaults["blockhash"] = tx.blockhash.c_str() == "" ? "needed block hash" : tx.blockhash.c_str();
		query.template_defaults["blocktime"] = mysqlpp::null;
		query.template_defaults["timereceived"] = mysqlpp::null;
		query.template_defaults["details"] = tx.details.c_str();
		//query.execute("sdfsfsgggg", 97, mysqlpp::sql_date("2006-03-06 00:00:00"), 1, "sdfsfsf", 8.79, "fffffffffffff", "blocktransaction");
		//query.execute(block.txid.c_str(), mysqlpp::null, mysqlpp::null, mysqlpp::sql_date("2006-03-06 00:00:00"), 1, "vin", "vout", 8.790000, 13, 1, 
	//		"5038c6435cfdfbbc292510318719420482bc3d6de076cf5dc75cf86c22fa1526", mysqlpp::sql_timestamp("2006-03-06 00:00:00"), mysqlpp::sql_timestamp("2006-03-06 00:00:00"), "blocktransaction");

		query.execute();
		cout << "CMysqlDb::insertTx() ~ inserted... " << m_con.count_rows("blocktransaction") << " rows." << endl;
		
	} catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
                cerr << endl << "Query error: " << er.what() << endl;
                return 1;
	} catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions
                cerr << endl << "Conversion error: " << er.what() << endl <<
                                "\tretrieved data size: " << er.retrieved <<
                                ", actual size: " << er.actual_size << endl;
                return 2;
	} catch (const mysqlpp::Exception& er) {
                // Catch-all for any other MySQL++ exceptions
                cerr << endl << "Error: " << er.what() << endl;
                return 3;
        }

	return 0;
}

int CMysqlDb::queryTest()
{
	m_con.set_option(new mysqlpp::SetCharsetNameOption("gbk"));

        if(!m_con.connect("mysql", "127.0.0.1", "root", ""))
        {
           cout<< "connect mysqldb failed!!" <<endl;
           return -1;
        }

	mysqlpp::Query query = m_con.query("select Host,User,password_expired,authentication_string,password_expired from user");
	if(mysqlpp::StoreQueryResult res = query.store()) {
		cout << "We have:" << endl;

		mysqlpp::StoreQueryResult::const_iterator it;
		for (it = res.begin(); it != res.end(); ++it) {
			mysqlpp::Row row = *it;
			//cout << '\t' << row["mycol"] << endl;			
			cout << "\tHost: " << row[0] << ", User: " << row[1] << ", password_expired: " 
			<< row[2] << ", authentication_string: " << row[3] << ", password_expired: " << row[4] << endl;
		}
	} else {
		cout << "Failed to get table blocktransaction : " << query.error() << endl;
	}
	
	return 0;
}

int CMysqlDb::queryBlockCount()
{
	int count = 0;

	mysqlpp::Query query = m_con.query("select count(*) from blockinfo");
	if(mysqlpp::StoreQueryResult res = query.store()) {	
		cout << "CMysqlDb::queryBlockCount() ~ count : " << res[0][0] << endl;
		count = res[0][0];
	}

	return count;
}


