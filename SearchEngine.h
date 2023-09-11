#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include "DocumentParser.h"
#include "IndexHandler.h"
#include "QueryEngine.h"
/**
 * @brief Search Engine
 */
class SearchEngine {
private:
    DocumentParser document_parser;
    IndexHandler index_handler;
    QueryEngine query_engine;
public:
    SearchEngine();

    /**
     * Opens the fileName as an output file and saves both the trees in the index handler and 
     * the unordered_map in the document parser to the tree.
     * */
    void save_index_to_file(const std::string& fileName);

    /**
     * Opens the fileName as an input file and loads both the trees in the index handler and 
     * the unordered_map in the document parser to the tree from the file.
     * */
    void load_index_to_file(const std::string& fileName);

    //Processes documents by updating words, organizations, and persons tree.
    void process_documents(char*&);

    //Asks the user to input a new query to search through the search engine.
    //Outputs statistics about the new query.
    void new_query();

    //Prints out the words, organizations, and persons tree in order to check if they are correct.
    void checking_index_handler_for_processing_and_persistence(std::ostream& out = std::cout);
    //Prints out how many elements the document parser has for testing.
    void checking_document_parser_for_processing_and_persistence(std::ostream& out = std::cout);
    //Tests the query engine for getting a new query.
    void test_new_query(std::ostream& out, std::istream& in);
};

#endif
