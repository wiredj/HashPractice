//
// Created by Jason on 11/16/17.
//
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#ifndef HASHPRACTICE_HASHPRACTICE_H
#define HASHPRACTICE_HASHPRACTICE_H
#endif //HASHPRACTICE_HASHPRACTICE_H

class Hash {

private:
    int size;
    int hashingValue;
    int insertCount = 0;
    struct kvPair{//Initializes key to -1 and value to blank.
        int key = -1;
        string value = "";
        ~kvPair(){std::cout << "Deleting array.\n";}
    };

    kvPair* buckets = 0;

public:
    Hash();

    ~Hash();



    void defaultValues();
    void createArray();
    void resizeArray();
    void cleanAndInsert(string insertString);
    void hashInsert (int insertKey, string insertValue);
    void hashSearch (int searchValue);
    void mainMenu();
    void branchMenu();
    void insertMenu();
    void fileMenu();
    void searchMenu();
    void cleanUp();
    void printArray();
};

Hash::Hash(){}
Hash::~Hash(){std::cout << "Deleting Hash object.\n";}

    void Hash::createArray (){
        kvPair* buckets = 0;
        buckets = new kvPair[this->size];
        this->buckets = buckets;
    }

    void Hash::resizeArray() {
        kvPair* tempBuckets;
        int oldSize = this->size;

        //Set new array size and hashing value.
        this->hashingValue = this->size*2;
        this->size = this->size*2 + 1;

        //Message to user
        std::cout << "Resizing array from " << oldSize << "." << std::endl;
        std::cout << "New size = " << this->size << ". New hash =  " << this->hashingValue << "." << std::endl;

        //Copy hash array to temporary array, delete hash array, create new larger hash array, and reset the count.
        tempBuckets = this->buckets;
        this->buckets = new kvPair[this->size];
        this->insertCount = 0;

        //Hash each value from the temp array into the new larger array with the new hash value.
        for (int i = 0; i < oldSize; i++){
            if (tempBuckets[i].key != -1) {
                this->hashInsert(tempBuckets[i].key, tempBuckets[i].value);
            }
        }
        //Clean-up
        delete tempBuckets;
    }

    void Hash::cleanAndInsert(string insertString) {
        stringstream ss(insertString);
        int key;
        string value;

        //Checks if first character is a number and if there is a space.
        if (isdigit(insertString.at(0)) and insertString.find(" ") != -1) {
            ss >> key >> value;
            this->hashInsert(key, value);
        }
        else if (toupper(insertString.at(0)) != 'R') std::cout << "Invalid entry." << std::endl;
    }

    void Hash::hashInsert(int insertKey, string insertValue){
        int hashLocation = insertKey % this->hashingValue; //Basic modulo hashing function
        if (hashLocation > this->size-1) hashLocation = this->size-1; //Prevents attempts to insert past the end of array.
        int firstLocation = hashLocation; //Record first location in case collisions cause a full traversal.  Shouldn't be possible.

        //Loops until an empty location is found (key of -1)
        while  (this->buckets[hashLocation].key != -1 and hashLocation != firstLocation - 1) {
            if (hashLocation < this->size - 1){
                std::cout << "Collision at index " << hashLocation << ". Attempting index " << hashLocation + 1 << std::endl;
                hashLocation++;
            }
            else {
                std::cout << "Collision at index " << hashLocation << ". Attempting index 0" << std::endl;
                hashLocation = 0;
            }
        }
        std::cout << "Inserted at index " << hashLocation << std::endl;
        buckets[hashLocation].key = insertKey;
        buckets[hashLocation].value = insertValue;

        this->insertCount++;

        //Resize if the array is greater than half full.
        if (this->insertCount * 2 > this->size){
            std::cout << "Array is more than half full. (" << this->insertCount << " out of " << this->size << ")." << std::endl;
            this->resizeArray();
        }
    }

    void Hash::hashSearch(int searchValue){
        int index = searchValue % this->hashingValue; //Basic modulo hashing function
        if (index > this->size-1) index = this->size-1; //Prevents attempts to search past the end of array.
        int firstLocation = index; //Record first location in case collisions cause a full traversal.  Shouldn't be possible.
        int key = this->buckets[index].key;

        //Loops until value is found or empty bucket is found.
        while  (key != searchValue and index != firstLocation - 1) {
            if (index < this->size) {
                if (this->buckets[index].key != -1) {
                    std::cout << "Key not found at index " << index << ". Checking index " << index + 1
                              << std::endl;
                    index++;
                }
                else break;  //Exit loop early if empty bucket is found to prevent extraneous messages.
            }
            else {
                std::cout << "Key not found at index " << index << ". Checking index 0" << std::endl;
                index = 0;
            }
            key = this->buckets[index].key; //Get key at next index.
        }
        if (key == searchValue) { //Success!
            std::cout << "Index: " << index << " Key: " << key << " Value: " << this->buckets[index].value << std::endl;
        }
        else { //Failure :(
            std::cout << "Key: " << searchValue << " not found in the array." << std::endl;
        }
    }

    void Hash::defaultValues(){
        this->size = 101;
        this->hashingValue = 100;
    }

    void Hash::mainMenu(){ //Initial menu that gets array size and hashing value.
        string input;
        int size = -1;
        int hashingValue;
        std::cout << "     Welcome to HashPractice.     " << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << "Enter array size (0 for default values): ";

        while (size < 0) { //Loop until a valid size is entered.
            getline(cin, input);
            stringstream(input) >> size;
            if (size == 0)
                this->defaultValues();
            else if (size > 0) {
                this->size = size;
                std::cout << "Enter hashing value: ";
                getline(cin, input);
                stringstream(input) >> hashingValue;
                this->hashingValue = hashingValue;
            } else
                std::cout << "Please enter a positive integer for array size (0 for default values): ";
        }
        std::cout << "Array size is " << this->size << std::endl;
        std::cout << "Hashing value is " << this->hashingValue << std::endl;
    }

    void Hash::branchMenu() { //Menu with 5 options for display, insert, input file, search, and exit.
        string input;
        char choice = '0';

        while (true) { //Loops until Exit option is chosen.
            std::cout << "\nWhat would you like to do next?" << std::endl;
            std::cout
                    << "Display hash table: D || Insert key and value: I || Input file path: F || Search for a key: S || Exit: E" << std::endl;
            getline(cin, input);
            choice = input.at(0);
            choice = toupper(choice);

            if      (choice == 'D')   {printArray();}
            else if (choice == 'I')   {insertMenu();}
            else if (choice == 'F')   {fileMenu();}
            else if (choice == 'S')   {searchMenu();}
            else if (choice == 'E')   {cleanUp();
                                        return;}
            else {std::cout << "Invalid input." << std::endl;}

            choice = '0';
        }
    }

    void Hash::printArray() {
        std::cout << "Displaying full hash table." << std::endl;
        for (int i = 0; i < this->size; i++){
            std::cout << "Index: " << i << " Key: " << this->buckets[i].key << " Value: " << this->buckets[i].value <<std::endl;
        }
    }
    void Hash::insertMenu() { //Accepts values entered in "key value" format.
        string input = "";
        char choice = '0';

        while (choice != 'R') {
            std::cout << "Enter a positive integer key and a string value (e.g. 39 Dog) or enter R to return." <<std::endl;
            getline(cin, input);
            choice = toupper(input.at(0));
            std::stringstream ss(input);

            cleanAndInsert(input);
        }
    }

    void Hash::fileMenu() { //Menu for inputting a file name
        string pathInput;
        string line;
        int key;
        string value;
        char choice = '0';
        ifstream inputFile;

        while (choice != 'R') { //Loops until R is entered.
            std::cout << "Enter a valid file path (e.g. C:\\stuff\\testfile.txt) or enter R to return." << std::endl;
            getline(cin, pathInput);
            choice = toupper(pathInput.at(0));

            if (choice == 'R') break;

            inputFile.open(pathInput); //Attempt to open file.

            if (inputFile.is_open()){ //Read from file if open, display error if not.
                while (getline(inputFile,line)){
                    cleanAndInsert(line);
                }
            }
            else std::cout << "Unable to open file." << std::endl;
        }
    }

    void Hash::searchMenu(){ //Menu for inputting search keys.
        string input = "";
        char choice = '0';
        int key = -1;

        while (choice != 'R'){ //Loops until R is entered
            std::cout << "Enter a positive integer key to search for or enter R to return." << std::endl;
            getline(cin, input);
            choice = toupper(input.at(0));

            if (isdigit(choice)){ //Checks if first character is a number before attempting to convert it to an int.
                key = stoi(input,nullptr);
                if (key >= 0) this->hashSearch(key);
                else std::cout << "Invalid entry." << std::endl;
            }
            else if (choice != 'R') std::cout << "Invalid entry." << std::endl;

            key = -1;
        }
    }

    void Hash::cleanUp(){ //Clean-up.  Deletes array to prevent memory leak.
        delete buckets;
    }















