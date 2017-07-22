
#include "config-in.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time.hpp>

using namespace boost::property_tree;
using namespace boost::gregorian;
using namespace boost;

const std::string file_path="./fidinventor.conf";

map<string, string> mapArgs;

void generate_defaultfidconfig()
{
	ptree root;
	ptree items;

	ptree item_mysql;
	item_mysql.put("database",	"fidchainblock");
	item_mysql.put("dbname",	"fidchain");
	item_mysql.put("dbpasswd",	"123123");
	item_mysql.put("dburl",		"127.0.0.1");
	item_mysql.put("dbport",	"3306");
	items.push_back(std::make_pair("mysql", item_mysql));
	//get_fidconfig("database"), get_fidconfig("dbname"), get_fidconfig("dbpasswd"), get_fidconfig("dburl")

	ptree item_rpc;
	item_rpc.put("rpcuser",		"fidchainrpc20");
	item_rpc.put("rpcpasswd",	"DSBs3pKSdpiTAPDms");
	item_rpc.put("rpcconnect",	"139.224.13.20");
	items.push_back(std::make_pair("rpc", item_rpc));

	root.put_child("fid", items);

	boost::property_tree::write_json(file_path, root);

	cout << "generate default fid config at >" << file_path << endl;
}

int parse_fidconfig(string path)
{
	ptree root;
	ptree items;
		
	boost::property_tree::read_json<boost::property_tree::ptree>("" != path ? path : file_path, root);

	items=root.get_child("fid");
	for(ptree::iterator it = items.begin(); it != items.end(); it++) {
		
		string key = it->first;
		cout << "==============parse_fidconfig() - " << key << endl;
		if(key == "mysql") {
		mapArgs.insert(std::make_pair<std::string, std::string>("-database", 	"" != it->second.get<string>("database") 	? it->second.get<string>("database") : "fidchain")); 
		mapArgs.insert(std::make_pair<std::string, std::string>("-dbname", 	"" != it->second.get<string>("dbname") 		? it->second.get<string>("dbname") : "fidchain")); 
		mapArgs.insert(std::make_pair<std::string, std::string>("-dbpasswd", 	"" != it->second.get<string>("dbpasswd") 	? it->second.get<string>("dbpasswd") : "123123")); 
		mapArgs.insert(std::make_pair<std::string, std::string>("-dburl", 	"" != it->second.get<string>("dburl") 		? it->second.get<string>("dburl") : "127.0.0.1")); 
		mapArgs.insert(std::make_pair<std::string, std::string>("-dbport", 	"" != it->second.get<string>("dbport") 		? it->second.get<string>("dbport") : "3306")); 

		cout << "==============parse_fidconfig() - " << mapArgs["database"] << endl;
		} else if(key == "rpc") {
		mapArgs.insert(std::make_pair<std::string, std::string>("-rpcuser", 	"" != it->second.get<string>("rpcuser")		? it->second.get<string>("rpcuser") : "fidchainrpc20")); 
        	mapArgs.insert(std::make_pair<std::string, std::string>("-rpcpassword", "" != it->second.get<string>("rpcpasswd")	? it->second.get<string>("rpcpasswd") : "DSBs3pKSdpiTAPDms"));
        	mapArgs.insert(std::make_pair<std::string, std::string>("-rpcconnect", 	"" != it->second.get<string>("rpcconnect")	? it->second.get<string>("rpcconnect") : "139.224.13.20"));
		cout << "==============parse_fidconfig() - " << mapArgs["rpcuser"] << endl;
		}
	}
	return 0 == items.size() ? -1 : items.size();
}

string get_fidconfig(const string& key)
{
	
	map<std::string, std::string>::iterator it = mapArgs.find(key);
	if(it != mapArgs.end())
		return (*it).second;
	
	{
		cerr << "===========get_fidconfig() " << key << " err, not default." << endl;
		return "";
	}
}

static string json_to_string(boost::property_tree::ptree item)
{
	std::stringstream is;
	boost::property_tree::write_json(is, item);
	return is.str();
}

static boost::property_tree::ptree  string_to_json(string c)
{
	std::istringstream iss; 
	iss.str(c);
	boost::property_tree::ptree item;
	boost::property_tree::json_parser::read_json(iss, item);
	// int n = item.get<int>("a");
	return item;
}
