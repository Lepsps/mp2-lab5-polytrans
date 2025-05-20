#include "translator.h"      
#include <iostream>
#include <limits>

void runTranslatorWithMapStorage();
void runTranslatorWithAddressHash();
void runTranslatorWithChainHash();
void runTranslatorWithUnorderedTable();
void runTranslatorWithOrderedTable();
void runTranslatorWithBSTree();
void runTranslatorWithRBTree();
void runTranslatorWithAVLTree();


int main() {
    std::cout << "Select variable storage type:" << std::endl;
    std::cout << "1. std::map" << std::endl;
    std::cout << "2. Address Hash Table" << std::endl;
    std::cout << "3. Chain Hash Table" << std::endl;
    std::cout << "4. Unordered Table (vector)" << std::endl;
    std::cout << "5. Ordered Table (sorted vector)" << std::endl;
    std::cout << "6. Binary Search Tree (BST)" << std::endl;
    std::cout << "7. Red-Black Tree (RBTree)" << std::endl;
    std::cout << "8. AVL Tree" << std::endl;
    std::cout << "Enter choice (1-8): ";

    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > 8) {
        std::cout << "Invalid input. Please enter a number between 1 and 8: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
    case 1: runTranslatorWithMapStorage(); break;
    case 2: runTranslatorWithAddressHash(); break;
    case 3: runTranslatorWithChainHash(); break;
    case 4: runTranslatorWithUnorderedTable(); break;
    case 5: runTranslatorWithOrderedTable(); break;
    case 6: runTranslatorWithBSTree(); break;
    case 7: runTranslatorWithRBTree(); break;
    case 8: runTranslatorWithAVLTree(); break;
    default: std::cerr << "Invalid choice." << std::endl; break;
    }

    return 0;
}

void runTranslatorWithMapStorage() {
    std::cout << "Using std::map storage." << std::endl;
    Translator<std::map<std::string, Polynom>> translator;
    translator.execute();
}

void runTranslatorWithAddressHash() {
    std::cout << "Using Address Hash Table storage." << std::endl;
    Translator<AddressHashTable<std::string, Polynom>> translator;
    translator.execute();
}

void runTranslatorWithChainHash() {
    std::cout << "Using Chain Hash Table storage." << std::endl;
    Translator<ChainHash<std::string, Polynom>> translator;
    translator.execute();
}

void runTranslatorWithUnorderedTable() {
    std::cout << "Using Unordered Table storage." << std::endl;
    Translator<UnOrderedTable<std::string, Polynom>> translator;
    translator.execute();
}
void runTranslatorWithOrderedTable() {
    std::cout << "Using Ordered Table storage." << std::endl;
    Translator<OrderedTable<std::string, Polynom>> translator;
    translator.execute();
}
void runTranslatorWithBSTree() {
    std::cout << "Using Binary Search Tree (BST) storage." << std::endl;
    Translator<BSTree<std::string, Polynom>> translator;
    translator.execute();
}

void runTranslatorWithRBTree() {
    std::cout << "Using Red-Black Tree storage." << std::endl;
    Translator<RBTree<std::string, Polynom>> translator;
    translator.execute();
}
void runTranslatorWithAVLTree() {
    std::cout << "Using AVL Tree storage." << std::endl;
    Translator<AVLTree<std::string, Polynom>> translator;
    translator.execute();
}