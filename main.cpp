#include "include/converter.h"
#include "include/inverted_index.h"
#include "include/search_server.h"
#include <iostream>

int main() {
    ConverterJSON c;
    InvertedIndex i;
    i.UpdateDocumentBase(c.GetTextDocuments());
    i.DocumentCount(c.GetDocumentCount());
    SearchServer s(i);
    c.putAnswers(s.search(c.GetRequests()));
    
    return 0;
}
