#include <iostream>
#include <fstream>
#include <exception>
#include <vector>
#include "json-develop/single_include/nlohmann/json.hpp"

struct RelativeIndex {
    size_t doc_id;
    float rank;
};

class ConfigIsEmptyException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Config file is empty.";
    }
};

class ConfigIsMissingException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Config file is missing.";
    }
};

class DocMissingException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Document file is missing.";
    }
};

class RequestsMissingException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Requests file is missing.";
    }
};

class ConverterJSON {
private:
    nlohmann::json config_json, requests_json, answers_json;
    std::string name;
    int max_responses;
    std::vector<std::string> docs, requests, answers;
    int document_count = 0;
public:
    ConverterJSON() {
        std::ifstream file("config.json");
        try {
            if(!file.is_open()) {
                throw ConfigIsMissingException();
            }
            if(file.peek() == file.eof()) {
                throw ConfigIsEmptyException();
            }
            file >> config_json;
            if(config_json["config"].empty()) {
                throw ConfigIsEmptyException();
            }
            name = config_json["config"]["name"];
            max_responses = config_json["config"]["max_responses"];
            for(auto &element : config_json["files"]) {
                document_count++;
                std::ifstream in(element);
                try {
                    if(!in.is_open()) {
                        throw DocMissingException();
                    }
                    std::stringstream buffer;
                    buffer << in.rdbuf();
                    docs.push_back(buffer.str());
                }
                catch(const DocMissingException &e) {
                    std::cout << e.what() << std::endl;
                }
            }
        }
        catch(const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
        file.close();
    }

    std::vector<std::string> GetTextDocuments() {
        return docs;
    }

    int GetResponsesLimit() {
        return max_responses;
    }

    std::vector<std::string> GetRequests() {
        if(requests.empty()) {
            std::ifstream file("requests.json");
            try {
                if (file.is_open()) {
                    file >> requests_json;
                    for (auto &r: requests_json["requests"]) {
                        requests.push_back(r);
                    }
                } else {
                    throw RequestsMissingException();
                }
            }
            catch(const RequestsMissingException &e) {
                std::cout << e.what() << std::endl;
            }
            file.close();
        }
        return requests;
    }

    void putAnswers(std::vector<std::vector<RelativeIndex>> answers) {
        std::ofstream out("answers.json");

        int n=1;
        for(auto response : answers) {
            std::string response_name;
            if(n < 10)
                response_name = "request00" + std::to_string(n);
            else if (n < 100)
                response_name = "request0" + std::to_string(n);
            else if(n < 1000)
                response_name = "request" + std::to_string(n);

            bool result;
            if(response.empty()) result = false;
            else result = true;
            answers_json["answers"][response_name]["result"] = result;

            if(result) {
                if(response.size() > 1) {
                    int i=0;
                    for(auto t : response) {
                        if(i < max_responses) {
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
                        else break;
                    }

                }
                else {
                    answers_json["answers"][response_name]["docid"] = response[0].doc_id;
                    answers_json["answers"][response_name]["rank"] = response[0].rank;
                }
            }
            n++;
        }


        out << answers_json;
    }

    int GetDocumentCount() {
        return document_count;
    }
};
