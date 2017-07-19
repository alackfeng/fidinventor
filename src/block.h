//

#ifndef FIDINVERNT_BLOCK_H
#define FIDINVERNT_BLOCK_H

#include "config-in.h"

namespace fid {

class CFidBlock
{
public:
	CFidBlock():hash("") {}
	~CFidBlock() {}
	int checkValidy();
	std::string toString() const;
	void clear();
public:
  	std::string 		hash; 
	fidint 			confirmations;
	int 			size;
	fidint			height;
	int 			version;
	   
	std::string 		merkleroot;
	double			mint;
	fidtime 		time;
	int 			nonce;
	std::string 		bits;
		
	double 			difficulty;
	std::string 		blocktrust;
	std::string		chaintrust;
	std::string 		previousblockhash;
	std::string 		nextblockhash;
	std::string  		flags;
	std::string 		proofhash;
	int 			entropybit;
	std::string 		modifier;
	std::string		tx;
	std::vector<std::string> vtx; // for  CFidTransaction
	std::string		signature;
};

}

#endif







