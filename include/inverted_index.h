#pragma once
#include <vector>
#include <map>
#include <string>

struct Entry {
    size_t doc_id, count;
};

class InvertedIndex {
private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    int doc_count;

public:
    InvertedIndex() = default;

    void DocumentCount(int _d) ;

    int GetDocumentCount() ;

    void UpdateDocumentBase(std::vector<std::string> input_docs) ;

    std::vector<Entry> GetWordCount(const std::string& word) ;
};