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

const double EPS = 1e-2;
const long int MAX_LONG_INT = std::numeric_limits<long int>::max();
const double MAX_DOUBLE = std::numeric_limits<double>::max();

int get_int_random_between(int xlower, int xupper);

double get_random_0_1();
bool is_equal(double xa, double xb);
bool is_A_less_than_B(double A, double B);
std::list<string> get_instance_files_list(string instances_dir_path);
void export_json_DOM_file(rapidjson::Document &xjson, string &xfile_path);
rapidjson::Document string_to_json(string json_string);
rapidjson::Document read_json_string_DOM(string xfile_path);
rapidjson::Document read_json_string_DOM_API(string xfile_path);
string json_to_string(rapidjson::Document &xjson);


#endif // GENERAL_FUNCTIONS_H
