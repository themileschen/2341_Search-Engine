// Class Owner: Miles

#ifndef DocumentParser_H
#define DocumentParser_H

#include "AVLTree.h"
#include "IndexHandler.h"

#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
using namespace rapidjson;
using namespace std;

#include "porter2_stemmer.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cctype>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>

/**
 * @brief Document Parser
 */
class DocumentParser {
private:
    //unordered_map of document ID to document fileName.
    unordered_map<string, string> documents_tree;
    //Stopwords that should be removed from all queries and trees.
    unordered_set<string> stopwords;

    size_t processed = 0; // for timing 
    chrono::high_resolution_clock::time_point start;
    chrono::high_resolution_clock::time_point end;

    size_t words = 0; //Number of Words counter.
    size_t numStopwords = 0; //Number of Stopwords counter.

public:
    DocumentParser();

    //Outputs document unordered_map to the output file.
    void save_documents_to_file(std::ofstream& output_file);
    //Adds document ID : document filename pairs from the input_file to the document unordered_map.
    void load_documents_to_file(std::ifstream& input_file);

    void findDocuments(IndexHandler& index_handler, char*& data);
    void findDocumentsTest(IndexHandler& index_handler, string fileName);

    void processDocument(const string&, IndexHandler& index_handler);
    
    bool parseWord(string& word);
    bool isStopword(string&);
    //Stems the string passed in as a parameter.
    void stemming(string& word);

    void printDocuments(std::ostream& out = std::cout);
    //Prints the documents information and puts the text of that document in the string text variable.
    void print_document_and_get_text(string& key, std::string& text, ostream& out = std::cout);
    vector<string> getDocumentInfo(const string& key);
    string getTitle(string);
    void printIndexingSpeed();

    //Checks if the document tree has stuff in it or if it is empty.
    bool is_document_tree_set() {
      return !documents_tree.empty();
    }

    size_t numDocuments()
    {
      return documents_tree.size(); 
    }

    size_t getNumWords()
    {
      return words;
    }
    size_t getNumStopwords()
    {
      return numStopwords;
    }
};

#endif