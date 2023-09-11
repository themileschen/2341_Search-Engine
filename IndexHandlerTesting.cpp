#include <iostream>
#include <string>
#include "IndexHandler.h"
#include "DocumentParser.h"

int main()
{
    // IndexHandler index_handler;

    // index_handler.update_words_tree("word1", "1234"); // a
    // index_handler.update_words_tree("word2", "4321"); // a
    // index_handler.update_words_tree("word1", "5678"); // b
    // index_handler.update_words_tree("word1", "1234"); // c 

    // index_handler.printInfo();



    IndexHandler ih;
    DocumentParser dp;
    
    dp.findDocumentsTest(ih, "sample_data/coll_1/news_0064567.json");
    ih.printInfo();
    


    return 0;
}