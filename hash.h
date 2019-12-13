//
// Created by Savannah Schmidt on 12/6/2019.
//

#ifndef PLAGIARISMCHECKER_HASH_H
#define PLAGIARISMCHECKER_HASH_H
#include <string>
#include <vector>

using namespace std;


class HashTable{
private:
    //256 possible ascii values
    static const int ASCII_CONVERSION = 256;

    //create node just like linked list
    struct hashNode{
        int fileNumber;
        hashNode *next;
    };

    vector<hashNode*> nodes;

   /*
 *   small: 33946 words
 *   med: 91273 words
 *   big: 1615193 words
 *
 *   load factor of about 0.7?
 *   keep size prime number
 *
  */
    static const int HASH_SIZE = 157037;
    hashNode* table[HASH_SIZE];
    int hash(string word);
public:
    vector<int> addNode(string word, int fileNumber);
    HashTable();
    ~HashTable();
};

//default constructor
HashTable::HashTable(){
    //set up hash table with nothing
    for(int i = 0; i < HASH_SIZE; i++){
        table[i] = NULL;
    }
}

//hash function
int HashTable::hash(string word){
    char letter;
    int val = 0;
    for(int i = 0; i < word.length(); i++){
        letter = word.at(i);
        //add up ascii values in chunk
        if(int(letter) > 0){
            val *= ASCII_CONVERSION;
            val += int(letter);
            val %= HASH_SIZE;
        }
    }
    return val;
}

//add in new key and determine if collision or not
vector<int> HashTable::addNode(string word, int fileNumber) {
    vector<int> collisions;
    hashNode *node = new hashNode;
    node->fileNumber = fileNumber;
    node->next = NULL;
    int hash = this->hash(word);
    //if trying to add, and spot is empty, add (no collision)
    if(table[hash] == NULL){
        table[hash] = node;
    }
    else{
        //COLLISION!!! CHEATING!!!!
        hashNode *collide = table[hash];
        collisions.push_back(collide->fileNumber);
        while(collide->next != NULL){
            collide = collide->next;
            collisions.push_back(collide->fileNumber);
        }
        collide->next = node;
    }
    nodes.push_back(node);
    return collisions;
}

//destructor
HashTable::~HashTable(){
    for(int i = (nodes.size() - 1); i > 0; i--){
        delete nodes[i];
    }
}

#endif //PLAGIARISMCHECKER_HASH_H
