#pragma once
#include <string>
#include <vector>

using KeyType = std::string;
using ValueType = double;

class Map
{
private:
    struct Node {
        KeyType key;
        Node* left;
        Node* right;
        ValueType val;

        Node(KeyType k, ValueType v) :left(nullptr), right(nullptr) {
            key = k;
            val = v;
        }
    };

    struct BST {
        Node* root;

        BST() :root(nullptr) {}

        BST(const BST& rhs);
        void auxCopyConstructor(Node*& lhsN, Node* rhsN);

        //Not needed so not implemented in this project
        BST& operator=(const BST& rhs);
        void auxAssignmentOperator(Node*& lhs, Node* rhs);

        ~BST();
        void auxDestructor(Node* currentNode); //same as: remove_entire_tree


        bool add(KeyType key, ValueType val);
        bool auxAdd(KeyType key, ValueType val, Node*& currentNode);

        Node* findNode(KeyType key);
        Node* auxFindNode(KeyType k, Node* currentNode);


        Node* findIndex(int& i, KeyType& key, ValueType& value, Node* currentNode) const; //aux_get_index

        bool removeNode(const KeyType& key, Node*& currentNode);
        Node* auxRemoveNodeTwoChildrenCase(Node*& n, Node*& parent);
    };

    struct HashTable {
        const int totalSize = 100; //would've been better to make this static but oh well.
        std::vector<BST*> list;

        HashTable() :list(totalSize, nullptr) {}
        ~HashTable();
        //Not needed so not implemented in this project
        HashTable& operator=(const HashTable& rhs);

        int Convert_Key(std::string key) const;
        int Convert_Key(double key) const;
        int hash(KeyType key) const;
    };

    int totalValues;
    HashTable hashTable;

public:
    // Create an empty map (i.e., one with no key/value pairs)
    Map() : totalValues(0) {}

    //Copy Constructor
    Map(const Map& other);

    //Assignment Operator
    const Map& operator=(const Map& rhs);

    //Destructor
    ~Map();

    bool empty() const; // Return true if the map is empty, otherwise false.
    int size() const;   // Return the number of key/value pairs in the map.

    bool insert(const KeyType& key, const ValueType& value);
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map).
    bool update(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map then add it and 
    // return true. In fact this function always returns true.
    bool erase(const KeyType& key);
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    bool contains(const KeyType& key) const;
    // Return true if key is equal to a key currently in the map, otherwise
    // false.
    bool get(const KeyType& key, ValueType& value) const;
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    bool get(int i, KeyType& key, ValueType& value) const;
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.  (See below for details about this function.)
    void swap(Map& other);
    // Exchange the contents of this map with the other one.
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);