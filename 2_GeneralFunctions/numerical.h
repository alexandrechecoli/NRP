#ifndef NUMERICAL_H
#define NUMERICAL_H

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
#include <vector>


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
vector<double> get_rel_freq_vector(vector<int> &v);
bool is_equal(double xa, double xb);
bool is_A_less_than_B(double A, double B);
double sum_vector_elements(vector<int> &v);
double sum_vector_elements(vector<double> &v);
int select_index_from_freq_vector(vector<double> &v_freq_vector);


#endif // NUMERICAL_H















