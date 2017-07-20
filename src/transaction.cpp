
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
	std::cout << "CFidTransaction::++++++++++++++++++++++++++++++++++++++++++ start~\n"
        << "txid(" << txid << "), \n"
        << "confirmations(" << confirmations << "), \n"
        << "time(" << time << "), \n"
        << "locktime(" << locktime << "), \n"
        << "version(" << version << "), \n"
        << "amount(" << amount << "), \n"
        << "generated(" << generated << "), \n"
        << "blockhash(" << blockhash << "), \n"
        << "blockindex(" << blockindex << "), \n"
        << "blocktime(" << blocktime << "), \n"
        << "timereceived(" << timereceived << "), \n"
        << "type(" << type << "), \n";
        //<< "vin(" << vin << "), \n"
        //<< "vout(" << vout << "), \n"
	for(int i=0; i<vins.size(); i++) {
		CFidTxVin vin = vins[i];
		cout << "vin(" << i << "-" << vin.type << ":" << vin.sequence << ":" << vin.txid << "), \n";
		cout << "vin(" << i << "-" << vin.vout << ":" << vin.scriptSig.sasm << ":" << vin.scriptSig.hex << "), \n";
	}
	for(int i=0; i<vouts.size(); i++) {
		CFidTxVout vin = vouts[i];
		cout << "vout(" << i << "-" << vin.type << ":" << vin.value << ":" << vin.n << "), \n";
		cout << "vout(" << i << "-" << vin.scriptPubKey.reqSigs << ":" << vin.scriptPubKey.type << ":" << vin.scriptPubKey.sasm << ":" << vin.scriptPubKey.addresses << "), \n";
	}
        cout << "CFidBlock::++++++++++++++++++++++++++++++++++++++++++  over~"
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

#if 0
string CFidTransaction::listToString(const string& addresses)
{

	std::ostringstream oss;
	std::stringstream ss(addresses);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	std::copy(vstrings.begin(), vstrings.end(), std::ostream_iterator<std::string>(oss, "|"));
	std::cout << "======================================================================================CFidTransaction::listToString() ---- " << oss.str() << std::endl;
	return oss.str();
	#if 0
	for(int i=0; i<addresses.length(); i++) {
		if(addresses.find('[') || addresses.find(']') || addresses.find(' ')) {
			continue;
		}
	}
	return "";
	std::ostringstream oss;
	std::copy(vec.begin(), vec.end()-1, std::ostream_iterator<int>(oss, ","));
	oss << vec.back();
	return oss.str();
	#endif
}
#endif
}


