#include "json.h"

std::list<string> get_instance_files_list(string instances_dir_path)
{

    /*
     * Get all file names on a defined folder and returns it
     * on a list of strings
     */

    int count = 0;
    std::list<std::string> l_instance_files;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (instances_dir_path.c_str())) != nullptr)
    {
      while ((ent = readdir (dir)) != nullptr)
      {
             l_instance_files.push_back(ent->d_name);
             count++;
      }
      closedir (dir);
    } else
    {
      perror ("");
    }
    l_instance_files.sort();
    l_instance_files.erase(l_instance_files.begin());
    l_instance_files.erase(l_instance_files.begin());
    return  l_instance_files;
}

void export_json_DOM_file(rapidjson::Document &xjson, string &xfile_path)
{
    /*
     * Print a DOM Document in a text file
     */

    rapidjson::StringBuffer buffer2;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer2);
    xjson.Accept(writer);
    std::ofstream output(xfile_path);
    output << buffer2.GetString();
}

rapidjson::Document read_json_string_DOM(string xfile_path)
{

    /*
     * Read a json file, parse it, and return it as a DOM document
     */


    //On Linux (or other os), change the 'rb' to 'r'
    FILE* pFile = fopen(xfile_path.c_str(), "rb");
    char buffer[65536];
    rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));


    rapidjson::Document document;
    document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
    fclose(pFile);
    // check validity of the document
    if(document.IsNull())
    {
        std::cout << "Invalid JSON...\n";
    }

    return document;
}

rapidjson::Document read_json_string_DOM_API(string xfile_path)
{

    /*
     * Read a json file, parse it, and return it as a DOM document.
     * If the file is invalid (not a json), adds an error message on
     * the json
     */


    //On Linux (or other os), change the 'rb' to 'r'
    FILE* pFile = fopen(xfile_path.c_str(), "rb");
    char buffer[65536];
    rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));


    rapidjson::Document document;
    document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
    fclose(pFile);
    // check validity of the document
    rapidjson::Value output_meta_info;
    output_meta_info.SetObject();
    rapidjson::Value error;
    error.SetObject();
    if(document.IsNull())
    {
        document.SetObject();
        error.AddMember("error", "invalid_file", document.GetAllocator());
        error.AddMember("message", "file is not in json format", document.GetAllocator());
        output_meta_info.AddMember("meta_info", error, document.GetAllocator());
        document.AddMember("output", output_meta_info, document.GetAllocator());
    }

    return document;
}

rapidjson::Document string_to_json(string json_string)
{
    const char* json_text = json_string.c_str();
    rapidjson::Document document;
    document.Parse(json_text);
    return document;
}

string json_to_string(rapidjson::Document &xjson)
{

    /*
     * Serialize a json into a string
     */

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    xjson.Accept(writer);
    std::string s(buffer.GetString(), buffer.GetSize());
    return s;
}

void parse_json_to_csv(rapidjson::Document &xjson, string path)
{


    ofstream o;
    o.open(path.c_str());
    o << ";";
    for(size_t i = 0; i < xjson["output"]["M_morning"][0].Size();i++)
    {
         o << i << ";";
    }
    o <<";Turno;Folgas;Folgas FDS;Turnos extras";
    o << "\n";

     //output.AddMember("v_shift_assigned_to_nurse",v_shift_assigned_to_nurse,root_output_DOM.GetAllocator());
     //output.AddMember("v_nurse_sum_rest_days",v_nurse_sum_rest_days,root_output_DOM.GetAllocator());
     //output.AddMember("v_nurse_sum_rest_days_weekends",v_nurse_sum_rest_days_weekends,root_output_DOM.GetAllocator());
     //output.AddMember("v_nurse_extra_shifts",v_nurse_extra_shifts,root_output_DOM.GetAllocator());

    for(size_t i = 0; i < xjson["output"]["M_morning"].Size();i++)
    {
         o << i << ";";
        for(size_t j = 0; j < xjson["output"]["M_morning"][i].Size();j++)
        {
            o << xjson["output"]["M_morning"][i][j].GetInt() << ";";
        }
        o << ";" << xjson["output"]["v_shift_assigned_to_nurse"][i].GetInt() << ";"
                 << xjson["output"]["v_nurse_sum_rest_days"][i].GetInt() << ";"
                 << xjson["output"]["v_nurse_sum_rest_days_weekends"][i].GetInt() << ";"
                 << xjson["output"]["v_nurse_extra_shifts"][i].GetInt();
        o << "\n";
    }

    // Pulando uma linha
    for(size_t j = 0; j < xjson["output"]["M_morning"][0].Size();j++) o << ";";
    o << "\n";

    // Morning minimum
    o << ";";
    for(size_t j = 0; j < xjson["input"]["data"]["shift_coverage"]["mor_min"].Size();j++)
    {
       o << xjson["input"]["data"]["shift_coverage"]["mor_min"][j].GetInt() << ";";
    }
     o << "\n";
     o << ";";
    for(size_t j = 0; j < xjson["output"]["v_mor_diff"].Size();j++)
    {
       o << xjson["output"]["v_mor_diff"][j].GetInt() << ";";
    }
    // Break line
    o << "\n";
    // Empty line
    for(size_t j = 0; j < xjson["output"]["M_morning"][0].Size();j++) o << ";";
    o << "\n";


    o << ";";
    for(size_t i = 0; i < xjson["output"]["M_morning"][0].Size();i++)
    {
         o << i << ";";
    }
     o << "\n";
    for(size_t i = 0; i < xjson["output"]["M_morning"].Size();i++)
    {
        o << i << ";";
        for(size_t j = 0; j < xjson["output"]["M_afternoon"][i].Size();j++)
        {
            o << xjson["output"]["M_afternoon"][i][j].GetInt() << ";";
        }
        o << "\n";
    }

    // Pulando uma linha
    for(size_t j = 0; j < xjson["output"]["M_morning"][0].Size();j++) o << ";";
    o << "\n";

    // Morning minimum
    o << ";";
    for(size_t j = 0; j < xjson["input"]["data"]["shift_coverage"]["aft_min"].Size();j++)
    {
       o << xjson["input"]["data"]["shift_coverage"]["aft_min"][j].GetInt() << ";";
    }
     o << "\n";
     o << ";";
    for(size_t j = 0; j < xjson["output"]["v_aft_diff"].Size();j++)
    {
       o << xjson["output"]["v_aft_diff"][j].GetInt() << ";";
    }

    // Dados da função objetivo
    o << "\n";
    o << "CUSTOS\n";
    o << "Soma diferencas manha;" << xjson["output"]["solution_cost"]["sum_mor_diff"].GetInt() << "\n";
    o << "Soma diferencas tarde;" << xjson["output"]["solution_cost"]["sum_aft_diff"].GetInt() << "\n";
    o << "Soma turnos extras;" << xjson["output"]["solution_cost"]["sum_extra_shifts"].GetInt() << "\n";
    o << "Balancemanto de turnos extras;" << xjson["output"]["solution_cost"]["extra_shift_load_balance"].GetInt() << "\n";
    o << "Violacoes folga fora de FDS;" << xjson["output"]["solution_cost"]["weekend_rest_violation"].GetInt() << "\n";
    o << "Violacoes folgas desejadas;" << xjson["output"]["solution_cost"]["prefered_rest_violation"].GetInt() << "\n";
    o << "Custo;" << xjson["output"]["solution_cost"]["cost"].GetInt() << "\n";
    o.close();


}












