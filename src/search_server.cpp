#include "search_server.h"
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>

void sort_vector(std::vector<RelativeIndex> &_v) {
    if(!_v.empty()) {
        for (int i = 0; i < _v.size() - 1; i++) {
            for (int j = 0; j < _v.size() - i - 1; j++) {
                if (_v[j].rank < _v[j + 1].rank) {
                    auto t = _v[j];
                    _v[j] = _v[j + 1];
                    _v[j + 1] = t;
                }
            }
        }
    }
}


SearchServer::SearchServer(InvertedIndex &idx) : _index(idx) {};

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> answers(queries_input.size());
    std::mutex mtx;
    std::vector<std::thread> threads;

    for(int n=0; n<answers.size(); n++) {
        std::thread thread([this, queries_input, n, &answers, &mtx]() {
            auto query = queries_input[n];
            std::vector<size_t> absolute_relevance(_index.GetDocumentCount());
            for(auto &i : absolute_relevance) i = 0;

            std::string word;
            std::istringstream iss(query);
            while(std::getline(iss, word, ' ')) {
                auto word_count = _index.GetWordCount(word);
                for(auto entry : word_count) {
                    absolute_relevance[entry.doc_id] += entry.count;
                }
            }

            size_t max_relevance = absolute_relevance[0];
            for(auto relevance : absolute_relevance) {
                if(relevance > max_relevance) max_relevance = relevance;
            }

            std::vector<RelativeIndex> answer;
            for(size_t i=0; i<absolute_relevance.size(); i++)  {
                float relative_relevance = float(absolute_relevance[i]) / float(max_relevance);
                if(relative_relevance > 0)
                    answer.push_back({i, relative_relevance});
            }
            sort_vector(answer);
            if(answer.size() > max_responses)
                answer.resize(max_responses);
            
            mtx.lock();
            answers[n] = answer;
            mtx.unlock();
        });
        threads.push_back(std::move(thread));
    }

    for(auto &th : threads) {
        if(th.joinable()) 
            th.join();
    }

    return answers;
}

void SearchServer::MaxResponses(int _max_responses) {
    max_responses = _max_responses;
}
