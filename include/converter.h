#pragma once
#include <filesystem>
#include <exception>
#include <vector>
#include "json-develop/single_include/nlohmann/json.hpp"

struct RelativeIndex {
    size_t doc_id;
    float rank;
    
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
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

class AnswersFileNotOpenException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Unable to open the answers.json file.";
    }
};

class ConverterJSON {
private:
    nlohmann::json config_json, requests_json, answers_json;
    std::string name;
    int max_responses;
    std::vector<std::string> docs, requests, answers;
    int document_count = 0;
    bool config_file_open = false;
    std::filesystem::path project_path;
public:
    ConverterJSON(std::filesystem::path _project_path);

    std::vector<std::string> GetTextDocuments();

    int GetResponsesLimit();

    std::vector<std::string> GetRequests();

    void putAnswers(std::vector<std::vector<RelativeIndex>> answers);

    int GetDocumentCount();

    bool _config_file_open();
};
