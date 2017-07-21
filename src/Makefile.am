
bin_PROGRAMS = fidinventor

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS) $(ERROR_CXXFLAGS) $(PTHREAD_CFLAGS) -I/usr/include/mysql -I/usr/include/mysql++
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)


fidinventor_SOURCES = main.cc \
thread.cpp \
rpcprotocol.cpp \
rpcclient.cpp \
utils.cpp \
transaction.cpp \
taskschedule.cpp \
wrap.cpp \
block.cpp \
mysqldb/mysqldb.cpp \
config.cpp \
thread.h \
rpcprotocol.h \
rpcclient.h \
tinyformat.h \
utils.h \
transaction.h \
wrap.h \
block.h \
mysqldb/mysqldb.h \
taskschedule.h

fidinventor_LDADD = $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) -lboost_system -lboost_thread -lboost_regex -lcrypto -lssl -lpthread -L/usr/lib -lmysqlpp -lmysqlclient -lnsl -lz -lm

fidinventor_CPPFLAGS = $(BOOST_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)
