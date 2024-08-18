#include "include/converter.h"
#include "include/inverted_index.h"
#include "include/search_server.h"
#include <filesystem>
#include <iostream>

using namespace std;

int main() {
    std::filesystem::path p = __FILE__;
    p = p.parent_path();
    
    ConverterJSON c(p);
    if(c._config_file_open()) {
        InvertedIndex i;
        i.UpdateDocumentBase(c.GetTextDocuments());
        SearchServer s(i);
        s.MaxResponses(c.GetResponsesLimit());
        c.putAnswers(s.search(c.GetRequests()));
    }

    return 0;
}
