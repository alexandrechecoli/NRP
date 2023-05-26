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

    output.SetObject();
    output.CopyFrom(json_input,json_input.GetAllocator());

    return output;
}

//cancelamento 2023 041 895 642 58


// OI FIBRA
// 119,90 - 500mb 7 dias de teste 12 meses |
// 109,90 -
