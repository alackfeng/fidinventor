
#include "transaction.h"

namespace fid {

CFidTransaction::CFidTransaction()
:txid(""), version(1), time(1), locktime(1), vin(""), vout(""), amount(0.000000), 
confirmations(0), generated(0), blockhash(""), blockindex(-1), blocktime(0), timereceived(0), details("blockchain fid")
{
	string          txid;           // tx id
        int             version;        // tx version
        fidtime         time;           // tx time
        fidtime         locktime;       // tx locktime
        //vector<CFidTxVin>     vin;            // tx vin params
        //vector<CFidTxVout>    vout;           // tx vout params
        string          vin;            // tx vin params
        string          vout;           // tx vout params
        double          amount;         // tx amount value
        int             confirmations;  // tx confirmations count
        bool            generated;      // tx generated
        string          blockhash;      // tx belongs to block
        int             blockindex;     // tx belongs to blockindex
        fidtime         blocktime;      // tx block time
        fidtime         timereceived;   // tx received time
        //vector<CFidTxDetails> details;        // tx details...
        string          details;        // tx details...

}

int CFidTransaction::checkValidy()
{
	if(txid == "")
		return 0;
	return 1;
}

}
