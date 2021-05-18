#include <map>
#include <vector>
#include <string>
#include <queue>

using namespace std;

// this represents the functions of a trie
class trie
{
public: // public data members
    map<char, trie *> child;
    trie *failureLinks;
    trie *dictionaryLinks;
    int endOfWord;

public:             //public member functions
    trie *newNode() //function to create a node
    {
        trie *temp = new trie;
        temp->failureLinks = nullptr;
        temp->dictionaryLinks = nullptr;
        temp->endOfWord = -1;

        return temp;
    }

    void trieCreation(trie *root, vector<string> &listOfWords)
    { //function to create the trie

        for (int i = 0; i < listOfWords.size(); i++)
        {
            trie *current = root;
            string word = listOfWords[i];
            for (int j = 0; j < word.length(); j++)
            {

                char letter = word[j];
                if (current->child.count(letter))
                    current = current->child[letter];
                else
                {
                    trie *addAlpha = newNode();
                    current->child.insert({letter, addAlpha});
                    current = addAlpha;
                }
            }
            current->endOfWord = i;
        }
    }

    void failAndDictLinks(trie *root)
    {   // function to create dictionary and failure links within the trie
        // here we use breadth first search to navigate to each child to create the failure links
        root->failureLinks = root;
        // BFS is implemented using queue and here we are using the cpp header file called queue
        queue<trie *> q;

        //creating failure link for each node
        for (auto &it : root->child)
        {
            q.push(it.second);
            it.second->failureLinks = root;
        }

        while (q.size())
        {
            trie *presentState = q.front();
            q.pop();

            for (auto &it : presentState->child)
            {
                char c = it.first;
                trie *temp = presentState->failureLinks;

                while (temp->child.count(c) == 0 && temp != root)
                    temp = temp->failureLinks;

                if (temp->child.count(c))
                    it.second->failureLinks = temp->child[c];
                else
                    it.second->failureLinks = root;

                q.push(it.second);
            }

            //creating dictionary link for each node
            if (presentState->failureLinks->endOfWord >= 0)
                presentState->dictionaryLinks = presentState->failureLinks;
            else
                presentState->dictionaryLinks = presentState->failureLinks->dictionaryLinks;
        }
    }
};
