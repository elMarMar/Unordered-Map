#include "Map.h"
#include <iostream>
#include <string>

using KeyType = std::string;
using ValueType = double;
// BST Class Functions:
// Function already declared in Map.h:
// BST();

Map::BST::BST(const BST& rhs)
{
    if (rhs.root == nullptr) {
        root = nullptr;
        return;
    }

    auxCopyConstructor(root, rhs.root);
}

void Map::BST::auxCopyConstructor(Node*& lhsN, Node* rhsN)
{
    if (rhsN == nullptr)
        return;

    lhsN = new Node(rhsN->key, rhsN->val);

    auxCopyConstructor(lhsN->left, rhsN->left);
    auxCopyConstructor(lhsN->right, rhsN->right);
}

/*
Map::BST& Map::BST::operator=(const BST& rhs) { return *this; }

void Map::BST::auxAssignmentOperator(Node*& lhs, Node* rhs) { }
*/

Map::BST::~BST()
{
    auxDestructor(root);
}

void Map::BST::auxDestructor(Node* currentNode)
{
    if (currentNode == nullptr)
        return;

    auxDestructor(currentNode->left);
    auxDestructor(currentNode->right);

    delete currentNode;
}

bool Map::BST::add(KeyType key, ValueType val)
{
    if (root == nullptr) {
        root = new Node(key, val);
        return true;
    }

    return auxAdd(key, val, root);
}

bool Map::BST::auxAdd(KeyType key, ValueType val, Node*& currentNode)
{
    if (currentNode == nullptr) {
        currentNode = new Node(key, val);
        return true;
    }

    // Left Traversal
    if (currentNode->key > key) {
        if (currentNode->left == nullptr) {
            currentNode->left = new Node(key, val);
            return false;
        }

        else {
            return auxAdd(key, val, currentNode->left);
        }
    }

    // Right Traversal
    else if (currentNode->key < key) {
        if (currentNode->right == nullptr) {
            currentNode->right = new Node(key, val);
            return false;
        }

        else {
            return auxAdd(key, val, currentNode->right);
        }
    }

    // Duplicate key
    else if (currentNode->key == key) {
        return false;
    }
    return false;
}

Map::Node* Map::BST::findNode(KeyType key)
{
    return auxFindNode(key, root);
}

Map::Node* Map::BST::auxFindNode(KeyType key, Node* currentNode)
{
    if (currentNode == nullptr || key == currentNode->key) {
        return currentNode;
    }

    // Search left
    if (key < currentNode->key) {
        return auxFindNode(key, currentNode->left);

    }

    // Search right
    return auxFindNode(key, currentNode->right);
}

Map::Node* Map::BST::findIndex(int& i, KeyType& key, ValueType& value, Node* currentNode) const {
    if (currentNode == nullptr || i == 0)
        return currentNode;

    Node* leftResult = findIndex(i, key, value, currentNode->left);
    if (leftResult != nullptr)
        return leftResult;

    if (i == 0) {
        key = currentNode->key;
        value = currentNode->val;
        return currentNode;
    }

    // In-order Traversal
    i--;

    return findIndex(i, key, value, currentNode->right);
}

bool Map::BST::removeNode(const KeyType& key, Node*& currentNode)
{
    if (currentNode == nullptr) {
        //std::cout << key << " is not in tree\n";
        return false;
    }

    if (currentNode->key == key) {

        // Case 1: No children
        if (currentNode->left == nullptr && currentNode->right == nullptr) {
            delete currentNode;
            currentNode = nullptr;
            //std::cout << "\nNo Children: " << key << std::endl;
            return true;
        }

        // Case 2: one child
        if (currentNode->left == nullptr || currentNode->right == nullptr) {
            //std::cout << "\nOne Child: " << key << std::endl;

            Node* temp;
            // currentNode has a left child
            if (currentNode->left != nullptr)
                temp = currentNode->left;
            // currentNode has a right child
            else
                temp = currentNode->right;

            delete currentNode;
            currentNode = temp;
            return true;
        }


        // Case 3: Two children
        // std::cout << "\nTwo Child: " << key << std::endl;
        Node* tempParent = currentNode;  // Track the parent of the leftmost node
        Node* temp = auxRemoveNodeTwoChildrenCase(currentNode->right, tempParent);

        currentNode->key = temp->key;
        currentNode->val = temp->val;

        // Remove the temp node 
        if (tempParent->left == temp)
            tempParent->left = temp->right;
        else if (tempParent->right == temp)
            tempParent->right = temp->right;

        delete temp;
        return true;
    }

    // split up return statement so we don't search both subtrees unecessarily
    if (removeNode(key, currentNode->left)) return true;
    if (removeNode(key, currentNode->right)) return true;

    return false;
}

Map::Node* Map::BST::auxRemoveNodeTwoChildrenCase(Node*& n, Node*& parent)
{
    // Finds leftmost in subtree
    // Remove from tree
    // return its pointer to be deleted in auxRemove
    while (n->left != nullptr) {
        parent = n;
        n = n->left;
    }

    // Return leftmost node
    return n;
}

// HashTable Functions:
// HashTable() constructor defined in Map.h

/*
// Hashtable assignment operator not required for implementation not required for this project
Map::HashTable& Map::HashTable::operator=(const HashTable& rhs)
{
    // TODO: insert return statement here
}
*/

Map::HashTable::~HashTable()
{
    //go through entire hashtable
    for (int index = 0; index < totalSize; index++) {
        if (list[index] == nullptr)
            continue;

        // Delete the BST itself
        // Prevent dangling pointer -- I don't think it's neccessary but just in case
        delete list[index];
        list[index] = nullptr;
    }
}

int Map::HashTable::hash(KeyType key) const {
    int index = 0;
    index = Convert_Key(key);

    return abs(index % totalSize);
}

int Map::HashTable::Convert_Key(std::string key) const{
    int hashResult = 0;
    for (int i = 0; i < key.size(); i++) {
        hashResult += key[i];
    }
    hashResult *= 97;
    
    return hashResult;
}

int Map::HashTable::Convert_Key(double key) const{
    int hashResult = int(97 - key) % totalSize;
    return abs(hashResult);
}

// Map Functions:
// Map() constructor defined in Map.h

Map::Map(const Map& other)
{
    totalValues = other.totalValues;
    for (int i = 0; i < hashTable.totalSize; i++) {
        if ((other.hashTable.list)[i] == nullptr) {
            (hashTable.list)[i] = nullptr;
            continue;
        }

        (hashTable.list)[i] = new BST();
        (hashTable.list)[i]->auxCopyConstructor((hashTable.list)[i]->root, (other.hashTable.list)[i]->root);
    }
}


const Map& Map::operator=(const Map& rhs)
{
    if (this == &rhs) //prevent self-copy
        return *this;

    // Delete current values
    for (int index = 0; index < hashTable.totalSize; index++) {
        if (hashTable.list[index] == nullptr)
            continue;

        // Delete the BST -- BST has its own destructor so this will prevent memory leak
        delete hashTable.list[index];
        hashTable.list[index] = nullptr; // Prevent dangling pointer
    }

    // Copy in new values
    totalValues = rhs.totalValues;
    for (int i = 0; i < hashTable.totalSize; i++) {
        if ((rhs.hashTable.list)[i] == nullptr) {
            (hashTable.list)[i] = nullptr; // Handled in deletion
            continue;
        }

        (hashTable.list)[i] = new BST();
        (hashTable.list)[i]->auxCopyConstructor((hashTable.list)[i]->root, (rhs.hashTable.list)[i]->root);
    }

    return *this;
}

Map::~Map()
{
    // struct HashTable holds all the dynamically allocated memory.
    // Therefore cleanup is only necessary when hashTable is deleted
}

int Map::size() const {
    return totalValues;
}

bool Map::empty() const
{
    return totalValues == 0;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    // check for duplicate keys
    int index = hashTable.hash(key);

    if (hashTable.list[index] != nullptr && hashTable.list[index]->findNode(key) != nullptr) {
        return false;  // Check for duplicates before allocating memory
    }

    if (hashTable.list[index] == nullptr) {
        hashTable.list[index] = new BST();
    }

    //std::cout << "Key " << key << " Index: " << index << std::endl;
    hashTable.list[index]->add(key, value);
    totalValues++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    int index = hashTable.hash(key);

    // return false if there is no BST at index
    if (hashTable.list[index] == nullptr)
        return false;

    // check that the tree *has* value
    Node* temp = hashTable.list[index]->findNode(key);
    if (temp != nullptr) {
        temp->val = value;
        return true;
    }

    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    int index = hashTable.hash(key);

    Node* temp = nullptr;
    if (hashTable.list[index] != nullptr)
        temp = hashTable.list[index]->findNode(key);

    if (temp == nullptr) {
        insert(key, value);
    }

    else if (temp != nullptr) {
        temp->val = value;
    }

    return true;
}

bool Map::erase(const KeyType& key)
{
    int index = hashTable.hash(key);
    if (hashTable.list[index] == nullptr) {
        //std::cout << key << " is in nullptr(empty) bucket\n";
        return false;
    }

    bool successfulErasure = (hashTable.list[index])->removeNode(key, hashTable.list[index]->root);

    if (successfulErasure)
        totalValues--;

    return successfulErasure;
}

bool Map::contains(const KeyType& key) const
{
    int index = hashTable.hash(key);
    if (hashTable.list[index] == nullptr)
        return false;

    Node* temp = hashTable.list[index]->findNode(key);
    if (temp == nullptr) {
        return false;
    }

    return true;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    int index = hashTable.hash(key);
    if (hashTable.list[index] == nullptr)
        return false;

    Node* temp = hashTable.list[index]->findNode(key);

    if (temp == nullptr) {
        return false;
    }

    value = temp->val;
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i >= totalValues || i > totalValues)
        return false;

    //search through hashtable BSTs
    for (int hashIndex = 0; hashIndex < hashTable.totalSize; hashIndex++) {
        if (hashTable.list[hashIndex] == nullptr)
            continue;

        Node* temp = hashTable.list[hashIndex]->findIndex(i, key, value, hashTable.list[hashIndex]->root);
        if (temp == nullptr)
            continue;

        if (temp != nullptr && i <= 0) {
            value = temp->val;
            key = temp->key;
            return true;
        };
    }

    return false;
}

void Map::swap(Map& other)
{
    // Swap the entire list vectors 
    std::vector<BST*> tempList = hashTable.list;
    hashTable.list = other.hashTable.list;
    other.hashTable.list = tempList;

    // Swap totalValues
    int tempTotalValues = totalValues;
    totalValues = other.totalValues;
    other.totalValues = tempTotalValues;
}

bool combine(const Map& m1, const Map& m2, Map& result) {
    result = Map();
    bool success = true;

    // First pass: go through m1
    for (int i = 0; i < m1.size(); i++) {
        KeyType k;
        ValueType v1;
        m1.get(i, k, v1);

        ValueType v2;
        // key also exists in m2
        if (m2.get(k, v2)) {
            if (v1 == v2) {
                // same value
                result.insert(k, v1);
            }

            // conflict
            // do not insert the key at all
            else {
                success = false;
            }
        }

        // unique to m1
        else {
            result.insert(k, v1);
        }
    }

    // Second pass: find keys in m2 that weren’t already handled
    for (int i = 0; i < m2.size(); i++) {
        KeyType k;
        ValueType v;
        m2.get(i, k, v);

        // unique to m2
        if (!m1.contains(k)) {
            result.insert(k, v);
        }
    }

    return success;
}

void subtract(const Map& m1, const Map& m2, Map& result) {
    // clear result
    result = Map();

    for (int i = 0; i < m1.size(); i++) {
        KeyType k;
        ValueType v;
        m1.get(i, k, v);

        // Only add if key not in m2
        if (!m2.contains(k)) {
            result.insert(k, v);
        }
    }
}