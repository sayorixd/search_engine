#include "converter.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <vector>
#include "json-develop/single_include/nlohmann/json.hpp"

ConverterJSON::ConverterJSON(std::filesystem::path _project_path) : project_path(_project_path)
{
    std::filesystem::path p = _project_path;
    std::ifstream file(p.append("config.json"));
    try
    {
        if (!file.is_open())
        {
            throw ConfigIsMissingException();
        }
        if (file.peek() == file.eof())
        {
            throw ConfigIsEmptyException();
        }
        file >> config_json;
        if (config_json["config"].empty())
        {
            throw ConfigIsEmptyException();
        }
        config_file_open = true;

        name = config_json["config"]["name"];
        max_responses = config_json["config"]["max_responses"];
        for (auto &element : config_json["files"])
        {
            document_count++;
            std::string document_path = element;
            p = document_path;
            if(p.is_relative()) {
                p = _project_path;
                p.append(document_path);
            }
            std::ifstream in(p);

            try
            {
                if(!in.is_open()) {
                    throw DocMissingException();
                }
                std::stringstream buffer;
                buffer << in.rdbuf();
                docs.push_back(buffer.str());
            }
            catch (const DocMissingException &e)
            {
                std::cout << e.what() << std::endl;
                std::cout << p << std::endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        config_file_open = false;
    }
    file.close();
}

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    return docs;
}

int ConverterJSON::GetResponsesLimit()
{
    return max_responses;
}

std::vector<std::string> ConverterJSON::GetRequests()
{
    if (requests.empty())
    {
        std::filesystem::path p = project_path;
        std::ifstream file(p.append("requests.json"));
        try
        {
            if (file.is_open())
            {
                file >> requests_json;
                for (auto &r : requests_json["requests"])
                {
                    requests.push_back(r);
                }
            }
            else
            {
                throw RequestsMissingException();
            }
        }
        catch (const RequestsMissingException &e)
        {
            std::cout << e.what() << std::endl;
        }
        file.close();
    }
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers)
{
    std::filesystem::path p = project_path;
    std::ofstream out(p.append("answers.json"));
    try
    {
        if (!out.is_open())
        {
            throw AnswersFileNotOpenException();
        }
        else
        {
            int n = 1;
            for (auto response : answers)
            {
                std::string response_name;
                if (n < 10)
                    response_name = "request00" + std::to_string(n);
                else if (n < 100)
                    response_name = "request0" + std::to_string(n);
                else if (n < 1000)
                    response_name = "request" + std::to_string(n);

                bool result;
                if (response.empty())
                    result = false;
                else
                    result = true;
                answers_json["answers"][response_name]["result"] = result;

                if (result)
                {
                    if (response.size() > 1)
                    {
                        int i = 0;
                        for (auto t : response)
                        {
                            if (i < max_responses)
                            {
                                i++;
                                nlohmann::json j_map = R"(
                        {
                            "docid": 0,
                            "rank": 0.001
                        }
                    )"_json;
                                j_map["docid"] = t.doc_id;
                                j_map["rank"] = t.rank;
                                answers_json["answers"][response_name]["relevance"].push_back(j_map);
                            }
                            else
                                break;
                        }
                    }
                    else
                    {
                        answers_json["answers"][response_name]["docid"] = response[0].doc_id;
                        answers_json["answers"][response_name]["rank"] = response[0].rank;
                    }
                }
                n++;
            }

            out << answers_json;
            std::cout << "Search complete!" << std::endl;
        }
    }
    catch (const AnswersFileNotOpenException &e)
    {
        std::cout << e.what() << std::endl;
    }
}

int ConverterJSON::GetDocumentCount()
{
    return document_count;
}

bool ConverterJSON::_config_file_open() {
    return config_file_open;
}
