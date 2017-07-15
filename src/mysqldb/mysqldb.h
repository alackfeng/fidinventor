#ifndef CMYSQLDB_H
#define CMYSQLDB_H

#include <iostream>
#include <string>
#include <cstdlib>
#include "../block.h"
#include "../utils.h"
#include "../transaction.h"
#include <pthread.h>
#include <limits>


#include <mysql++.h>


using namespace std;
using namespace fid; // fid namespace for parseblock 




class CMysqlDb
{
private:
	CMysqlDb();
	CMysqlDb(const CMysqlDb&  );
	
public:
	~CMysqlDb();

   	int initDb(const string &dbInstance,const string& user,const string &passwd,const string& dbaddress );

    	static CMysqlDb* getinitance();
    
   	int insertBlock(const CFidBlock &block);
   
   	/*Add by Taurus.Feng at 20170712 for query gettransaction rpc cmd to insert into mysqldb table `blocktransaction` */
   	int insertTx(const CFidTransaction &tx);
	int queryTest();
	int queryBlockCount();
   
private:
	mysqlpp::Connection m_con;

	static pthread_mutex_t m_mutex;
	
	static CMysqlDb* m_p;
	int inited;
};
#endif



