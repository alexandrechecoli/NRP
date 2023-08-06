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

    // FRAC DAYS OFF (also weekend) -------------------
    data_input.frac_days_off             = json_input["input"]["data"]["frac_days_off"].GetDouble();
    data_input.frac_days_off_weekend     = json_input["input"]["data"]["frac_days_off_weekend"].GetDouble();


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

        //N_DAYSOFF/ N_WEEKEND_DAYS_OFF (round up or down?)



        data_input.v_nurses[i].n_days_off = round((data_input.n_days - data_input.v_nurses[i].v_vacation_days.size()) * data_input.frac_days_off);
        data_input.v_nurses[i].n_days_off_weekend = round((data_input.n_days - data_input.v_nurses[i].v_vacation_days.size()) * data_input.frac_days_off_weekend);


        data_input.v_nurses[i].id = json_input["input"]["data"]["nurses"][i]["id"].GetString();
    }




}

rapidjson::Document JsonParser::parse_json_output(InputData &data_input, rapidjson::Document &json_output)
{

    /*
     * Parses the dt output to a json
     */

    Document root_output_DOM;
    root_output_DOM.SetObject();

    root_output_DOM.CopyFrom(json_output, root_output_DOM.GetAllocator());

    Value output;
    output.SetObject();


    Value vv_morning_assign(kArrayType);
    for(size_t i = 0; i < data_input.vv_mor_shift.size();i++)
    {
        Value v_morning_assign(kArrayType);
        for(size_t j = 0; j < data_input.vv_mor_shift[i].size();j++ )
        {
            v_morning_assign.PushBack(data_input.vv_mor_shift[i][j], root_output_DOM.GetAllocator());
        }
        vv_morning_assign.PushBack(v_morning_assign,root_output_DOM.GetAllocator());
    }
    output.AddMember("M_morning",vv_morning_assign,root_output_DOM.GetAllocator());

    Value vv_aft_assign(kArrayType);
    for(size_t i = 0; i < data_input.vv_aft_shift.size();i++)
    {
        Value v_aft_assign(kArrayType);
        for(size_t j = 0; j < data_input.vv_aft_shift[i].size();j++ )
        {
            v_aft_assign.PushBack(data_input.vv_aft_shift[i][j], root_output_DOM.GetAllocator());
        }
        vv_aft_assign.PushBack(v_aft_assign,root_output_DOM.GetAllocator());
    }
    output.AddMember("M_afternoon",vv_aft_assign,root_output_DOM.GetAllocator());


    // DEBUG ----------------------------

    Value v_morning_diff(kArrayType);
    for(size_t j = 0; j < data_input.v_mor_diff.size();j++ )
    {
        v_morning_diff.PushBack(data_input.v_mor_diff[j], root_output_DOM.GetAllocator());
    }
     output.AddMember("v_mor_diff",v_morning_diff,root_output_DOM.GetAllocator());

     Value v_afternoon_diff(kArrayType);
     for(size_t j = 0; j < data_input.v_aft_diff.size();j++ )
     {
         v_afternoon_diff.PushBack(data_input.v_aft_diff[j], root_output_DOM.GetAllocator());
     }
      output.AddMember("v_aft_diff",v_afternoon_diff,root_output_DOM.GetAllocator());


      // Nurses vectors
      //dt->v_shift_assigned_to_nurse      = s.v_shift_assigned_to_nurse;
      //dt->v_nurse_sum_rest_days          = s.v_nurse_sum_rest_days;
      //dt->v_nurse_sum_rest_days_weekends = s.v_nurse_sum_rest_days_weekends;
      //dt->v_nurse_extra_shifts           = s.v_nurse_extra_shifts;

      Value v_shift_assigned_to_nurse(kArrayType);
      Value v_nurse_sum_rest_days(kArrayType);
      Value v_nurse_sum_rest_days_weekends(kArrayType);
      Value v_nurse_extra_shifts(kArrayType);

      for(size_t j = 0; j < data_input.v_nurse_extra_shifts.size();j++ )
      {
          v_shift_assigned_to_nurse.PushBack(data_input.v_shift_assigned_to_nurse[j], root_output_DOM.GetAllocator());
          v_nurse_sum_rest_days.PushBack(data_input.v_nurse_sum_rest_days[j], root_output_DOM.GetAllocator());
          v_nurse_sum_rest_days_weekends.PushBack(data_input.v_nurse_sum_rest_days_weekends[j],root_output_DOM.GetAllocator());
          v_nurse_extra_shifts.PushBack(data_input.v_nurse_extra_shifts[j], root_output_DOM.GetAllocator());
      }
      output.AddMember("v_shift_assigned_to_nurse",v_shift_assigned_to_nurse,root_output_DOM.GetAllocator());
      output.AddMember("v_nurse_sum_rest_days",v_nurse_sum_rest_days,root_output_DOM.GetAllocator());
      output.AddMember("v_nurse_sum_rest_days_weekends",v_nurse_sum_rest_days_weekends,root_output_DOM.GetAllocator());
      output.AddMember("v_nurse_extra_shifts",v_nurse_extra_shifts,root_output_DOM.GetAllocator());


    // Objective function cost components

      Value solution_cost;
      solution_cost.SetObject();

      solution_cost.AddMember("sum_mor_diff",data_input.sum_mor_diff,root_output_DOM.GetAllocator());
      solution_cost.AddMember("sum_aft_diff",data_input.sum_aft_diff,root_output_DOM.GetAllocator());
      solution_cost.AddMember("sum_extra_shifts",data_input.sum_extra_shifts,root_output_DOM.GetAllocator());
      solution_cost.AddMember("extra_shift_load_balance",data_input.extra_shift_load_balance,root_output_DOM.GetAllocator());
      solution_cost.AddMember("weekend_rest_violation",data_input.weekend_rest_violation,root_output_DOM.GetAllocator());
      solution_cost.AddMember("prefered_rest_violation",data_input.prefered_rest_violation,root_output_DOM.GetAllocator());
      solution_cost.AddMember("cost",data_input.cost,root_output_DOM.GetAllocator());

      output.AddMember("solution_cost", solution_cost,root_output_DOM.GetAllocator());
    //-----------------------------------





    root_output_DOM.AddMember("output", output,root_output_DOM.GetAllocator());


    return root_output_DOM;
}
