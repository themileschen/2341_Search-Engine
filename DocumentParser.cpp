// Class Owner: Miles

#include "DocumentParser.h"

DocumentParser::DocumentParser() 
{
    stopwords.reserve(191); // total of 184 stopwords

    // Initialize unordered set for stopwords
    ifstream input;
    input.open("stopWords.txt");
    if(!input.is_open())
    {
        cerr << "cannot open file: " << "stopWords.txt" << "\n";
        return;
    }
    string word;
    while(!input.eof())
    {
        getline(input, word);
        stopwords.insert(word);
    }
}

void DocumentParser::save_documents_to_file(std::ofstream& output_file) {
    //Outputs document unordered_map to the output file.
    for(auto const& pair : documents_tree) {
        //Separates each document ID and document filename in the unordered_map with a '\n'.
        output_file << pair.first << "\n" << pair.second << "\n";
    }
    //Ends the document unordered_map in the output file with a ;
    output_file << ";\n";
    std::cout << "Documents Tree has been successfully saved to the file!\n";
}
void DocumentParser::load_documents_to_file(std::ifstream& input_file) {

    documents_tree.clear(); //Clears the current document unordered_map if there is something in it.
    documents_tree.reserve(306209); // Total of 306188 articles.

    //Adds document ID : document filename pairs from the input_file to the document unordered_map.
    while (true) {
        string docName;
        string fileName;

        //Each document ID and document filename is separated by '\n'.
        getline(input_file, docName, '\n');
        if (docName == ";" || input_file.eof()) {
            //Semicolon ; signals the end of the file and end of the document unordered_map.
            break;
        }
        getline(input_file, fileName, '\n');

        //Add the pair to the unordered_map.
        documents_tree[docName] = fileName;
    }
    std::cout << "Documents Tree has been successfully loaded from the file!\n";
}

void DocumentParser::findDocuments(IndexHandler& index_handler, char*& data) // reads in all the files and sends each to processDocument()
{
    auto it = filesystem::recursive_directory_iterator(data); // recursively iterate thorugh the file folder 

    index_handler.reset(); //Resets the index handler just in case the trees were already loaded.
    documents_tree.clear(); //Clears the current document tree if there is something in it.
    documents_tree.reserve(306209); // total of 306188 articles 

    start = chrono::high_resolution_clock::now();

    for (const auto &entry : it)
    {
        if (entry.is_regular_file() && entry.path().extension().string() == ".json")
        {
            processDocument(entry.path().string(), index_handler); // process each file that is a proper .json file 
        }
    }
}

void DocumentParser::findDocumentsTest(IndexHandler& index_handler, string filename)
{
    index_handler.reset();
    documents_tree.clear();
    processDocument(filename, index_handler);
}

bool DocumentParser::parseWord(string& word) {
    // convert all words to lowercase (look at the first letter)
    if(word.at(0) >= 65 && word.at(0) <= 90)
        word.at(0) += 32;
    
    //If the word is a stopword, ignore the word and do not do anything with it.
    if(isStopword(word)) { return false; }

    //Stems the word.
    stemming(word);
    return true;
}
bool DocumentParser::isStopword(std::string& word)
{
    // Using list of 184 stopwords from: https://www.semrush.com/blog/seo-stop-words/#a-comprehensive-a-z-list-of-175+-stop-words

    std::unordered_set<std::string>::const_iterator got = stopwords.find(word);

    if ( got == stopwords.end() )
    {
        return false; // not a stopword
    }
    else
    {
        return true; // is a stopword 
    }
} 

void DocumentParser::stemming(string& word) // stems the word 
{
    Porter2Stemmer::trim(word);
    Porter2Stemmer::stem(word);
}

void DocumentParser::printIndexingSpeed()
{
    end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end-start);
    double seconds = elapsed.count() / 1000.0;
    double speed = 100 / seconds;

    cout << "Document indexing speed for last 100 documents: " << speed << " documents per second." << "\n";
    
    start = chrono::high_resolution_clock::now(); // restart the clock for the next round of documents 
}

void DocumentParser::processDocument(const string& fileName, IndexHandler& index_handler)
{
    processed++;
    if(processed % 100 == 0) // print the speed for every 100 documents 
    {
        printIndexingSpeed();
    }

    ifstream input(fileName);
    if(!input.is_open())
    {
        cerr << "cannot open file: " << fileName << "\n";
        return;
    }
    IStreamWrapper isw(input);
    Document d;
    d.ParseStream(isw); 
        
    // get id
    std::string id = d["uuid"].GetString();

    // get words
    std::string text = d["text"].GetString();
    
    istringstream iss(text);
    string word;
    while(getline(iss, word, ' ')) // for each word in the text 
    {
        // convert all words to lowercase (look at the first letter)
        if(word != "") // if word is not an empty space 
        {
            words++;

            if(word.at(0) >= 65 && word.at(0) <= 90)
            word.at(0) += 32;
        }

        if(!isStopword(word)) // if not stopword
        {
            // stem and add to words tree 
            stemming(word);
            index_handler.update_words_tree(word, id, 1);
        }
        else
            numStopwords++; // track number of stopwords 
    }

    // get persons
    auto persons = d["entities"]["persons"].GetArray();
    for (auto &p : persons)
    {
        index_handler.update_persons_tree(p["name"].GetString(), id, 1);
    }

    // get orgs
    auto organizations = d["entities"]["organizations"].GetArray();
    for (auto &o : organizations)
    {
        index_handler.update_organizations_tree(o["name"].GetString(), id, 1);
    }

    documents_tree.insert({id, fileName}); // keep track of the current document 

    input.close();
}

void DocumentParser::printDocuments(std::ostream& out) 
{
    out << "Documents\n";
    for(auto const& pair : documents_tree)
    {
        vector<string> result = getDocumentInfo(pair.first);
        out << "ID: " << result.at(0) << "\n";
        out << "\tTitle: " << result.at(1) << "\n";
    }
}

void DocumentParser::print_document_and_get_text(string& key, std::string& text, ostream& out) 
{
    //Get the filename associated with the document ID key given.
    auto fileName = documents_tree.at(key);
    ifstream input(fileName);

    //Checks if the filename exists to be opened.
    if (!input.is_open()) {
        cerr << "Cannot Open File: " << fileName << "\n";
        text = "No Text Found!";
    }

    //Goes through the .json document for information.
    IStreamWrapper isw(input);
    Document d;
    d.ParseStream(isw); 

    //Gets the document information to print out.
    out << d["title"].GetString() << "\n";
    out << "\tPublication: " << d["author"].GetString() << "\n";
    out << "\tDate Published: " << d["published"].GetString() << "\n";
    //Gets the text of the file to put into the string text variable.
    text = d["text"].GetString();
}

vector<string> DocumentParser::getDocumentInfo(const string& key)
{
    vector<string> documentInfo(5); // to store the document info 
    auto fileName = documents_tree.at(key);

    ifstream input(fileName);
  
    IStreamWrapper isw(input);
    Document d;
    d.ParseStream(isw); 
        
    documentInfo.at(0) = key;
    documentInfo.at(1) = d["title"].GetString();
    documentInfo.at(2) = d["author"].GetString();
    documentInfo.at(3) = d["published"].GetString();
    documentInfo.at(4) = d["text"].GetString();

    return documentInfo;
}

string DocumentParser::getTitle(string key)
{
    return getDocumentInfo(key).at(1);
}
