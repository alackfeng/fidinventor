#ifndef FIDINVENTOR_CONFIG_IN_H
#define FIDINVENTOR_CONFIG_IN_H


#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

// json be related
#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_utils.h"
#include "json/json_spirit_writer_template.h"

using namespace std;


typedef int64_t 	fidtime; 	// Fid time
typedef uint64_t 	fidint; 	// Fid long long


extern map<string, string> mapArgs;
void generate_defaultfidconfig();
int parse_fidconfig(string path);
string get_fidconfig(const string& key);

#endif
