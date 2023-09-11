#include "SearchEngine.h"

SearchEngine::SearchEngine() {
    std::cout << "Starting up Search Engine.\n";
}

void SearchEngine::save_index_to_file(const std::string& fileName) {
    /**
     * Opens the fileName as an output file and saves both the trees in the index handler and 
     * the unordered_map in the document parser to the tree.
     * */
    std::ofstream output_file;
    output_file.open(fileName);

    index_handler.save_index_to_file(output_file);
    document_parser.save_documents_to_file(output_file);

    output_file.close();
}
void SearchEngine::load_index_to_file(const std::string& fileName) {
    /**
     * Opens the fileName as an input file and loads both the trees in the index handler and 
     * the unordered_map in the document parser to the tree from the file.
     * */
    std::ifstream input_file;
    input_file.open(fileName);

    //Checks if the input file exists or can be opened.
    if(!input_file.is_open()) {
        std::cout << "Error: The specified input file cannot be opened.  The file was mostly likely not created yet.\n";
        return;
    }

    index_handler.load_index_to_file(input_file);
    document_parser.load_documents_to_file(input_file);

    input_file.close();
}

void SearchEngine::process_documents(char*& data) 
{
    //Starts tracking how long the document processing takes.
    auto start = chrono::high_resolution_clock::now();

    //Processes Documents.
    document_parser.findDocuments(index_handler, data);

    //Calculates how long the document took to process data.
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end-start);

    //Calculates statistics.
    double seconds = elapsed.count() / 1000.0;
    size_t docs = document_parser.numDocuments();
    double speed = docs / seconds;

    //Outputs statistics for word processing.
    cout << "Total number of articles parsed: " << docs << "\n";
    cout << "Number of unique words indexed: " << index_handler.get_num_words() << "\n";
    cout << "Total number of words indexed: " << document_parser.getNumWords() << "\n";
    cout << "Percentage of words that are stopwords: " << 100.0 * document_parser.getNumStopwords() / document_parser.getNumWords() << "%" << "\n";
    cout << "Document indexing speed: " << speed << " documents per second." << "\n";
}

void SearchEngine::new_query() 
{
    //Asks the user to input a new query to search through the search engine.
    query_engine.new_query(index_handler, document_parser);

    //Outputs statistics.
    cout << "\n" << "\n";
    cout << "Total number of articles parsed: " << document_parser.numDocuments() << "\n";
    cout << "Total number of words indexed: " << document_parser.getNumWords() << "\n";
    cout << "Number of unique words indexed: " << index_handler.get_num_words() << "\n";
}

void SearchEngine::checking_index_handler_for_processing_and_persistence(std::ostream& out) {
    //Prints out the words, organizations, and persons tree in order to check if they are correct.
    index_handler.printInfo(out);
}
void SearchEngine::checking_document_parser_for_processing_and_persistence(std::ostream& out) {
    //Prints out how many elements the document parser has for testing.
    out << "Is Empty? " << document_parser.is_document_tree_set() << " and Number of Documents: " << document_parser.numDocuments();
}
void SearchEngine::test_new_query(std::ostream& out, std::istream& in) {
    //Tests the query engine for getting a new query.
    query_engine.test_new_query(index_handler, document_parser, out, in);
}
