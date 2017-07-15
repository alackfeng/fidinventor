//

#ifndef FIDINVERNT_BLOCK_H
#define FIDINVERNT_BLOCK_H

#include <string>
#include <vector>


using namespace std; 


class CFidBlock
{
public:
	CFidBlock() {}
	~CFidBlock() {}
	int checkValidy();
	std::string toString() const;
	void clear();
public:
  	  std::string 		hash; 
	  int 			confirmations;
	  int 			size;
	  int			height;
	  int 			version;
	   
	  std::string 		merkleroot;
	  double		mint;
	  int 			time;
	  int 			nonce;
	  std::string 		bits;
		
	  double 		difficulty;
	  std::string 		blocktrust;
	  std::string		chaintrust;
	  std::string 		previousblockhash;
	  std::string 		nextblockhash;
	  std::string  		flags;
	  std::string 		proofhash;
	  int 			entropybit;
	  std::string 		modifier;
	  std::string		tx;
	  std::vector<std::string> vtx;
};


#endif







