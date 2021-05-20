#include <iostream>
#include <bits/stdc++.h>
#include <queue>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>
#include "trie.cpp" // contains trie class

using namespace std;
using namespace std::chrono;

void startsWith(trie *root, string &text, vector<int> &indexOfWords)
{ // function that enables ~startwith feature
    trie obj;
    trie *parent = root;
    char c;
    for (int i = 0; i < text.length(); i++)
    {                                  // loop to iterate through the input sequence of characters from the user
        c = text[i];                   // stores the ith character in input string
        if (parent->child.count(c))    // checks if the child of parent node exists
            parent = parent->child[c]; // if child node exists, the child node is made the parent node

    }                                              // finish processing the input sequence of characters
    if (parent->endOfWord >= 0)                    // now it checks if the given sequence of characters is a word itself
        indexOfWords.push_back(parent->endOfWord); // if yes it pushes into the vector

    recStartsWith(indexOfWords, parent); // calls recursive function to find more words that start with the given sequence of characters
}

void recStartsWith(vector<int> &indexOfWords, trie *parent)
{ // recursive function to look for children nodes of nodes corresponding to input sequence of characters push them into the vector
    for (auto &it : parent->child)
    {                                                     // iterates through all the children of a corresponding parent node to find all possible words
        if (it.second->endOfWord >= 0)                    // checks if a particular node marks the end of the word
            indexOfWords.push_back(it.second->endOfWord); // if the node marks the end of the push it into the vector

        recStartsWith(indexOfWords, it.second); // calling the function on the current child node to look for their child nodes
    }
}

void createAutomaton(trie *root, vector<string> &listOfWords)
{             // this function is used to create the automaton for the Aho-Corasick algorithm
              // the primary objective of the function is to initialise the trie with the list of words and create it's corresponding failure and dictionary links
    trie obj; // using object of trie class to call functions from the trie class

    obj.trieCreation(root, listOfWords); // calling function to create trie
    obj.failAndDictLinks(root);          // calling function to create failure and dictionary links
}

void findWords(trie *root, string &text, vector<vector<int>> &indexOfWords)
{                        // function to look for words in the sequence of characters
    trie obj;            // object of class
    trie *parent = root; // parent node pointing to root to traverse through trie

    for (int i = 0; i < text.length(); i++)
    { // starts from first character of sequence and goes on till the end of the sequence until all characters are covered
        char c = text[i];
        if (parent->child.count(c)) // looks for the child of the parent node with character c
        {
            parent = parent->child[c]; // parent goes to the child node which contains character c in that branch

            if (parent->endOfWord >= 0) // if that node marks the end of a pattern, the index of the word is pushed into the final vector
                indexOfWords[parent->endOfWord].push_back(i);
            // after checking it for end of a word, we follow the dictionary links to see if it matches any other pattern within the sequence of characters
            trie *temp = parent->dictionaryLinks;
            while (temp)
            {
                indexOfWords[temp->endOfWord].push_back(i); // if it marks the end of the word, it gets pushed into the vector
                temp = temp->dictionaryLinks;               // moving to the next dictionary link
            }
        }

        else
        { // if prefix does not exist, we follow the failure links to find other prefixes to match the pattern
            while (parent != root && parent->child.count(c) == 0)
                parent = parent->failureLinks;

            if (parent->child.count(c))
                i--;
        }
    }
}

bool getFileContent(string fileName, vector<string> &vecOfStrs)
{ // function to initialze words in the file to a vector for creation of trie
    ifstream in(fileName.c_str());
    if (!in)
    { // checks for error in opening the file
        cerr << "Cannot open the File : " << fileName << endl;
        return false;
    }

    string str;
    while (getline(in, str))
    {                       // it checks characters line by line and pushes it into the vector
        if (str.size() > 2) //it pushes characters of only length > 2 to avoid spam of random words
            vecOfStrs.push_back(str);
    }

    in.close(); // closing the file after creating vector
    return true;
}

void CLI(trie *root, vector<string> &listOfWords)
{
    // functions for the CLI interface
    cout << "Command list:" << endl;
    cout << "To find possible words within the given sequence of characters. - ~substring" << endl;
    cout << "To find possible words which starts with the given set of characters. - ~startwith" << endl;
    cout << "End - ~end" << endl;

    int k = 143029; // number of words in the list
    string text;    // variable to store input sequence of characters

    string check;
    cout << "What would you like to do?" << endl;
    cin >> check;
    int ch = -1;
    transform(check.begin(), check.end(), check.begin(), ::tolower);

    //to convert commands into integers for using in switch case
    if (check.compare("~substring") == 0)
        ch = 0;
    else if (check.compare("~startwith") == 0)
        ch = 1;
    else if (check.compare("~end") == 0)
        ch = 2;

    bool yes = false;

    while (ch != 2)
    { // starting command execution
        switch (ch)
        {
        case 0:
        {
            // case for finding substrings within the input sequence of characters
            vector<vector<int>> indexOfWords(k, vector<int>()); // creating vector of a vector to store final set of output words
            cout << "\nEnter sequence of characters to look for words:" << endl;
            cin >> text;
            //converting text to lower case
            transform(text.begin(), text.end(), text.begin(), ::tolower);

            auto start = high_resolution_clock::now(); // start time to find the words
            //finding words from the sequence of characters
            findWords(root, text, indexOfWords);

            cout << "\nList of all possible words from the given sequence of characters:" << endl;

            yes = false;
            for (int i = 0; i < k; i++)
            { // printing words which matched
                if (indexOfWords[i].size())
                {
                    cout << listOfWords[i] << endl;
                    yes = true;
                }
            }

            if (!yes) // no words found
                cout << "Sorry! No words could be found with the sequence of characters input by you." << endl;

            cout << endl;

            auto stop = high_resolution_clock::now();                  // stop time to find words
            auto duration = duration_cast<milliseconds>(stop - start); // calculating duration
            cout << "Time taken to find words: " << duration.count() << "ms.\n"
                 << endl; // printing time taken
        }
        break;

        case 1:
        {
            // case for finding characters that start with input sequence of characters
            vector<int> start; // vector to store set of characters

            cout << "\nEnter set of characters to look for words:" << endl;
            cin >> text;
            // converting it into lower case
            transform(text.begin(), text.end(), text.begin(), ::tolower);

            auto star = high_resolution_clock::now(); // start time taken to execute
            // calling function to start
            startsWith(root, text, start);
            cout << "\nSet of words that start with:" << text << endl;
            yes = false;

            for (int i = 0; i < start.size(); i++)
            { // printing list of words
                yes = true;
                cout << listOfWords[start[i]] << endl;
            }

            if (!yes) // now words found
                cout << "Sorry! No words could be found with the set of characters input by you." << endl;

            cout << endl;
            auto stop = high_resolution_clock::now();                 // stop time
            auto duration = duration_cast<milliseconds>(stop - star); // calculating duration taken
            cout << "Time taken to find words: " << duration.count() << "ms.\n"
                 << endl; // printing out the time
        }
        break;

        case 2:
        { // ending program execution
            cout << "Hope this served your purpose!" << endl;
        }
        break;

        default: // if incorrect command typed
            cout << "\nYou typed in an incorrect option. Please try again.\n"
                 << endl;
        }

        // repeating command list
        ch = -1;
        cout << "Command list:" << endl;
        cout << "To find possible words within the given sequence of characters. - ~substring" << endl;
        cout << "To find possible words which starts with the given set of characters. - ~startwith" << endl;
        cout << "End - ~end" << endl;

        string check;
        cout << "What would you like to do?" << endl;
        cin >> check;

        transform(check.begin(), check.end(), check.begin(), ::tolower);
        if (check.compare("~substring") == 0)
            ch = 0;
        else if (check.compare("~startwith") == 0)
            ch = 1;
        else if (check.compare("~end") == 0)
            ch = 2;
    }
}

int main()
{
    vector<string> listOfWords; // vector to store set of words for creating a trie

    bool result = getFileContent("wordlist.txt", listOfWords); // initializing the words into a trie

    trie obj;
    trie *root = obj.newNode(); // creating a new node for root

    auto start = high_resolution_clock::now();
    createAutomaton(root, listOfWords); //creating the automaton
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Time taken to create automaton: " << duration.count() << "s.\n"
         << endl;

    CLI(root, listOfWords); // calling the list of commands that is to be executed

    // Code is good
    cout << "\nThank you!" << endl;
    return 0;
}
