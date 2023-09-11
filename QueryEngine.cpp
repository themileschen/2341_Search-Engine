/**Owner: Marc Pham*/
#include "QueryEngine.h"

QueryEngine::QueryEngine() {}

bool QueryEngine::add_documents_for_query(
    std::istringstream& query_stream, 
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_words, 
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_remove_words, 
    DocumentParser& document_parser, 
    IndexHandler& index_handler) 
{

    /**
     * documents_for_words has all the documents with a word, organization, or person from the query.
     * documents_for_remove_words has all the documents with a word that the user does not want to include.
    */
    
    std::string currentWord;
    while(query_stream >> currentWord) {
        //Checks for words the user does not want to be in any of the documents.
        if(currentWord.length() >= 1 && currentWord[0] == '-') {
            //Removes the - sign in front.
            int newWordLength = currentWord.length() - 1;
            currentWord = currentWord.substr(1, newWordLength);

            //Inserts the documents in the documents_for_remove_words unordered map, if the word is not a stopword.
            if(document_parser.parseWord(currentWord)) {
                if(index_handler.checkWord(currentWord, 0))
                    documents_for_remove_words.insert({currentWord, index_handler.get_documents_for_word(currentWord)});
                else    
                {
                    cout << "Key " << currentWord << " not found in any of the documents! Please try again" << endl;
                    return false;
                }
            }
            continue;
        }
        //Checks for Organizations and adds their associated documents to the documents_for_words unordered map if applicable.
        else if (currentWord.length() > 4 && currentWord.substr(0, 4) == "ORG:") {
            if(!add_documents_for_organization(currentWord, query_stream, documents_for_words, index_handler))
                return false;
            continue;
        }
        
        //Checks for Persons and adds their associated documents to the documents_for_words unordered map if applicable.
        if (currentWord.length() > 7 && currentWord.substr(0, 7) == "PERSON:") {
            if(!add_documents_for_person(currentWord, query_stream, documents_for_words, index_handler))
                return false;
            continue;
        }

        //Checks for normal words.
        if(document_parser.parseWord(currentWord)) {
            //If the currentWord is not a stemword, add its associated documents to the documents_for_words unordered map.
            if(index_handler.checkWord(currentWord, 0))
                documents_for_words.insert({currentWord, index_handler.get_documents_for_word(currentWord)});
            else    
            {
                cout << "Key " << currentWord << " not found in any of the documents! Please try again" << endl;
                return false;
            }
        }
    }
    return true;
}

void QueryEngine::add_relevancy_ranking(
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_words, 
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_remove_words, 
    std::priority_queue<MapValue<int, std::string>>& top15,
    const std::string& currentDocName) {

    int relevancy_ranking = 0;
    for(const auto& element : documents_for_words) {
        //If a word from the query is not found in the document, skip the document completely.
        if(element.second.find(currentDocName) == element.second.end()) { return; }
        
        //Whatever the frequency is for the document for that word is added to the relevancy ranking.
        relevancy_ranking += element.second[currentDocName];
    }
    
    //If the document contains a remove word, skip the document.
    for(const auto& element : documents_for_remove_words)
        if(element.second.find(currentDocName) != element.second.end()) { return; }
    
    result++; // add to the counter: this document contains all the search terms 
    /**
     * The top15 minimum heap has the 15 most relevant documents for the query.
     * The top of the heap will have the least relevant document (of the lowest relevancy ranking).
     * If the new document is more relevant than the 15th most relevant document, add it to top15.
    */
    if(top15.size() < 15) {
        top15.push(MapValue(-relevancy_ranking, currentDocName));
    } else if (-relevancy_ranking < top15.top().getKey()) {
        top15.pop();
        top15.push(MapValue(-relevancy_ranking, currentDocName));
    }
}

bool QueryEngine::add_documents_for_organization(
    std::string& currentWord, 
    std::istringstream& query_stream, 
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_words, 
    IndexHandler& index_handler) {

    //Remove the ORG: at the beginning of the search term.
    int newWordLength = currentWord.length() - 4;
    currentWord = currentWord.substr(4, newWordLength);

    /**
     * Finds the rest of the organization search term.
     * For example, if the query is ORG:bank of america, the code will initially read in ORG:bank
     * and "of america" needs to be added to "bank" for the full organization search term.
    */
    std::ostringstream organizationWord;
    organizationWord << currentWord << " ";
    /**End of the organization search term if reached when
     * (1) there is nothing left to read in the query or
     * (2) a person search term has been reached.
    */
    while(!query_stream.eof()) {
        query_stream >> currentWord;
        if(currentWord.length() > 7 && currentWord.substr(0, 7) == "PERSON:") {
            //Add the documents for the person search term when it is reached.
            if(!add_documents_for_person(currentWord, query_stream, documents_for_words, index_handler))
                return false;
            break;
        }
        else if(currentWord.length() > 4 && currentWord.substr(0, 4) == "ORG:") {
            //Adds the documents for the organization search term when it is reached.
            if(!add_documents_for_organization(currentWord, query_stream, documents_for_words, index_handler))
                return false;
            break;
        }
        organizationWord << currentWord << " ";
    }

    //Find the documents for the full organization search term.
    std::string finalOrganizationWord = organizationWord.str();
    finalOrganizationWord = finalOrganizationWord.substr(0, finalOrganizationWord.size() - 1);

    if(index_handler.checkWord(finalOrganizationWord, 1))
        documents_for_words.insert({finalOrganizationWord, index_handler.get_documents_for_organizations(finalOrganizationWord)});
    else    
    {
        cout << "Key " << currentWord << " not found in any of the documents! Please try again" << endl;
        return false;
    }

    return true;
}
bool QueryEngine::add_documents_for_person(
    std::string& currentWord, 
    std::istringstream& query_stream, 
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_words, 
    IndexHandler& index_handler) {
    
    //Remove the PERSON: at the beginning of the search term.
    int newWordLength = currentWord.length() - 7;
    currentWord = currentWord.substr(7, newWordLength);

    /**
     * Finds the rest of the person search term until
     * (1) There is nothing left to read in the query or
     * (2) an organization search term has been reached.
    */
    std::ostringstream personWord;
    personWord << currentWord << " ";
    while(!query_stream.eof()) { //While the new word is not the end of the query.
        query_stream >> currentWord;
        if(currentWord.length() > 7 && currentWord.substr(0, 7) == "PERSON:") {
            //Add the documents for the person search term when it is reached.
            if(!add_documents_for_person(currentWord, query_stream, documents_for_words, index_handler))
                return false;
            break;
        }
        else if(currentWord.length() > 4 && currentWord.substr(0, 4) == "ORG:") {
            //Adds the documents for the organization search term when it is reached.
            if(!add_documents_for_organization(currentWord, query_stream, documents_for_words, index_handler))
                return false;
            break;
        }
        personWord << currentWord << " ";
    }

    //Find the documents for the full persons search term.
    std::string finalPersonWord = personWord.str();
    finalPersonWord = finalPersonWord.substr(0, finalPersonWord.size() - 1);
     
    if(index_handler.checkWord(finalPersonWord, 2))
        documents_for_words.insert({finalPersonWord, index_handler.get_documents_for_persons(finalPersonWord)});
    else    
    {
        cout << "Key " << currentWord << " not found in any of the documents! Please try again" << endl;
        return false;
    }

    return true;
}

void QueryEngine::printTop15Documents(
    std::priority_queue<MapValue<int, std::string>>& top15, 
    vector<std::string>& text_of_top15documents, 
    DocumentParser& document_parser) {

    cout << "Total number of results obtained: " << getResult() << endl;

    /**
     * Store the document IDs of the top 15 most relevant documents for the query into the vector text_of_top15documents,
     * where the beginning of the vector are the most relevant documents.
    */
    for(int i = top15.size() - 1; i >= 0; i--) {
        std::string currentDocument = top15.top().getValues();
        text_of_top15documents[i] = currentDocument;
        top15.pop();
    }

    /**
     * Print out the document details for each of the documents from most to least relevant.
     * Get the full text of each of the document IDs in place of where the document ID was in the text_of_top15documents.
    */
    for(size_t i = 0; i < text_of_top15documents.size(); i++) {
        std::cout << "Document " << i + 1 << ": ";
        document_parser.print_document_and_get_text(text_of_top15documents[i], text_of_top15documents[i]);
    }
}

void QueryEngine::new_query(IndexHandler& index_handler, DocumentParser& document_parser) {

    result = 0; // reset results counter
    
    //Stops new_query if the documents have not been processed or loaded yet.
    if (!index_handler.are_trees_set() || !document_parser.is_document_tree_set()) {
        std::cout << "Error: Index Handler and Document Parser are not set up.\n";
        return;
    }
    
    //Gets the new query until it is a valid query.
    std::string query = "";
    std::cout << "Enter your query here: ";
    getline(std::cin, query, '\n');

    while (query == "") {
        std::cout << "Please enter a query: ";
        getline(std::cin, query, '\n');
    }

    //The query but as a stream.
    std::istringstream query_stream(query);

    /**
     * Holds the documents for all the words, organizations, and persons in the query.
     * Each term has an unordered_map with the documents it is in and the frequency of the word in each document.
    */
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&> documents_for_words;
    documents_for_words.reserve(5);
    /**
     * Holds the documents that have words that the user does not want have.
     * Each term has an unordered_map with the documents it is in and the frequency of the word in each document.
    */
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&> documents_for_remove_words;
    documents_for_remove_words.reserve(5);

    //Gets all the documents that the words in the query are in, putting them into documents_for_words and documents_for_removed_words respectively.
    if(!add_documents_for_query(query_stream, documents_for_words, documents_for_remove_words, document_parser, index_handler))
        return;

    /**Finds the search term with appears in the smallest number of documents to make relevancy ranking more efficient*/
    std::string key_of_smallest_size = getWordWithLeastNumberOfDocuments(documents_for_words);

    /** 
     * top15 is a minimum heap that holds the 15 most relevant documents for the query.
     * The top of the tree has the least relevant document of the 15 most relevant ones.
     */
    std::priority_queue<MapValue<int, std::string>> top15;
    for(const auto& element : documents_for_words.at(key_of_smallest_size)) {
		add_relevancy_ranking(documents_for_words, documents_for_remove_words, top15, element.first);
    }

    //If no relevant documents were found...
    if(top15.size() == 0) {
        std::cout << "No Documents were found with all the search terms!\n";
        return;
    }

    //Vector of the full text of the 15 most relevant documents for the query.
    vector<std::string> text_of_top15documents(top15.size(), "");

    //Prints out the 15 documents from most to least relevant.
    printTop15Documents(top15, text_of_top15documents, document_parser);

    //Prints a specific document that the user must choose.
    printSpecificDocument(text_of_top15documents);
    std::cin.ignore(); //There is an extra space that will make main.cpp not work too well.
}
std::string QueryEngine::getWordWithLeastNumberOfDocuments(
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&>& documents_for_words) {

    //Gets the key that appears in the smallest number of documents.
    std::string key_of_smallest_size = "";
    size_t smallest_size = (size_t) - 1;
    for(const auto& pair : documents_for_words) {
        if(pair.second.size() < smallest_size) {
            smallest_size = pair.second.size();
            key_of_smallest_size = pair.first;
        }
    }
    return key_of_smallest_size;
}
void QueryEngine::printSpecificDocument(const vector<std::string>& text_of_top15documents) {
   //Prompts the user to choose the document they want to see the text of until they input a valid number.
    std::cout << "Input the number of the document that you want to open up: ";
    std::string number_input = "";

    while (true) {
        cin >> number_input;
        
        //Checks if the input is a non-number value or too long to be valid, skipping it if this is so.
        bool valid_input = true;
        if(number_input.size() > 2)
        {
            //If the input is longer than two digits, ask for the input again.
            std::cout << "Please input a valid number: ";
            continue;
        }
        for(const auto& character : number_input)
        {
            if(!isdigit(character))
            {
                std::cout << "Please input a valid number: ";
                valid_input = false;
                break;
            }
        }
        if(valid_input == false)
        {
            continue; //If the input is not a number, ask for the input again.
        }

        int int_number = stoi(number_input);
        //If the number does not correspond to a document, ask for the input again.
        if(int_number < 0 || int_number > static_cast<int>(text_of_top15documents.size()))
        {
            std::cout << "Please input a valid number: ";
        } 
        else { break; } //If the input is valid, stop asking for an input.
    }

    //Prints the document the user chose.
    std::cout << "\n" << text_of_top15documents[stoi(number_input)-1] << "\n";
}
void QueryEngine::test_new_query(IndexHandler& index_handler, DocumentParser& document_parser, std::ostream& out, std::istream& in) {
    //Stops new_query if the documents have not been processed or loaded yet.
    if (!index_handler.are_trees_set() || !document_parser.is_document_tree_set()) {
        out << "Error: Index Handler and Document Parser are not set up.\n";
        return;
    }
    
    //Gets the new query until it is a valid query.
    std::string query = "";
    getline(in, query, '\n');
    std::cout << "Current Query: " << query;

    //The query but as a stream.
    std::istringstream query_stream(query);

    /**
     * Holds the documents for all the words, organizations, and persons in the query.
     * Each term has an unordered_map with the documents it is in and the frequency of the word in each document.
    */
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&> documents_for_words;
    documents_for_words.reserve(5);
    /**
     * Holds the documents that have words that the user does not want have.
     * Each term has an unordered_map with the documents it is in and the frequency of the word in each document.
    */
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>&> documents_for_remove_words;
    documents_for_remove_words.reserve(5);

    //Gets all the documents that the words in the query are in, putting them into documents_for_words and documents_for_removed_words respectively.
    add_documents_for_query(query_stream, documents_for_words, documents_for_remove_words, document_parser, index_handler);

    /**Finds the search term with appears in the smallest number of documents to make relevancy ranking more efficient*/
    std::string key_of_smallest_size = getWordWithLeastNumberOfDocuments(documents_for_words);

    /** 
     * top15 is a minimum heap that holds the 15 most relevant documents for the query.
     * The top of the tree has the least relevant document of the 15 most relevant ones.
     */
    std::priority_queue<MapValue<int, std::string>> top15;
    for(const auto& element : documents_for_words.at(key_of_smallest_size)) {
		add_relevancy_ranking(documents_for_words, documents_for_remove_words, top15, element.first);
    }

    //If no relevant documents were found...
    if(top15.size() == 0) {
        out << "No Documents were found with all the search terms!\n";
        return;
    }

    //Vector of the full text of the 15 most relevant documents for the query.
    vector<std::string> text_of_top15documents(top15.size(), "");

    /**
     * Store the document IDs of the top 15 most relevant documents for the query into the vector text_of_top15documents,
     * where the beginning of the vector are the most relevant documents.
    */
    for(int i = top15.size() - 1; i >= 0; i--) {
        std::string currentDocument = top15.top().getValues();
        text_of_top15documents[i] = currentDocument;
        top15.pop();
    }

    /**
     * Print out the document details for each of the documents from most to least relevant.
     * Get the full text of each of the document IDs in place of where the document ID was in the text_of_top15documents.
    */
    for(size_t i = 0; i < text_of_top15documents.size(); i++) {
        out << "Document " << i + 1 << ": ";
        document_parser.print_document_and_get_text(text_of_top15documents[i], text_of_top15documents[i], out);
    }
}