//============================================================================
// Name        : HashTable.cpp
// Author      : John Watson & Andrew Biggs(version 2)
// Version     : 2
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hash Table in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>
#include <vector>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        unsigned int size;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
            bid.bidId = -1;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node*> buckets;
    
    unsigned int tableSize = 0;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string searchId);
    void displayBid(Bid bid);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    for (int i = 0; i < DEFAULT_SIZE; ++i) {
        buckets.push_back(new Node);
        tableSize++;
    }
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    for (int i = 0; i < size; ++i) {
        buckets[i] = new Node;
        tableSize++;
    }
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    for (int i = 0; i < sizeof(buckets); ++i) {
        Node* current = buckets[i];
        Node* temp;
        
        // loop over each node, detach from list then delete
        while (current != nullptr) {
            temp = current; // hang on to current node
            current = current->next; // make current the next node
            delete temp; // delete the orphan node
        }
    }
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    return key % 179;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    /// Uses hash to find the appropriate bucket based on bidId from Bid parameter
    /// Traverse nodes in the linked list in the bucket until a node's next is nullptr
    /// Make the last node in the list point to new node containing Bid parameter information
    int hashKey = hash(stoi(bid.bidId));
    Node* newNode = new Node(bid, hashKey);
    Node* curNode = buckets[hashKey];
    while (curNode->next != nullptr)
        curNode = curNode->next;
    
    curNode->next = newNode;
    buckets[hashKey]->size++;
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    /// Loop over each bucket in vector. Output bucket number.
    /// Traverse the linked list in each bucket and output info for each bid in bucket.
    for (int i = 0; i < tableSize; ++i) {
        Node* curNode = buckets[i];
        cout << "Bucket " << i << ": " << endl;
        while (curNode != NULL) {
            if (curNode->key != UINT_MAX) {
                displayBid(curNode->bid);
            }
            curNode = curNode->next;
        }
    }
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    /// Get hashKey from passed parameter, set curNode = index of the appropriate bucket containing hashKey
    /// Traverse linked list in bucket until a match is found or curNode->next is nullptr, keeping track of the most recent 2 nodes visited
    /// If a match is found, make the previous node's next Node pointer instead point to the Node following curNode
    int hashKey = hash(stoi(bidId));
    Node* curNode = buckets[hashKey];
    Node* prevNode;
    while (bidId != curNode->bid.bidId && curNode->next != nullptr) {
        prevNode = curNode;
        curNode = curNode->next;
    }
    if (bidId == curNode->bid.bidId) {
        prevNode->next = curNode->next;
        cout << "Bid removed." << endl;
    }
    else
        cout << "Bid not found." << endl;
}

/**
 * Search for the specified bidId
 *
 * @param searchId The bid id to search for
 */
Bid HashTable::Search(string searchId) {
    ///
    Bid bid;
    int hashKey = hash(stoi(searchId));
    Node* curNode = buckets[hashKey];
    if (buckets[hashKey]->next == nullptr)
        return bid;
    
    while (curNode != NULL) {
        if (curNode->bid.bidId == searchId)
            return curNode->bid;
        curNode = curNode->next;
    }
    return bid;
}

/**
 * Function to reduce redundant code
 *
 * @param bid The bid whose information to display
 */
void HashTable::displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey, removalKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    case 4:
        
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();
    
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            cin >> bidKey;
                
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            cin >> bidKey;
            bidTable->Remove(bidKey);
            break;
        
        case 9:
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
