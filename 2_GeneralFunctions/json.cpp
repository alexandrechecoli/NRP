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
