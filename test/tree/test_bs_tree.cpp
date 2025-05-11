#include <gtest.h>
#include "bs_tree.h"  // предполагается, что код сохранён как BSTree.h

TEST(BSTreeTest, EmptyInitially) {
    BSTree<int, std::string> tree;
    EXPECT_TRUE(tree.empty());
}


TEST(BSTreeTest, InsertDuplicateThrows) {
    BSTree<int, std::string> tree;
    tree.insert(1, "x");
    EXPECT_THROW(tree.insert(1, "duplicate"), std::runtime_error);
}

TEST(BSTreeTest, EraseLeafNode) {
    BSTree<int, std::string> tree;
    tree.insert(10, "root");
    tree.insert(5, "left");
    tree.erase(5);
    EXPECT_EQ(tree.find(5), nullptr);
}

TEST(BSTreeTest, EraseNodeWithOneChild) {
    BSTree<int, std::string> tree;
    tree.insert(10, "root");
    tree.insert(5, "left");
    tree.insert(2, "left-left");
    tree.erase(5);
    EXPECT_EQ(tree.find(5), nullptr);
    EXPECT_NE(tree.find(2), nullptr);
}

TEST(BSTreeTest, EraseNodeWithTwoChildren) {
    BSTree<int, std::string> tree;
    tree.insert(10, "root");
    tree.insert(5, "left");
    tree.insert(15, "right");
    tree.insert(13, "rl");
    tree.insert(17, "rr");

    tree.erase(15);
    EXPECT_EQ(tree.find(15), nullptr);
    EXPECT_NE(tree.find(13), nullptr);
    EXPECT_NE(tree.find(17), nullptr);
}

TEST(BSTreeTest, EraseNonExistentKeyThrows) {
    BSTree<int, std::string> tree;
    tree.insert(1, "x");
    EXPECT_THROW(tree.erase(999), std::runtime_error);
}

TEST(BSTreeTest, IteratorTraversalInOrder) {
    BSTree<int, char> tree;
    tree.insert(3, 'c');
    tree.insert(1, 'a');
    tree.insert(2, 'b');
    tree.insert(4, 'd');

    std::string result;
    for (auto it = tree.begin(); it != tree.end(); ++it)
        result += it.value();
    EXPECT_EQ(result, "abcd");
}

TEST(BSTreeTest, IteratorEquality) {
    BSTree<int, int> tree;
    tree.insert(1, 10);
    auto it1 = tree.begin();
    auto it2 = tree.begin();
    EXPECT_TRUE(it1 == it2);
    ++it1;
    EXPECT_FALSE(it1 == it2);
}

TEST(BSTreeTest, ClearTree) {
    BSTree<int, std::string> tree;
    tree.insert(10, "a");
    tree.insert(20, "b");
    tree.clear();
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.find(10), nullptr);
}

TEST(BSTreeTest, IsBalanceBalancedTree) {
    BSTree<int, int> tree;
    tree.insert(2, 0);
    tree.insert(1, 0);
    tree.insert(3, 0);
    EXPECT_TRUE(tree.is_balance());
}

TEST(BSTreeTest, IsBalanceUnbalancedTree) {
    BSTree<int, int> tree;
    tree.insert(1, 0);
    tree.insert(2, 0);
    tree.insert(3, 0);
    EXPECT_FALSE(tree.is_balance());
}

TEST(BSTreeTest, FindItReturnsCorrectIterator) {
    BSTree<int, std::string> tree;
    tree.insert(42, "answer");
    auto it = tree.find_it(42);
    EXPECT_EQ(it.key(), 42);
    EXPECT_EQ(it.value(), "answer");
}

TEST(BSTreeTest, EraseItReturnsUpdatedRoot) {
    BSTree<int, std::string> tree;
    tree.insert(10, "root");
    tree.insert(5, "left");
    tree.insert(15, "right");

    auto newRoot = tree.erase_it(5);
    EXPECT_EQ(newRoot.key(), 10);
    EXPECT_EQ(tree.find(5), nullptr);
}

TEST(BSTreeTest, InsertItReturnsCorrectIterator) {
    BSTree<int, std::string> tree;
    auto it = tree.insert_it(99, "ninety-nine");
    EXPECT_EQ(it.key(), 99);
    EXPECT_EQ(it.value(), "ninety-nine");
}
