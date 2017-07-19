
#include "transaction.h"

namespace fid {

CFidTransaction::CFidTransaction()
:txid(""), version(1), time(1), locktime(1), vin(""), vout(""), amount(0.000000), 
confirmations(0), generated(0), blockhash(""), blockindex(-1), blocktime(0), timereceived(0), details("blockchain fid"),
type('T')
{
	vins.clear();
	vouts.clear();
}

int CFidTransaction::checkValidy()
{
	if(txid == "")
		return 0;
	return 1;
}

std::string CFidTransaction::toString() const
{
	std::cout << "CFidTransaction::++++++++++++++++++++++++++++++++++++++++++ start~"
        << "txid(" << txid << "), "
        << "confirmations(" << confirmations << "), "
        << "time(" << time << "), "
        << "locktime(" << locktime << "), "
        << "version(" << version << "), "
        << "amount(" << amount << "), "
        << "generated(" << generated << "), "
        << "blockhash(" << blockhash << "), "
        << "blockindex(" << blockindex << "), "
        << "blocktime(" << blocktime << "), "
        << "timereceived(" << timereceived << "), "
        << "type(" << type << "), "
        << "vin(" << vin << "), "
        << "vout(" << vout << "), "
        << "CFidBlock::++++++++++++++++++++++++++++++++++++++++++  over~"
        << std::endl;

	return "";
}
void CFidTransaction::clear()
{
	txid = "";
	type = 'T';
	version = 0;
	time = 0;
	locktime = 0;
	vin = "";
	vout = "";
	amount = 0;
	confirmations = 0;
	generated = 0;
	blockhash = "";
	blockindex = 0;
	blocktime = 0;
	timereceived = 0;
	details = "";
	vins.clear();
	vouts.clear();
}

}


