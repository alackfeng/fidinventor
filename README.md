# fidinventor
the project use to get bitcoin info according to rpc comment


## 1. 安装mysql服务及mysql++库
sudo apt-get install mysql-server mysql-client libmysqlclient-dev
sudo apt-get install libmysqlclient-dev libmysqld-dev libmysql++-dev libmysql++-doc libmysql++3v5
##ubuntu14.04 sudo apt-get install libmysqlclient-dev libmysqld-dev libmysql++-dev libmysql++-doc libmysql++3
locate libmysqlpp.so

## 配置mysql
mysql -uroot -p
create database fidchainblock;
use fidchain;
grant all privileges on fidchainblock.* to fidchain@'%' identified by '123123' WITH GRANT OPTION;
grant all privileges on *.* to fidchain@'%' identified by '123123' WITH GRANT OPTION;
flush privileges;
select host,user,password from mysql.user;
mysqlimport -u root -p'' < fidinventor.sql
service mysql restart
mysql -h 139.224.13.20 -ufidchain -p'123123'

## 2. 下载源码编译安装
apt-get update && apt-get install libdb-dev libdb++-dev libboost-all-dev libssl-dev libminiupnpc-dev libzmq3-dev libevent-dev
apt-get install build-essential autotools-dev m4 autoconf2.13 autoconf-archive gnu-standards autoconf-doc libtool
cd opt/
git clone https://github.com/alackfeng/fidinventor.git
cd fidinventor

./autogen.sh
./configure --prefix=/opt/fidchain/
make
make install

## 3. 运行程序
#执行./src/fidinventor 生成配置文件./fidinventor.conf，再修改相应选项，run
nohup fidinventor /opt/fidinventor.conf 2>&1 1>/var/log/fidinventor.log &
ps -ef|grep fidinventor
