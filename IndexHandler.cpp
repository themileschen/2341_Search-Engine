#include "IndexHandler.h"

IndexHandler::IndexHandler() {}

void IndexHandler::load_to_tree( AVLTree<MapValue<string, unordered_map<string, size_t>>, string, unordered_map<string, size_t>>& tree, std::ifstream& input_file) {
    string currentWord;
    while (currentWord != ";") {
        //Gets the current word, separated by '\n' in the input file.
        getline(input_file, currentWord, '\n');

        //Semicolon ; represents the end of that tree, so stop reading into that tree.
        if(currentWord == ";") {
            return;
        }

        //Holds the document ID : frequency of the word unordered_map for each word.
        unordered_map<string, size_t> tempMap;

        string docName;
        size_t frequency;
        //Gets inputs from input_file two at a time, with docName coming first then frequency second.
        while (true) {
            getline(input_file, docName, '\n');

            //Unordered map ends when a } is reached.
            if (docName == "}") {
                break;
            }
            //Adds pairs to the unordered_map as you read it in.
            input_file >> frequency;
            tempMap.insert({docName, frequency});
            input_file.ignore();
        }

        //Add the word and the unordered_map associated with word to the correct tree.
        MapValue<string, unordered_map<string, size_t>> newNode(currentWord, tempMap);
        tree.insert(newNode);
    }
}

void IndexHandler::save_index_to_file(std::ofstream& output_file) {
    //Output the words, persons, and organizations trees in order to an output_file.
    wordsTree.printTreeForPersistence(output_file);
    output_file << ";\n";
    std::cout << "Words Tree successfully saved!\n";

    personsTree.printTreeForPersistence(output_file);
    output_file << ";\n";
    std::cout << "Persons Tree successfully saved!\n";

    organizationsTree.printTreeForPersistence(output_file);
    output_file << ";\n";
    std::cout << "Organizations Tree successfully saved!\n";
}
void IndexHandler::load_index_to_file(std::ifstream& input_file) {
    /**
     * Make each of the trees empty, just in case it was set before.
     * Then load the tree from an input file.
     * */
    wordsTree.makeEmpty();
    load_to_tree(wordsTree, input_file);
    std::cout << "Words Tree successfully loaded!\n";

    personsTree.makeEmpty();
    load_to_tree(personsTree, input_file);
    std::cout << "Persons Tree successfully loaded!\n";

    organizationsTree.makeEmpty();
    load_to_tree(organizationsTree, input_file);
    std::cout << "Organizations Tree successfully loaded!\n";

}

void IndexHandler::update_words_tree(string word, string id, size_t frequency) 
{
    if(wordsTree.containsKey(word)) // word already in tree
    {
        wordsTree.add_value_map(word, id, frequency);
    }
    else // word not in tree 
    {  
        // create new node and insert into the tree 
        unordered_map<string, size_t> newDoc;
        newDoc.insert({id, frequency});

        MapValue<string, unordered_map<string, size_t>> newNode(word, newDoc);
        wordsTree.insert(newNode); 
    }
}

bool IndexHandler::checkWord(string word, int type)
{
    if(type == 0) // if word 
    {
        return wordsTree.containsKey(word);
    }
    else if(type == 1) // if org 
    {
        return organizationsTree.containsKey(word);
    }
    else // if person 
        return personsTree.containsKey(word);
}

void IndexHandler::update_persons_tree(string person, string id, size_t frequency)
{
    if(personsTree.containsKey(person)) // person already in tree
    {
        personsTree.add_value_map(person, id, frequency);
    }
    else // person not in tree 
    {
        unordered_map<string, size_t> newDoc;
        newDoc.insert({id, frequency});

        MapValue<string, unordered_map<string, size_t>> newNode(person, newDoc);
        personsTree.insert(newNode);
    }
}
void IndexHandler::update_organizations_tree(string org, string id, size_t frequency)
{
    if(organizationsTree.containsKey(org)) // org already in tree 
    {
        organizationsTree.add_value_map(org, id, frequency);
    }
    else // org not in tree
    {
        unordered_map<string, size_t> newDoc;
        newDoc.insert({id, frequency});

        MapValue<string, unordered_map<string, size_t>> newNode(org, newDoc);
        organizationsTree.insert(newNode);
    }
}
//Get the number of words terms in the words search tree.
size_t IndexHandler::get_num_words()
{
    return wordsTree.getSize();
}
//Get the number of person terms in the persons search tree.
size_t IndexHandler::get_num_persons()
{
    return personsTree.getSize();
}
//Get the number of organization terms in the organizations search tree.
size_t IndexHandler::get_num_orgs()
{
    return organizationsTree.getSize();
}
//Prints the words, persons, and organizations tree in order for testing.
void IndexHandler::printInfo(std::ostream& out)
{
    // print words
    out << "WORDS" << "\n";
    wordsTree.printTreeWithValues(out);

    // print persons
    out << "PERSONS" << "\n";
    personsTree.printTreeWithValues(out);

    // print orgs 
    out << "ORGANIZATIONS" << "\n";
    organizationsTree.printTreeWithValues(out);
}

void IndexHandler::reset() {
    //Resets the words, persons, and organizations tree to size = 0.
    wordsTree.makeEmpty();
    personsTree.makeEmpty();
    organizationsTree.makeEmpty();
}

size_t IndexHandler::checkWords(string word, string id) // returns the frequency for the given id in the word 
{
    return wordsTree.getFreq(word, id);
}

size_t IndexHandler::checkPerson(string word, string id) // returns the frequency for the given id in the word 
{
    return personsTree.getFreq(word, id);
}

size_t IndexHandler::checkOrg(string word, string id) // returns the frequency for the given id in the word 
{
    return organizationsTree.getFreq(word, id);
}