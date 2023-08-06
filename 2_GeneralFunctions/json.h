#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H
#include <time.h>
#include <ctime>
#include <algorithm>
#include <math.h>
#include <limits>
#include <list>
#include <string>
#include <dirent.h>
#include <rapidjson.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <sstream>

#include "document.h"        //rapidjson
#include "writer.h"          //rapidjson
#include "stringbuffer.h"    //rapidjson
#include "filereadstream.h"  //rapidjson
#include "error/error.h"     //rapidjson
#include "error/en.h"        //rapidjson


using namespace std;

void export_json_DOM_file(rapidjson::Document &xjson, string &xfile_path);
std::list<string> get_instance_files_list(string instances_dir_path);
string json_to_string(rapidjson::Document &xjson);
rapidjson::Document string_to_json(string json_string);
rapidjson::Document read_json_string_DOM(string xfile_path);
rapidjson::Document read_json_string_DOM_API(string xfile_path);
void parse_json_to_csv(rapidjson::Document &xjson, string path);


#endif // GENERAL_FUNCTIONS_H
