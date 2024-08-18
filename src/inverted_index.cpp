#include "inverted_index.h"
#include <iostream>
#include <sstream>

void InvertedIndex::DocumentCount(int _d) {
    doc_count = _d;
}

int InvertedIndex::GetDocumentCount() {
    return doc_count;
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
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
    doc_count = input_docs.size();
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    return freq_dictionary[word];
}
