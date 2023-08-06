#include "optlib.h"

OptLib::OptLib()
{

}

Document OptLib::optimize(Document &json_input)
{

    Document output;

    InputData input;
    JsonParser parser;
    parser.parse_json_input(input,json_input);

    // Otimiza o problema
    NRP nrp(&input);
    nrp.optimize();


    //output.SetObject();
    //output.CopyFrom(json_input,json_input.GetAllocator());
    output = parser.parse_json_output(input, json_input);


    return output;
}


