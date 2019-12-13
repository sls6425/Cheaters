//
// Created by Savannah Schmidt on 12/6/2019.
//

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include "hash.h"
#include <fstream>
#include <queue>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>

using namespace std;

struct collideTotals{
    int file1;
    int file2;
    int collisions;
};

//ALL OF THIS IS FROM PRIEBE
/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

//strips essay into chunks, hashes the tokens
void stripEssay(int **collisions, HashTable &hashTable, int chunkSize, ifstream &file, int fileNumber){
    string chunk[chunkSize];
    string nextWord;
    int word0 = chunkSize - 1;
    //skipping spaces
    for(int i = 0; i < (chunkSize - 1); i++){
        if(file >> nextWord){
            chunk[i] = nextWord;
        }
        else{
            return;
        }
    }
    
    while(file >> nextWord){
        chunk[word0] = nextWord;
        word0 = ((word0 + 1) % chunkSize);
        string hashChunk;
        for(int i = word0; i < (word0 + chunkSize); i++){
            hashChunk += chunk[i % chunkSize];
            hashChunk += " ";
        }
        vector<int> response = hashTable.addNode(hashChunk, fileNumber);
        for(int j = 0; j < response.size(); j++){
            collisions[response[j]][fileNumber]++;
        }
    }

}



int main(int argc, char* argv[]) {

    //command line input
    int chunkSize = atoi(argv[2]);
    int numberSimilarities = atoi(argv[3]);

    //FROM PRIEBE
    string dir = string(argv[1]);
    vector<string> files = vector<string>();
    getdir(dir,files);
    int **collisions = new int* [files.size()];
    for (unsigned int i = 0;i < files.size();i++) {
        collisions[i] = new int[files.size()];
        for(int j = 0; j < files.size(); j++){
            collisions[i][j] = 0;
        }
    }

    //create hash table
    HashTable hashTable;
    //for every file in directory
    //collect: file name, file contents, then strip/chunk file contents
    //chunks get hashed in stripEssay()
    for(int i = 2; i < files.size(); i++){
        string fileName = dir + "/" + files[i];
        ifstream file(fileName.c_str());
        stripEssay(collisions, hashTable, chunkSize, file, i);
        file.close();
    }

    //creating array of collision structs (defined at beginning of main)
    collideTotals totals[files.size() * files.size()];
    for(int i = 0; i < files.size(); i++){
        for(int j = 0; j < files.size(); j++){
            //border cases of array
            if(i < 2 || j < 2 || i >= j){
                totals[i *files.size() + j].collisions = -10000; // place holder value
            }
            else{
                //assign each index file pair and their individual collision total
                totals[i *files.size() + j].file1 = i;
                totals[i *files.size() + j].file2 = j;
                totals[i *files.size() + j].collisions = collisions[i][j];
            }
        }
    }
    //ordering array in descending order of total collision numbers
    //so that for output loop dont have to worry about ordering
    for(int i = 0; i < (files.size()*files.size()); i++) {
        for (int j = 0; j < ((files.size() * files.size()) - 1); j++) {
            if(totals[j].collisions < totals[j + 1].collisions){
		//swapping structs positions in array
                collideTotals temp = totals[j];
                totals[j] = totals[j+1];
                totals[j+1] = temp;
            }
        }
    }
    //final step: output
    //loop through matrix of collision data
    //print if index had a reported collision total greater than desired
    for(int i = 0; i < (files.size()*files.size()); i++){
        if(totals[i].collisions > numberSimilarities){
            cout << totals[i].collisions << ": " << files[totals[i].file1] << "., " << files[totals[i].file2] << endl;
        }
        else break;
    }
    return 0;
}
