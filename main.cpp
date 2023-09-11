// Class Owner: Marc

#include <iostream>
#include <chrono>
#include <string>
#include "SearchEngine.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "Welcome to the Super Searcher!\n";
    //Initialize the Search Engine.
    SearchEngine searchengine;
    
    //File path for the persistence file.
    string filePath = "saved_tree_file";

    //Asks the user to enter a letter input corresponding with a task.
    std::string user_input = "wrong string";
    while(user_input != "e") {
        cout << "Type one of the following letters to choose an option.\n";
        cout << "\ta. Create a new Index for all the files.\n";
        cout << "\tb. Load a previously used Index from a file " << filePath << ".\n";
        cout << "\tc. Save the currently created Index into a file " << filePath << ".\n";
        cout << "\td. Make a new query to search with the Search Engine.\n";
        cout << "\te. Terminate the Super Searcher program.\n";
        
        cout << "Input a Letter: ";
        getline(std::cin, user_input, '\n');
        //Keep asking the user for input until it is a valid input.
        while(user_input.size() != 1 || user_input[0] > 'e' || user_input[0] < 'a') {
            std::cout << "Invalid Input, Choose a Letter from a to e: ";
            getline(std::cin, user_input, '\n');
        }
        cout << "\n";

        //Start timing how long the task takes.
        auto start = chrono::high_resolution_clock::now();

        //Depending on the input, perform different tasks.
        if(user_input == "a") {
            searchengine.process_documents(argv[1]);
        } else if (user_input == "b") {
            searchengine.load_index_to_file(filePath);
        } else if (user_input == "c") {
            searchengine.save_index_to_file(filePath);
        } else if (user_input == "d") {
            searchengine.new_query();
        } else if(user_input == "e") {
            cout << "Thank you for using the Super Searcher!\n";
            break;
        } else {
            continue;
        }

        //Output the time take to run the last task.
        auto end = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end-start);
        cout << "The last task took a total of " << elapsed.count() << " milliseconds.\n\n";
    }
    return 0;
}
