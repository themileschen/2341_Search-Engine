#ifndef IndexHandler_H
#define IndexHandler_H

#include "AVLTree.h"
#include "MapValue.h"

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;
/**
 * @brief Index Handler
 */
class IndexHandler {
private:
    //AVL Tree with word search terms, which documents they appear in, and the frequency at which they appear in those documents.
    AVLTree<MapValue<string, unordered_map<string, size_t>>, string, unordered_map<string, size_t>> wordsTree;
    //AVL Tree with organization search terms, which documents they appear in, and the frequency at which they appear in those documents.
    AVLTree<MapValue<string, unordered_map<string, size_t>>, string, unordered_map<string, size_t>> organizationsTree;
    //AVL Tree with person search terms, which documents they appear in, and the frequency at which they appear in those documents.
    AVLTree<MapValue<string, unordered_map<string, size_t>>, string, unordered_map<string, size_t>> personsTree;

    /**
     * Based on a given input file, load the words in the file, the documents they are associated with,
     * and the frequency of the words in those documents to the given AVL Tree.
    */
    void load_to_tree( AVLTree<MapValue<string, unordered_map<string, size_t>>, string, unordered_map<string, size_t>>& tree, std::ifstream& input_file);

public:
    IndexHandler();

    //Output the words, persons, and organizations trees in order to an output_file.
    void save_index_to_file(std::ofstream& output_file);
    /**
     * Make each of the trees empty, just in case it was set before.
     * Then load the tree from an input file.
     * */
    void load_index_to_file(std::ifstream& input_file);

    void update_words_tree(string word, string id, size_t frequency);
    void update_persons_tree(string word, string id, size_t frequency);
    void update_organizations_tree(string word, string id, size_t frequency);

    bool checkWord(string word, int);

    //Get the number of words terms in the words search tree.
    size_t get_num_words();
    //Get the number of person terms in the persons search tree.
    size_t get_num_persons();
    //Get the number of organization terms in the organizations search tree.
    size_t get_num_orgs();

    //Prints the words, persons, and organizations tree in order for testing.
    void printInfo(std::ostream& out = std::cout);

    //Resets the words, persons, and organizations tree to size = 0.
    void reset();
    //Checks if the sizes of the words, persons, and organizations trees are all 0
    bool are_trees_set() {
        return !wordsTree.isEmpty() && !personsTree.isEmpty() && !organizationsTree.isEmpty();
    }
    
    auto& get_documents_for_word(std::string word) {
        return wordsTree.returnValues(word);
    }
    auto& get_documents_for_organizations(std::string word) {
        return organizationsTree.returnValues(word);
    }
    auto& get_documents_for_persons(std::string word) {
        return personsTree.returnValues(word);
    }

    size_t checkWords(string, string);
    size_t checkPerson(string, string);
    size_t checkOrg(string, string);
};

#endif
