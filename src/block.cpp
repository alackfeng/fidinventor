//
#include "block.h"
#include <iostream>


std::string CFidBlock::toString() const
{
	std::cout << "CFidBlock ~"
	<< "hash(" << hash << "), "
	<< "confirmations(" << confirmations << "), "
	<< "size(" << size << "), "
	<< "height(" << height << "), "
	<< "version(" << version << "), "
	<< "merkleroot(" << merkleroot << "), "
	<< "mint(" << mint << "), "
	<< "time(" << time << "), "
	<< "nonce(" << nonce << "), "
	<< "bits(" << bits << "), "
	<< "difficulty(" << difficulty << "), "
	<< "blocktrust(" << blocktrust << "), "
	<< "chaintrust(" << chaintrust << "), "
	<< "previousblockhash(" << previousblockhash << "), "
	<< "nextblockhash(" << nextblockhash << "), "
	<< "flags(" << flags << "), "
	<< "proofhash(" << proofhash << "), "
	<< "entropybit(" << entropybit << "), "
	<< "modifier(" << modifier << "), "
	<< "tx(" << tx << "), "
	<< std::endl;
	return "";
}

int CFidBlock::checkValidy()
{
	if(hash == "")
		return 0;
	return 1;
}
void CFidBlock::clear() {
	hash = "";
	confirmations = 0;
	size = 0;
	height = -1;
	version = 0;
	merkleroot = "";
	mint = 0;
	time = 0;
	nonce = 0;
	bits = "";
	difficulty = 0;
	blocktrust = "";
	chaintrust = "";
	previousblockhash = "";
	nextblockhash = "";
	flags = "";
	proofhash = "";
	entropybit = 0;
	modifier = "";
	tx = "";
	
}


