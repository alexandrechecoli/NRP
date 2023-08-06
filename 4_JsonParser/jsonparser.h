#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "inputdata.h"
#include "document.h"
#include <cmath>

using namespace rapidjson;
using namespace std;
class JsonParser
{
public:
    JsonParser();

    void parse_json_input(InputData &data_input, rapidjson::Document &json_input);
    Document parse_json_output(InputData &data_input, rapidjson::Document &json_output);
};

#endif // JSONPARSER_H
