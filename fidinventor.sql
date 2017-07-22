## https://tangentsoft.com/mysqlpp/home
## http://liuweipingblog.cn/cpp/install-use-mysql-on-linuxdebian-ubuntu/
## sudo apt-get install libmysqlclient-dev libmysqld-dev libmysql++-dev libmysql++-doc libmysql++3v5
## locate libmysqlpp.so



## show variables like 'sql_mode';
## set session
## sql_mode='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION'
## show variables like 'sql_mode';


create database `fidchainblock`;
use `fidchainblock`;
DROP TABLE IF EXISTS `fidchainblock`.`blocktransaction`;
CREATE TABLE `fidchainblock`.`blocktransaction` (
	 `seq_id` INT(10) NOT NULL AUTO_INCREMENT COMMENT 'auto increment sequence',
    `tx_id` VARCHAR(256) NOT NULL COMMENT 'block transaction txid',
	 `tx_version` INT(1) DEFAULT NULL COMMENT 'block transaction version',
    `tx_time` TIMESTAMP DEFAULT '0000-00-00 00:00:00' COMMENT 'block transaction time',
    `tx_locktime` TIMESTAMP DEFAULT '0000-00-00 00:00:00' COMMENT 'block transaction locktime',
    `tx_type` CHAR(1) NOT NULL DEFAULT 'T' COMMENT 'block transaction type pow(W) or pos(S)',
    `tx_vin` BLOB DEFAULT NULL COMMENT 'block transaction vin multiple input params',
	 `tx_vout` BLOB DEFAULT NULL COMMENT 'block transaction vout multiple output params',
	 `tx_amount` DOUBLE(32,8) DEFAULT 0 COMMENT 'block transaction amount',
	 `tx_confirmations` INT(1) DEFAULT 0 COMMENT 'block transaction confirmations',
 	 `tx_generated` INT(1) DEFAULT 0 COMMENT 'block transaction generated 0 or 1',
    `tx_blockhash` VARCHAR(256) NOT NULL COMMENT 'block transaction belong to blockhash',
 	 `tx_blockindex` INT(1) DEFAULT 0 COMMENT 'block transaction belong to blockindex',
    `tx_blocktime` TIMESTAMP DEFAULT '0000-00-00 00:00:00' COMMENT 'block transaction blocktime',
    `tx_timereceived` TIMESTAMP DEFAULT '0000-00-00 00:00:00' COMMENT 'block transaction timereceived',
    `tx_details` BLOB DEFAULT NULL COMMENT 'block transaction details',
    `create_time` TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT 'table record create time',
    `modify_time` TIMESTAMP DEFAULT '0000-00-00 00:00:00' COMMENT 'table record modify time',
    PRIMARY KEY (`seq_id`),
	 UNIQUE KEY block_tx_key (`tx_id`),
	 INDEX block_tx_index (`tx_id`, `tx_blockhash`)
);

DROP TABLE IF EXISTS `fidchainblock`.`block`;
CREATE TABLE `fidchainblock`.`block` (
	 `seq_id` INT(10) NOT NULL AUTO_INCREMENT COMMENT 'auto increment sequence',
    `bk_hash` VARCHAR(256) NOT NULL COMMENT 'block hash id',
	 `bk_confirmations` INT(10) DEFAULT NULL COMMENT 'block confirmations',
    `bk_size` INT(10) DEFAULT NULL COMMENT 'block size',
    `bk_height` INT(10) DEFAULT NULL COMMENT 'block height',
    `bk_version` INT(1) DEFAULT 1 COMMENT 'block version',
    `bk_merkleroot` VARCHAR(256) NOT NULL COMMENT 'block merkle root',
	 `bk_mint` DOUBLE(32,8) DEFAULT NULL COMMENT 'block mint',
	 `bk_time` TIMESTAMP DEFAULT '0000-00-00 00:00:00' COMMENT 'block time',
	 `bk_nonce` INT(10) DEFAULT 0 COMMENT 'block nonce',
 	 `bk_bits` VARCHAR(256) NOT NULL COMMENT 'block bits',
    `bk_difficulty` DOUBLE(32,8) DEFAULT NULL COMMENT 'block difficulty',
 	 `bk_blocktrust` VARCHAR(256) NOT NULL COMMENT 'block block trust',
 	 `bk_chaintrust` VARCHAR(256) NOT NULL COMMENT 'block chain trust',
    `bk_previousblockhash` VARCHAR(256) DEFAULT NULL COMMENT 'block previousblockhash',
    `bk_nextblockhash` VARCHAR(256) DEFAULT NULL COMMENT 'block nextblockhash',
    `bk_flags` VARCHAR(256) NOT NULL COMMENT 'block flags',
    `bk_proofhash` VARCHAR(256) NOT NULL COMMENT 'block proofhash',
    `bk_entropybit` INT(10) DEFAULT 0 COMMENT 'block entropybit',
    `bk_modifier` VARCHAR(256) NOT NULL COMMENT 'block modifier',
	 `bk_vtx` BLOB DEFAULT NULL COMMENT 'block transaction list',
    `create_time` TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT 'table record create time',
    `modify_time` TIMESTAMP DEFAULT '0000-00-00 00:00:00' COMMENT 'table record modify time',
    PRIMARY KEY (`seq_id`),
	 UNIQUE KEY block_info_key (`bk_hash`)
);

DROP TABLE IF EXISTS `fidchainblock`.`blocktx_vout`;
CREATE TABLE `fidchainblock`.`blocktx_vout` (
	 `seq_id` INT(10) NOT NULL AUTO_INCREMENT COMMENT 'auto increment sequence',
    `bktx_txid` VARCHAR(256) NOT NULL COMMENT 'block transaction txid',
    `bktx_blockhash` VARCHAR(256) DEFAULT NULL COMMENT 'block transaction belong to blockhash',
    `bktx_type` CHAR(1) NOT NULL DEFAULT 'T' COMMENT 'block transaction type, pow|pos|trans',
	 `bktx_voutvalue` DOUBLE(32,8) DEFAULT 0 COMMENT 'block transaction vout amount',
	 `bktx_voutn` INT(1) DEFAULT 0 COMMENT 'block transaction vout position',
    `bktx_voutaddresses` VARCHAR(256) DEFAULT NULL COMMENT 'block transaction vout address',
    `bktx_voutstasm` VARCHAR(256) DEFAULT NULL COMMENT 'block transaction vout script asm',
    `bktx_voutstreqSigs` VARCHAR(256) DEFAULT NULL COMMENT 'block transaction vout script reqSigs',
    `bktx_voutsttype` VARCHAR(256) DEFAULT NULL COMMENT 'block transaction vout script type',
    `create_time` TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT 'table record create time',
    `modify_time` TIMESTAMP DEFAULT '0000-00-00 00:00:00' COMMENT 'table record modify time',
    PRIMARY KEY (`seq_id`),
	 INDEX blocktx_vout_index (`bktx_txid`),
	 INDEX blocktx_vout_index_vin (`bktx_txid`, `bktx_voutn`),
	 INDEX blocktx_vout_index_address (`bktx_voutaddresses`)
);

DROP TABLE IF EXISTS `fidchainblock`.`blocktx_vin`;
CREATE TABLE `fidchainblock`.`blocktx_vin` (
	 `seq_id` INT(10) NOT NULL AUTO_INCREMENT COMMENT 'auto increment sequence',
    `bktx_txid` VARCHAR(256) NOT NULL COMMENT 'block transaction txid',
    `bktx_blockhash` VARCHAR(256) DEFAULT NULL COMMENT 'block transaction belong to blockhash',
    `bktx_prevtxid` VARCHAR(256) DEFAULT NULL COMMENT 'block transaction prev txid',
	 `bktx_prevvoutn` INT(1) DEFAULT 0 COMMENT 'block transaction prev vout position',
    `bktx_vinstasm` VARCHAR(256) DEFAULT NULL COMMENT 'block transaction vin script asm',
    `bktx_vinsthex` VARCHAR(256) DEFAULT NULL COMMENT 'block transaction vin script hex',
	 `bktx_vinsequence` INT(10) DEFAULT 0 COMMENT 'block transaction vin sequence',
	 `bktx_vinvalue` DOUBLE(32,8) DEFAULT 0 COMMENT 'block transaction vin amount',
    `bktx_vinaddresses` VARCHAR(256) DEFAULT NULL COMMENT 'block transaction vin address',
    `bktx_type` CHAR(1) NOT NULL DEFAULT 'T' COMMENT 'block transaction type, pow|pos|trans',
    `create_time` TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT 'table record create time',
    `modify_time` TIMESTAMP DEFAULT '0000-00-00 00:00:00' COMMENT 'table record modify time',
    PRIMARY KEY (`seq_id`),
	 INDEX blocktx_vin_index (`bktx_txid`),
	 INDEX blocktx_vin_index_vin_addressamount (`bktx_prevtxid`, `bktx_prevvoutn`),
	 INDEX blocktx_vin_index_address (`bktx_vinaddresses`)
);

