#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <map>
#include <bits/stdc++.h>
#include "trie.cpp"

using namespace std;

void recStartsWith(vector<int> &indexOfWords, trie *parent)
{
    for (auto &it : parent->child)
    {
        if (it.second->endOfWord >= 0)
            indexOfWords.push_back(it.second->endOfWord);

        recStartsWith(indexOfWords, it.second);
    }
}

void startsWith(trie *root, string &text, vector<int> &indexOfWords)
{
    trie obj;
    trie *parent = root;
    char c;
    for (int i = 0; i < text.length(); i++)
    {
        c = text[i];
        if (parent->child.count(c))
        {
            parent = parent->child[c];
        }
    }
    if (parent->endOfWord >= 0)
        indexOfWords.push_back(parent->endOfWord);

    recStartsWith(indexOfWords, parent);
}

void createAutomaton(trie *root, vector<string> &listOfWords)
{
    trie obj;
    obj.trieCreation(root, listOfWords);
    obj.failAndDictLinks(root);
}

void findWords(trie *root, string &text, vector<vector<int>> &indexOfWords)
{
    trie obj;
    trie *parent = root;

    for (int i = 0; i < text.length(); i++)
    {
        char c = text[i];
        if (parent->child.count(c))
        {
            parent = parent->child[c];

            if (parent->endOfWord >= 0)
                indexOfWords[parent->endOfWord].push_back(i);

            trie *temp = parent->dictionaryLinks;
            while (temp)
            {
                indexOfWords[temp->endOfWord].push_back(i);
                temp = temp->dictionaryLinks;
            }
        }

        else
        {
            while (parent != root && parent->child.count(c) == 0)
                parent = parent->failureLinks;

            if (parent->child.count(c))
                i--;
        }
    }
}

bool getFileContent(string fileName, vector<string> &vecOfStrs)
{
    ifstream in(fileName.c_str());
    if (!in)
    {
        cerr << "Cannot open the File : " << fileName << endl;
        return false;
    }

    string str;
    while (getline(in, str))
    {
        if (str.size() > 2)
            vecOfStrs.push_back(str);
    }

    in.close();
    return true;
}

int main()
{
    string text;
    vector<string> listOfWords;

    bool result = getFileContent("wordlist.txt", listOfWords);
    int k = 143091;

    trie obj;
    trie *root = obj.newNode();

    createAutomaton(root, listOfWords);

    cout << "Command list:" << endl;
    cout << "To find possible words within the given sequence of characters. - ~substring" << endl;
    cout << "To find possible words which starts with the given set of characters. - ~startwith" << endl;
    cout << "End - ~end" << endl;

    string check;
    cout << "What would you like to do?" << endl;
    cin >> check;
    int ch = -1;
    transform(check.begin(), check.end(), check.begin(), ::tolower);
    if (check.compare("~substring") == 0)
        ch = 0;
    else if (check.compare("~startwith") == 0)
        ch = 1;
    else if (check.compare("~end") == 0)
        ch = 2;

    bool yes = false;

    while (ch != 2)
    {
        switch (ch)
        {
        case 0:
        {
            vector<vector<int>> indexOfWords(k, vector<int>());
            cout << "\nEnter sequence of characters to look for words:" << endl;
            cin >> text;
            transform(check.begin(), check.end(), check.begin(), ::tolower);

            findWords(root, text, indexOfWords);

            cout << "\nList of all possible words from the given sequence of characters:" << endl;

            yes = false;
            for (int i = 0; i < k; i++)
            {
                if (indexOfWords[i].size())
                {
                    cout << listOfWords[i] << endl;
                    yes = true;
                }
            }

            if (!yes)
                cout << "Sorry! No words could be found with the sequence of characters input by you." << endl;

            cout << endl;
        }
        break;

        case 1:
        {
            vector<int> start;

            cout << "\nEnter set of characters to look for words:" << endl;
            cin >> text;
            transform(check.begin(), check.end(), check.begin(), ::tolower);

            startsWith(root, text, start);
            cout << "Set of words that start with:" << text << endl;
            yes = false;

            for (int i = 0; i < start.size(); i++)
            {
                yes = true;
                cout << listOfWords[start[i]] << endl;
            }

            if (!yes)
                cout << "Sorry! No words could be found with the set of characters input by you." << endl;

            cout << endl;
        }
        break;

        case 2:
        {
            cout << "Hope this served your purpose!" << endl;
        }
        break;

        default:
            cout << "\nYou typed in an incorrect option. Please try again.\n" << endl;
        }

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

    cout << "\nThank you!" << endl;
    return 0;
}
