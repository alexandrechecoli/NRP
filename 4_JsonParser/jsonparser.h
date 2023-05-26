#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "inputdata.h"
#include "document.h"

class JsonParser
{
public:
    JsonParser();

    void parse_json_input(InputData &data_input, rapidjson::Document &json_input);
};

#endif // JSONPARSER_H
