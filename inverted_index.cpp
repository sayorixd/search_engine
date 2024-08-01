#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

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

    void DocumentCount(int _d) {
        doc_count = _d;
    }

    int GetDocumentCount() {
        return doc_count;
    }

    void UpdateDocumentBase(std::vector<std::string> input_docs) {
        size_t doc_id = 0;
        for(auto sentence : input_docs) {
            std::map<std::string, size_t> word_count;

            std::string word;
            std::istringstream iss(sentence);
            while (std::getline(iss, word, ' ')) {
                if(word_count.find(word) == word_count.end()) {
                    word_count[word] = 1;
                }
                else {
                    word_count[word]++;
                }
            }

            for(auto it=word_count.begin(); it!=word_count.end(); it++) {
                if(freq_dictionary.find(it->first) == freq_dictionary.end()) {
                    freq_dictionary[it->first] = {{doc_id, it->second}};
                }
                else {
                    freq_dictionary[it->first].push_back({doc_id, it->second});
                }
            }
            doc_id++;
        }
    }

    std::vector<Entry> GetWordCount(const std::string& word) {
        return freq_dictionary[word];
    }

};