#include "include/converter.h"
#include "include/inverted_index.h"
#include "include/search_server.h"
#include <filesystem>

int main() {
    std::filesystem::path p = __FILE__;
    p = p.parent_path();
    
    ConverterJSON c(p);
    if(c._config_file_open()) {
        InvertedIndex i;
        i.UpdateDocumentBase(c.GetTextDocuments());
        i.DocumentCount(c.GetDocumentCount());
        SearchServer s(i);
        c.putAnswers(s.search(c.GetRequests()));
    }
    
    return 0;
}
