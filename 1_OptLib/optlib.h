#ifndef OPTLIB_H
#define OPTLIB_H

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


#include "inputdata.h"
#include "jsonparser.h"

using namespace std;
using namespace rapidjson;
class OptLib
{
public:
    OptLib();

    static Document optimize(Document &json_input);

};

#endif // OPTLIB_H
