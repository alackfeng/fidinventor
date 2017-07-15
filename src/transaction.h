
#ifndef _FIDINVENTOR_TRANACTION_H_
#define _FIDINVENTOR_TRANACTION_H_

#include <iostream>
#include <string>
#include <vector>


using namespace std;

namespace fid {

typedef int fidtime;

class CFidTxVin {
public:
	string          coinbase; 	// minting coinbase tx
        int             sequence; 	// minting sequence
};

class CFidTxScript {
public:
	string 		sasm;		// tx vout script opcode
	int		reqSigs;	// tx vout script req signature
	string		type;		// tx vout script type
	vector<string>	addresses;	// tx vout addresses list
};

class CFidTxVout {
public:
	string		value; 		// tx vout amount value
	int		n;		// tx vout seq
	CFidTxScript	scriptPubKey;	// tx vout script info
};

class CFidTxDetails {
public:
	string		account;	// tx details account
	string		address;	// tx details address
	string		category;	// tx details category
	double		amount;		// tx details amount
};

class CFidTransaction {
public:
	CFidTransaction();
	int checkValidy();
public:
	string		txid;		// tx id
	int		version;	// tx version
	fidtime		time;		// tx time, default time(1) '1970-01-01 08:00:01', not '1970-01-01 08:00:00':
	fidtime		locktime;	// tx locktime
	//vector<CFidTxVin>	vin;		// tx vin params
	//vector<CFidTxVout>	vout;		// tx vout params
	string		vin;		// tx vin params
	string		vout;		// tx vout params
	double		amount;		// tx amount value
	int		confirmations;  // tx confirmations count
	bool		generated;	// tx generated
	string		blockhash;	// tx belongs to block
	int		blockindex;	// tx belongs to blockindex
	fidtime		blocktime;	// tx block time
	fidtime		timereceived;	// tx received time
	//vector<CFidTxDetails>	details;	// tx details...
	string		details;	// tx details...
};

}


#endif
