#include <map>
#include <vector>
#include <string>

using namespace std;

class trie
{
public:
    map<char, trie *> child;
    trie *failureLinks;
    trie *dictionaryLinks;
    int endOfWord;

public:
    trie *newNode()
    {
        trie *temp = new trie;
        temp->failureLinks = nullptr;
        temp->dictionaryLinks = nullptr;
        temp->endOfWord = -1;

        return temp;
    }

    void trieCreation(trie *root, vector<string> &listOfWords)
    {

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
    {
        root->failureLinks = root;

        queue<trie *> q;

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

            if (presentState->failureLinks->endOfWord >= 0)
                presentState->dictionaryLinks = presentState->failureLinks;
            else
                presentState->dictionaryLinks = presentState->failureLinks->dictionaryLinks;
        }
    }
};
