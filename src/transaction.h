
#ifndef _FIDINVENTOR_TRANACTION_H_
#define _FIDINVENTOR_TRANACTION_H_

#include "config-in.h"

namespace fid {



/* script public key  */
class CFidTxScriptPubKey {
public:
	string 		sasm;		// tx vout script opcode
	int		reqSigs;	// tx vout script req signature
	string		type;		// tx vout script type
	vector<string>	addresses;	// tx vout addresses list, to blob
};

/* script sigatures  */
class CFidTxScriptSig {
public:
	string		sasm;		//
	string		hex;		// 
};

class CFidTxVin {
public:
	char		type;		// transaction type: pow | pos | trans
	union {
	struct pow{ // pow
		string          coinbase; 	// minting coinbase tx
		int             sequence; 	// minting sequence
	};
	typedef struct pos_s{ // pos
		string		txid;
		int		vout;
		CFidTxScriptSig scriptSig;
		int		sequence;	
	}pos_t;
	}u;

	fidint			value; 		// tx vin amount value
	vector<string>		addresses;	// tx vin addresses list ?, to blob
};

class CFidTxVout {
public:
	char		type;		// transaction type: pow | pos | trans
	
	fidint			value; 		// tx vout amount value
	int			n;		// tx vout position
	CFidTxScriptPubKey	scriptPubKey;	// tx vout script info
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
	std::string toString() const;
	void clear();
public:
	char		type;		// tx type: pow | pos | trans
	string		txid;		// tx id
	int		version;	// tx version
	fidtime		time;		// tx time, default time(1) '1970-01-01 08:00:01', not '1970-01-01 08:00:00':
	fidtime		locktime;	// tx locktime
	vector<CFidTxVin>	vins;		// tx vin params
	vector<CFidTxVout>	vouts;		// tx vout params
	string		vin;		// tx vin params
	string		vout;		// tx vout params
	double		amount;		// tx amount value
	fidint		confirmations;  // tx confirmations count
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
