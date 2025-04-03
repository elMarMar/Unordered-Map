#include "Map.h"
#include <string>
#include<iostream>
#include <cassert>
#include <type_traits>

using namespace std;
using KeyType = std::string;
using ValueType = double;

#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }
static_assert(std::is_default_constructible<Map>::value,
    "Map must be default-constructible.");
static_assert(std::is_copy_constructible<Map>::value,
    "Map must be copy-constructible.");

void ThisFunctionWillNeverBeCalled()
{
    CHECKTYPE(&Map::operator=, Map & (Map::*)(const Map&));
    CHECKTYPE(&Map::empty, bool (Map::*)() const);
    CHECKTYPE(&Map::size, int  (Map::*)() const);
    CHECKTYPE(&Map::insert, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::update, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::insertOrUpdate, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::erase, bool (Map::*)(const KeyType&));
    CHECKTYPE(&Map::contains, bool (Map::*)(const KeyType&) const);
    CHECKTYPE(&Map::get, bool (Map::*)(const KeyType&, ValueType&) const);
    CHECKTYPE(&Map::get, bool (Map::*)(int, KeyType&, ValueType&) const);
    CHECKTYPE(&Map::swap, void (Map::*)(Map&));

    CHECKTYPE(combine, bool (*)(const Map&, const Map&, Map&));
    CHECKTYPE(subtract, void (*)(const Map&, const Map&, Map&));
}


int main() {
    //TODO update uncomment lines

    Map m;

    // "" (Empty Key can be inserted)
    // non-empty keys can be inserted
    // Uncomment Line 341 -- Checks and prints which index each Key/Value was inserted to.
    assert(m.insert("", 4.0002));
    assert(m.insert("Fred", 2.956));

    m.insert("Ethel", 3.538);
    m.insert("lethE", 27.28);
    m.insert("tElhe", 27.28);

    m.insert("jiM", 40.0);
    m.insert("Mij", 25.0);

    m.insert("cuLy", 8.008);
    m.insert("Lucy", 2.956);
    m.insert("ycuL", 9.008);

    // Case 0 & Case 1: No Children Case
    // Uncomment Line 157 -- Checks to see that key "Fred" was deleted under no children case
    assert(m.erase("Fred"));

    // Case 2.1: One Child Case -- RIGHT CHILD
    // Uncomment Line 163 -- Checks to see that key "Ethel" was deleted under one child case
    assert(m.erase("Ethel"));
    //  Ethel
    //  /   \
    // null lethE
    //         \
    //       tElhe

    // Case 2.2: One Child case -- LEFT CHILD
    // Uncomment Line 163 -- Checks to see that key "Ethel" was deleted under one child case
    assert(m.erase("jiM"));
    //   jiM
    //  /   \
    // Mij  null

    // Case 3: Two Children Case
    // Uncomment Line 180 -- Checks that key "cuLy" was deleted under two child case
    assert(m.erase("cuLy"));
    //    culy <--- Erase
    //    /   \
    //  Lucy  yCul

    // Case 4.1: The key is not in the map
    // Uncomment Line 387 -- Shows that erase returns false BECAUSE there is no bucket that matches key's hashed index
    assert(!m.erase("Example of Key not in Hashmap"));

    // Case 4.2: The key would normally be in a tree that's been allocated BUT cannot be found in BST
    // cuLy has already been delete SO it shouldn't be found
    // Uncomment Line 387 -- Should print 3 times for the 3 nullptrs the function encounters
    assert(!m.erase("cuLy"));

    // Case 5: A key that has already been added (duplicate) should not be added to the map again
    assert(!m.insert("Lucy", 100.0));

    // Check Size -- Initial Size: 10 --> Erasure Size: 6
    assert(m.size() == 6);


    KeyType k = "";
    ValueType v;
    m.update("", 100.34);
    for (int i = 0; i < m.size(); i++) {
        // Should not return false
        assert(m.get(i, k, v));
        cout << '\n' << k << " " << v;
    }

    //Should return false since i == size() == 6
    assert(!m.get(6, k, v));
    cout << "\nTotal Values " << m.size() << endl;

    // update(...) Case 1: Key "Lucy" is found in map
    v = -1;
    assert(m.update("Lucy", 100));
    m.get("Lucy", v);
    cout << "Updated Key: " << "Lucy" << " " << v << endl;

    // update(...) Case 2: Key "Example of Key not in map" is not found in map
    v = -1;
    assert(!m.update("Example of Key not in map", 100));
    m.get("Example of Key not in map", v);
    cout << "Updated Key: " << "Example of Key not in map" << ". Prints -1 if value not updated " << v << endl;

    // insertOrUpdate(...) Case 1: Key "Lucy" is in the map. Value should be update
    v = -1;
    assert(m.insertOrUpdate("Lucy", 505050));
    m.get("Lucy", v);
    cout << "Updated Key: " << "Lucy" << " " << v << endl;

    // insertOrUpdate(...) Case 2: Key "Jeff" is NOT in the map. Key and Value should be inserted
    v = -1;
    assert(m.insertOrUpdate("Jeff", 101010));
    m.get("Jeff", v);
    cout << "Inserted Key: " << "Jeff" << " " << v << endl;

    // contains(...) Case 1: Key "Example of Key not in map" is not found in map
    assert(!m.contains("Example of Key not in map"));
    // contains(...) Case 2: Key "Jeff" is found in map
    assert(m.contains("Jeff"));

    // Project 1 Given Assertions
    // swap() assertions
    Map m1;
    m1.insert("Fred", 2.956);
    Map m2;
    m2.insert("Ethel", 3.538);
    m2.insert("Lucy", 2.956);
    m1.swap(m2);
    assert(m1.size() == 2 && m1.contains("Ethel") && m1.contains("Lucy") &&
        m2.size() == 1 && m2.contains("Fred"));

    // get() and insert() assertions
    Map m3;
    m3.insert("A", 10);
    m3.insert("B", 44);
    m3.insert("C", 10);
    string all;
    double total = 0;
    for (int n = 0; n < m3.size(); n++)
    {
        string k;
        double v;
        m3.get(n, k, v);
        all += k;
        total += v;
    }
    cout << all << total;


    Map gpas;

    gpas.insert("Fred", 2.956);
    gpas.insert("Ethel", 3.538);
    //double v;
    string k1;
    assert(gpas.get(1, k1, v) && (k1 == "Fred" || k1 == "Ethel"));
    string k2;
    assert(gpas.get(1, k2, v) && k2 == k1);


    // empty string insert assertion
    Map gpas2;
    gpas2.insert("Fred", 2.956);
    assert(!gpas2.contains(""));
    gpas2.insert("Ethel", 3.538);
    gpas2.insert("", 4.000);
    gpas2.insert("Lucy", 2.956);
    assert(gpas2.contains(""));
    gpas2.erase("Fred");
    assert(gpas2.size() == 3 && gpas2.contains("Lucy") && gpas2.contains("Ethel") &&
        gpas2.contains(""));

    // ASSERTIONS FOR COPY AND ASSIGNMENT OPERATOR
    Map gpas1 = gpas;
    gpas2 = gpas;

    assert(&gpas1 != &gpas && gpas2.size() == gpas.size());
    assert(&gpas2 != &gpas && gpas2.size() == gpas.size());

    for (int i = 0; i < gpas.size(); i++)
    {
        string k1, k2;
        double v1, v2;
        gpas.get(i, k1, v1);
        gpas2.get(i, k2, v2);

        assert(k1 == k2 && v1 == v2);
    }

    // ASSERTIONS FOR COMBINE/SUBTRACT 
    Map combineSubtractTest1, combineSubtractTest2, result;

    combineSubtractTest1.insert("Fred", 123.0);
    combineSubtractTest1.insert("Ethel", 456.0);
    combineSubtractTest1.insert("Lucy", 789.0);

    combineSubtractTest2.insert("Lucy", 789.0);
    combineSubtractTest2.insert("Ricky", 321);
    combineSubtractTest2.insert("Ethel", 654);

    subtract(combineSubtractTest1, combineSubtractTest2, result);
    assert(result.size() == 1 && result.contains("Fred")
        && !result.contains("Ethel")
        && !result.contains("Lucy")
        && !result.contains("Ricky"));

    combine(combineSubtractTest1, combineSubtractTest2, result);

    assert(result.size() == 3
        && result.contains("Fred")
        && result.contains("Lucy")
        && result.contains("Ricky"));
}