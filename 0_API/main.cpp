#include <iostream>
#include <string>
#include <deque>
#include <string>
#include <algorithm>

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"



#include "json.h"
#include "optlib.h"


using namespace std;
using namespace rapidjson;
using namespace std;

int main(int argc, char *argv[])
{
    //std::string file_path = argv[1];
    //std::string file_path = "C:\\Users\\Usuario\\Documents\\Alexandre\\C++\\NurseRostering\\1-API\\build-API-Desktop_Qt_5_15_2_MinGW_64_bit-Debug\\debug\\input_9_clientes.json";
    std::string file_path = "C:\\Users\\Usuario\\Documents\\Alexandre\\C++\\NurseRostering\\1-API\\build-API-Desktop_Qt_5_15_2_MinGW_64_bit-Debug\\debug\\instance_sta_casa.json";
    rapidjson::Document input_json = read_json_string_DOM_API(file_path);

    Document output_json;
    output_json = OptLib::optimize(input_json);


    string exp_path_file = "C:\\Users\\Usuario\\Documents\\Alexandre\\C++\\NurseRostering\\1-API\\build-API-Desktop_Qt_5_15_2_MinGW_64_bit-Debug\\debug\\out.json";
    export_json_DOM_file(output_json, exp_path_file);
    cout << "###### FIM ########";
    return 0;
}
