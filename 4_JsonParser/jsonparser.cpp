#include "jsonparser.h"

JsonParser::JsonParser()
{

}

void JsonParser::parse_json_input(InputData &data_input, rapidjson::Document &json_input)
{

    // Parses from 'json_input' saving the data into 'data_input'

    data_input.n_days = json_input["input"]["data"]["n_days"].GetInt();

    int n_weekends = json_input["input"]["data"]["weekend"].Size();
    data_input.v_weekend = vector<int>(n_weekends);
    for(int i = 0; i < n_weekends; i++)
    {
        data_input.v_weekend[i] = json_input["input"]["data"]["weekend"][i].GetInt();
    }

    // FRAC DAYS OFF -------------------
    data_input.frac_days_off             = json_input["input"]["data"]["frac_days_off"].GetDouble();

    // MAX CONSECUTIVE DAYS OFF --------
    data_input.max_consecutive_work_days = json_input["input"]["data"]["max_consecutive_work_days"].GetInt();

    data_input.v_mor_min = vector<int>(data_input.n_days);
    data_input.v_mor_max = vector<int>(data_input.n_days);
    data_input.v_aft_min = vector<int>(data_input.n_days);
    data_input.v_aft_max = vector<int>(data_input.n_days);

    // SHIFT COVERAGE -------------------
    for(int i = 0; i < data_input.n_days; i++)
    {
        data_input.v_mor_min[i] = json_input["input"]["data"]["shift_coverage"]["mor_min"][i].GetInt();
        data_input.v_mor_max[i] = json_input["input"]["data"]["shift_coverage"]["mor_max"][i].GetInt();
        data_input.v_aft_min[i] = json_input["input"]["data"]["shift_coverage"]["aft_min"][i].GetInt();
        data_input.v_aft_max[i] = json_input["input"]["data"]["shift_coverage"]["aft_max"][i].GetInt();
    }


    // NURSES ---------------------------
    int n_nurses = json_input["input"]["data"]["nurses"].Size();
    data_input.v_nurses = vector<InputData::nurse>(n_nurses);

    for(int i = 0; i < n_nurses; i++)
    {
        // PREFERRED DAYS OFF ------
        int n_preferred_days_off = json_input["input"]["data"]["nurses"][i]["preferred_days_off"].Size();
        data_input.v_nurses[i].v_preferred_days_off = vector<int>(n_preferred_days_off);
        for(int j = 0; j < n_preferred_days_off; j++)
        {
           data_input.v_nurses[i].v_preferred_days_off[j] = json_input["input"]["data"]["nurses"][i]["preferred_days_off"][j].GetInt();
        }
        // VACATION DAYS -----------
        int n_vacation_days = json_input["input"]["data"]["nurses"][i]["vacation_days"].Size();
        data_input.v_nurses[i].v_vacation_days = vector<int>(n_vacation_days);
        for(int j = 0; j < n_vacation_days; j++)
        {
           data_input.v_nurses[i].v_vacation_days[j] = json_input["input"]["data"]["nurses"][i]["vacation_days"][j].GetInt();
        }
        //
        data_input.v_nurses[i].id = json_input["input"]["data"]["nurses"][i]["id"].GetString();
    }


}
