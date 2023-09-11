// Class Owner: Miles 

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include <string>
#include <vector>
using namespace std;

#include "MapValue.h"
#include "AVLTree.h"
#include "IndexHandler.h"
#include "DocumentParser.h"
#include "SearchEngine.h"
#include "QueryEngine.h"

TEST_CASE("Check that AVLTree works", "[AVLTree]")
{   
    AVLTree<MapValue<std::string, std::vector<std::string>>, std::string, std::vector<std::string>> test1;
    vector<string> v = {"id1", "id2"};
    MapValue<std::string, std::vector<std::string>> one("Finance", v);

    // empty tree 
    CHECK(test1.contains(one) == false);
    CHECK(test1.containsKey("Finance") == false);
    CHECK_THROWS(test1.returnValues("Finance"));
    CHECK(test1.isEmpty() == true);
    CHECK_NOTHROW(test1.makeEmpty());
    CHECK_NOTHROW(test1.remove(one));
    CHECK(test1.getSortedTree() == "");
    CHECK(test1.getTreeByLevel() == "");
    CHECK(test1.getPrettyTree() == "");
    CHECK(test1.findMin() == nullptr);
    CHECK(test1.height() == -1);
    CHECK_THROWS(test1.add_value_vector("Finance", "id0"));
    CHECK_THROWS(test1.findKey("Finance"));
    CHECK_THROWS(test1.isBalanced() == true);

    // one node tree 
    test1.insert(one);
    CHECK(test1.contains(one) == true);
    CHECK(test1.containsKey("Finance") == true);
    CHECK(test1.getSortedTree() == "Finance ");
    CHECK(test1.getTreeByLevel() == "Finance ");
    string multiline = 
        "└──Finance"
        "\n";
    CHECK(test1.getPrettyTree() == multiline);
    CHECK(test1.findMin()->key == "Finance");
    test1.insert(one);
    CHECK(test1.height() == 0);
    vector<string> temp = test1.returnValues("Finance");
    CHECK(temp.size() == 2);
    CHECK(temp.at(0) == "id1");
    CHECK(temp.at(1) == "id2");
    test1.add_value_vector("Finance", "id100");
    temp = test1.returnValues("Finance");
    CHECK(temp.at(2) == "id100");
    CHECK(test1.findKey("Finance")->key == "Finance");
    CHECK(test1.isEmpty() == false);
    test1.makeEmpty();
    CHECK(test1.isEmpty() == true);
    test1.insert(one);
    test1.remove(one);
    CHECK(test1.isEmpty() == true);

    // two node tree 
    vector<string> v2 = {"id3"};
    MapValue<std::string, std::vector<std::string>> two("Accounting", v2);
    test1.insert(one);
    test1.insert(two);
    CHECK(test1.contains(two) == true);
    CHECK(test1.containsKey("Accounting") == true);
    CHECK(test1.getSortedTree() == "Accounting Finance ");
    CHECK(test1.getTreeByLevel() == "Finance Accounting ");
    multiline = 
        "└──Finance"
        "\n"
        "    └──Accounting"
        "\n";
    CHECK(test1.getPrettyTree() == multiline);
    CHECK(test1.findMin()->key == "Accounting");
    test1.insert(two);
    CHECK(test1.height() == 1);
    temp = test1.returnValues("Accounting");
    CHECK(temp.size() == 1);
    CHECK(temp.at(0) == "id3");
    test1.add_value_vector("Accounting", "id200");
    temp = test1.returnValues("Accounting");
    CHECK(temp.at(1) == "id200");
    test1.makeEmpty();
    CHECK(test1.isEmpty() == true);
    test1.insert(one);
    test1.insert(two);
    test1.remove(two);
    CHECK(test1.contains(one) == true);
    CHECK(test1.contains(two) == false);

    // copy constructor 
    auto testCopy = test1;
    CHECK(testCopy.returnValues("Finance").at(0) == "id1");
    MapValue<std::string, std::vector<std::string>> copyOne("Business", v2);
    testCopy.insert(copyOne);
    CHECK_THROWS(test1.returnValues("Business"));
    CHECK(testCopy.returnValues("Business").at(0) == "id3");

    // assignment operator 
    AVLTree<MapValue<std::string, std::vector<std::string>>, std::string, std::vector<std::string>> testCopy2;
    testCopy2.insert(copyOne);
    testCopy2 = test1;
    CHECK(testCopy2.returnValues("Finance").at(0) == "id1");
    CHECK_THROWS(testCopy2.returnValues("Business"));
    testCopy2.insert(copyOne);
    CHECK_THROWS(test1.returnValues("Business"));
    CHECK(testCopy2.returnValues("Business").at(0) == "id3");

    // 5 node tree
    test1.insert(two);
    MapValue<std::string, std::vector<std::string>> three("Business", v);
    MapValue<std::string, std::vector<std::string>> four("Marketing", v);
    MapValue<std::string, std::vector<std::string>> five("Real Estate", v);
    test1.insert(three);
    test1.insert(four);
    test1.insert(five);
    CHECK(test1.getSortedTree() == "Accounting Business Finance Marketing Real Estate ");
    CHECK(test1.getTreeByLevel() == "Business Accounting Marketing Finance Real Estate ");
    multiline = 
        "└──Business"
        "\n"
        "    ├──Marketing"
        "\n"
        "    │   ├──Real Estate"
        "\n"
        "    │   └──Finance"
        "\n"
        "    └──Accounting"
        "\n";
    CHECK(test1.getPrettyTree() == multiline);
    CHECK(test1.findMin()->key == "Accounting");
    test1.insert(two);
    CHECK(test1.height() == 2);
    CHECK(test1.findKey("Accounting")->key == "Accounting");
    CHECK(test1.isBalanced() == true);
    test1.remove(four);
    CHECK_THROWS(test1.findKey("Marketing"));

    // max function
    CHECK(max(1, 2) == 2);
    CHECK(max(2, 1) == 2);
    CHECK(max(2, 2) == 2);

    // inserting in order
    AVLTree<MapValue<std::string, std::vector<std::string>>, std::string, std::vector<std::string>> test2;
    MapValue<std::string, std::vector<std::string>> a ("bank", v);
    MapValue<std::string, std::vector<std::string>> b ("credit", v);
    MapValue<std::string, std::vector<std::string>> c ("financial", v);
    MapValue<std::string, std::vector<std::string>> d ("crisis", v);
    test2.insert(a);
    test2.insert(b);
    test2.insert(c);
    test2.insert(d);
    CHECK(test2.isBalanced() == true);
}

TEST_CASE("Check that indexing works", "[IndexHandler]")
{
    IndexHandler ih;
    DocumentParser dp;
    string filename = "sample_data/coll_1/news_0064567.json";
    string id = "e17252151a2d36344b7104d14805414a55042abe";

    dp.findDocumentsTest(ih, filename);
    
    // Expected results obtained from printing out documents after stopwords and stemming 
    CHECK(ih.checkWords("percent", id) == 4);
    CHECK(ih.checkWords("berlin", id) == 1);
    CHECK(ih.checkPerson("joachim lang", id) == 1);
    CHECK(ih.checkOrg("eu", id) == 1);
}

TEST_CASE("Check that query works", "[QueryProcessor]") {

    //This is for the sample_data
    SearchEngine searchengine;
    std::string filePath = "saved_tree_file";

    vector<std::string> inputs = {
            "This First One is for No Processing Completed",
            "year\n",
            "year share\n", 
            "year ORG:reuters\n",
            "year -rate\n"
    };
    vector<std::string> outputs = {
            "Error: Index Handler and Document Parser are not set up.\n",

            "Document 1: Eaton Vance Closed-End Funds Release Estimated Sources Of Distributions\n"
                "\tPublication: cnbc.com\n"
                "\tDate Published: 2018-02-28T18:54:00.000+02:00\n"
            "Document 2: BRIEF-Francesca’s Sees Q4 2017 GAAP Earnings Per Share $0.07 To $0.11\n"
                "\tPublication: Reuters Editorial\n"
                "\tDate Published: 2018-02-28T05:29:00.000+02:00\n"
            "Document 3: German firms doing business in UK gloomy about Brexit - survey\n"
                "\tPublication: Reuters Editorial\n"
                "\tDate Published: 2018-02-27T20:09:00.000+02:00\n",

            "Document 1: Eaton Vance Closed-End Funds Release Estimated Sources Of Distributions\n"
                "\tPublication: cnbc.com\n"
                "\tDate Published: 2018-02-28T18:54:00.000+02:00\n"
            "Document 2: BRIEF-Francesca’s Sees Q4 2017 GAAP Earnings Per Share $0.07 To $0.11\n"
                "\tPublication: Reuters Editorial\n"
                "\tDate Published: 2018-02-28T05:29:00.000+02:00\n",
            
            "Document 1: BRIEF-Francesca’s Sees Q4 2017 GAAP Earnings Per Share $0.07 To $0.11\n"
                "\tPublication: Reuters Editorial\n"
                "\tDate Published: 2018-02-28T05:29:00.000+02:00\n"
            "Document 2: German firms doing business in UK gloomy about Brexit - survey\n"
                "\tPublication: Reuters Editorial\n"
                "\tDate Published: 2018-02-27T20:09:00.000+02:00\n",
            
            "Document 1: German firms doing business in UK gloomy about Brexit - survey\n"
                "\tPublication: Reuters Editorial\n"
                "\tDate Published: 2018-02-27T20:09:00.000+02:00\n"
    };

    for(size_t i = 0; i < inputs.size(); i++) {
        std::istringstream in(inputs[i]);
        std::ostringstream out;

        searchengine.test_new_query(out, in);
        CHECK(out.str() == outputs[i]);
        //Load the indices and documents to file if successful.
        if(i == 0) {
            searchengine.load_index_to_file(filePath);
        }
    }
}

TEST_CASE("Check that persistence algorithm works", "[Persistence]")
{
    SearchEngine searchengine;
    std::string filePath = "saved_tree_file";

    std::ostringstream raw_processing_index_handler;
    std::ostringstream raw_processing_document_parser;
    std::ostringstream loaded_index_handler;
    std::ostringstream loaded_document_parser;

    //Checks if nothing is there if no processing has been done.
    searchengine.checking_index_handler_for_processing_and_persistence(raw_processing_index_handler);
    searchengine.checking_document_parser_for_processing_and_persistence(raw_processing_document_parser);

    searchengine.save_index_to_file(filePath);
    searchengine.load_index_to_file(filePath);

    //Checks if there is nothing there, nothing will be loaded from the file.
    searchengine.checking_index_handler_for_processing_and_persistence(loaded_index_handler);
    searchengine.checking_document_parser_for_processing_and_persistence(loaded_document_parser);

    //Checks if there is nothing there.
    CHECK(raw_processing_index_handler.str() == "WORDS\nPERSONS\nORGANIZATIONS\n");
    CHECK(raw_processing_document_parser.str() == "Is Empty? 0 and Number of Documents: 0");
    CHECK(loaded_index_handler.str() == "WORDS\nPERSONS\nORGANIZATIONS\n");
    CHECK(loaded_document_parser.str() == "Is Empty? 0 and Number of Documents: 0");

    CHECK(raw_processing_index_handler.str() == loaded_index_handler.str());
    CHECK(raw_processing_document_parser.str() == loaded_document_parser.str());

    //Processes the documents and checks if it updated.
    searchengine.process_documents();

    searchengine.checking_index_handler_for_processing_and_persistence(raw_processing_index_handler);
    searchengine.checking_document_parser_for_processing_and_persistence(raw_processing_document_parser);

    searchengine.save_index_to_file(filePath);
    //searchengine.load_index_to_file(filePath);

    //Checks if the persistence will load save and load correctly.
    searchengine.checking_index_handler_for_processing_and_persistence(loaded_index_handler);
    searchengine.checking_document_parser_for_processing_and_persistence(loaded_document_parser);

    std::istringstream raw_processing_index_handler1 (raw_processing_index_handler.str());
    std::istringstream raw_processing_document_parser1 (raw_processing_document_parser.str());
    std::istringstream loaded_index_handler1 (loaded_index_handler.str());
    std::istringstream loaded_document_parser1 (loaded_document_parser.str());

    CHECK(raw_processing_index_handler.str() != "WORDS\nPERSONS\nORGANIZATIONS\n");
    CHECK(raw_processing_document_parser.str() != "Is Empty? 0 and Number of Documents: 0");
    CHECK(loaded_index_handler.str() != "WORDS\nPERSONS\nORGANIZATIONS\n");
    CHECK(loaded_document_parser.str() != "Is Empty? 0 and Number of Documents: 0");

    std::string currentWord1;
    std::string currentWord2;
    //Checks if all the contents of the index handlers are the same before and after.
    while (raw_processing_index_handler1 >> currentWord1) {
        loaded_index_handler1 >> currentWord2;
        CHECK(currentWord1 == currentWord2);
    }
    //Checks if the document parser has the same number of as before.
    CHECK(raw_processing_document_parser.str() == loaded_document_parser.str());
}