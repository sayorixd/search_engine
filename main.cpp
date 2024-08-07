#include "include/converter.h"
#include "include/inverted_index.h"
#include "include/search_server.h"


int main() {
    ConverterJSON c;
    InvertedIndex i;

    // компилятор выдает ошибку, потому что он не может найти определения функций, хотя они находятся в src
    i.UpdateDocumentBase(c.GetTextDocuments());
    i.DocumentCount(c.GetDocumentCount());
    SearchServer s(i);
    c.putAnswers(s.search(c.GetRequests()));
    return 0;
}
