#pragma once
#include "inverted_index.h"
#include "converter.h"

class SearchServer {
private:
    InvertedIndex _index;

public:
    SearchServer(InvertedIndex &idx);

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queries_input);
};
