/** Owner: Marc Pham*/
#ifndef QueryEngine_H
#define QueryEngine_H

#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
#include "AVLTree.h"
#include "IndexHandler.h"
#include "DocumentParser.h"
#include "MapValue.h"
using namespace std;
/**
 * @brief Query Engine: 
 * Takes in a query of words, organizations, and persons and prints out the 15 most relevant documents based on the query.
 */
class QueryEngine {

private:
    size_t result = 0;
    
public:
    QueryEngine();

    //Takes in a new query and prints out the documents associated with that query.
    void new_query(IndexHandler& index_handler, DocumentParser& document_parser);

    //Gets all the documents that the words in the query are in.
    bool add_documents_for_query(
        std::istringstream& query_stream, 
        std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_words, 
        std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_remove_words, 
        DocumentParser& document_parser, 
        IndexHandler& index_handler);

    //Gets all the relevancy rankings for the documents are sorts the top 15 most relevant documents in the top15 minimum heap.
    void add_relevancy_ranking(
        std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_words, 
        std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_remove_words, 
        std::priority_queue<MapValue<int, std::string>>& top15,
        const std::string& currentDocName);

    //Called by add_documents_for_query() to get the documents for organizations.
    bool add_documents_for_organization(
        std::string& currentWord, 
        std::istringstream& query_stream, 
        std::unordered_map<std::string, 
        std::unordered_map<std::string, size_t>&>& documents_for_words, 
        IndexHandler& index_handler);
    
    //Called by add_documents_for_query() to get the documents for persons.
    bool add_documents_for_person(
        std::string& currentWord, 
        std::istringstream& query_stream, 
        std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_words, 
        IndexHandler& index_handler);
    
    //Prints the 15 most relevant documents that is sorted in the top15 minimum heap.
    void printTop15Documents(
        std::priority_queue<MapValue<int, std::string>>& top15, 
        vector<std::string>& text_of_top15documents, 
        DocumentParser& document_parser);
    
    //Prints a specific document that the user must choose.
    void printSpecificDocument(const vector<std::string>& text_of_top15documents);

    /**Finds the search term with appears in the smallest number of documents to make relevancy ranking more efficient*/
    std::string getWordWithLeastNumberOfDocuments(std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_words);

    //Exclusively used to test new query; copied code from the actual new query with slight alterations.
    void test_new_query(IndexHandler& index_handler, DocumentParser& document_parser, std::ostream& out, std::istream& in);

    // Get the number of results 
    size_t getResult()
    {
        return result;
    }
};

#endif
