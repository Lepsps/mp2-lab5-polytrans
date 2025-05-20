#include <gtest.h>
#include "bs_tree.h"
#include <vector>
#include <algorithm> 
#include <set>       
#include <random>


TEST(bst_tree_suite, insert_and_find) {
    BSTree<int, std::string> tree;
    ASSERT_TRUE(tree.insert(10, "ten"));
    ASSERT_TRUE(tree.insert(5, "five"));
    ASSERT_TRUE(tree.insert(15, "fifteen"));
    ASSERT_FALSE(tree.insert(10, "ten_again"));

    ASSERT_NE(tree.find(10), nullptr);
    ASSERT_EQ(tree.find(10)->value, "ten");
    ASSERT_NE(tree.find(5), nullptr);
    ASSERT_EQ(tree.find(5)->value, "five");
    ASSERT_NE(tree.find(15), nullptr);
    ASSERT_EQ(tree.find(15)->value, "fifteen");
    ASSERT_EQ(tree.find(100), nullptr);
}

TEST(bst_tree_suite, erase_leaf) {
    BSTree<int, std::string> tree;
    tree.insert(10, "ten");
    tree.insert(5, "five");
    ASSERT_TRUE(tree.erase(5));
    ASSERT_EQ(tree.find(5), nullptr);
    ASSERT_NE(tree.find(10), nullptr);
    ASSERT_FALSE(tree.erase(5));
}

TEST(bst_tree_suite, erase_node_with_one_child) {
    BSTree<int, std::string> tree;
    tree.insert(10, "ten");
    tree.insert(5, "five");
    tree.insert(3, "three");
    ASSERT_TRUE(tree.erase(5));
    ASSERT_EQ(tree.find(5), nullptr);
    ASSERT_NE(tree.find(10), nullptr);
    ASSERT_NE(tree.find(3), nullptr);

    auto keys = tree.get_all_keys_inorder();
    ASSERT_EQ(keys.size(), 2);
    ASSERT_EQ(keys[0], 3);
    ASSERT_EQ(keys[1], 10);
}

TEST(bst_tree_suite, erase_node_with_two_children) {
    BSTree<int, std::string> tree;
    tree.insert(10, "ten");
    tree.insert(5, "five");
    tree.insert(15, "fifteen");
    tree.insert(3, "three");
    tree.insert(7, "seven");
    tree.insert(12, "twelve");
    tree.insert(17, "seventeen");

    ASSERT_TRUE(tree.erase(10));
    ASSERT_EQ(tree.find(10), nullptr);

    auto keys = tree.get_all_keys_inorder();
    ASSERT_EQ(keys.size(), 6);
    ASSERT_TRUE(std::is_sorted(keys.begin(), keys.end()));
    std::set<int> expected_keys = { 3, 5, 7, 12, 15, 17 };
    std::set<int> actual_keys(keys.begin(), keys.end());
    ASSERT_EQ(actual_keys, expected_keys);
}

TEST(bst_tree_suite, erase_root) {
    BSTree<int, std::string> tree;
    tree.insert(10, "ten");
    ASSERT_TRUE(tree.erase(10));
    ASSERT_EQ(tree.find(10), nullptr);
    ASSERT_TRUE(tree.get_all_keys_inorder().empty());

    tree.insert(10, "ten");
    tree.insert(5, "five");
    ASSERT_TRUE(tree.erase(10));
    auto keys = tree.get_all_keys_inorder();
    ASSERT_EQ(keys.size(), 1);
    ASSERT_EQ(keys[0], 5);
}

TEST(bst_tree_suite, insert_descending_order) {
    BSTree<int, std::string> tree;
    ASSERT_TRUE(tree.insert(30, "thirty"));
    ASSERT_TRUE(tree.insert(20, "twenty"));
    ASSERT_TRUE(tree.insert(10, "ten"));
    auto keys = tree.get_all_keys_inorder();
    std::vector<int> expected = { 10, 20, 30 };
    ASSERT_EQ(keys, expected);
}

TEST(bst_tree_suite, stress_insert_and_erase_many) {
    BSTree<int, std::string> tree;
    std::vector<int> data;
    for (int i = 0; i < 50; ++i) {
        data.push_back(i * 2);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);

    for (int val : data) {
        ASSERT_TRUE(tree.insert(val, std::to_string(val)));
    }

    auto keys1 = tree.get_all_keys_inorder();
    std::sort(data.begin(), data.end());
    ASSERT_EQ(keys1, data);

    std::shuffle(data.begin(), data.end(), g);

    std::set<int> initial_set(data.begin(), data.end());
    std::vector<int> removed_keys_for_check;

    for (size_t i = 0; i < data.size() / 2; ++i) {
        ASSERT_TRUE(tree.erase(data[i]));
        initial_set.erase(data[i]);
        removed_keys_for_check.push_back(data[i]);
    }

    auto keys2 = tree.get_all_keys_inorder();
    ASSERT_TRUE(std::is_sorted(keys2.begin(), keys2.end()));

    std::set<int> actual_remaining_set(keys2.begin(), keys2.end());
    ASSERT_EQ(actual_remaining_set.size(), initial_set.size());
    ASSERT_EQ(actual_remaining_set, initial_set);
}

TEST(bst_tree_suite, iterator_traversal_empty) {
    BSTree<int, std::string> tree;
    int count = 0;
    for (BSTree<int, std::string>::Iterator it = tree.begin(); it != tree.end(); ++it) {
        count++;
    }
    ASSERT_EQ(count, 0);
    ASSERT_EQ(tree.begin(), tree.end());
}

TEST(bst_tree_suite, iterator_traversal_single_node) {
    BSTree<int, std::string> tree;
    tree.insert(10, "ten");
    std::vector<int> inorder_keys;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        inorder_keys.push_back(it->key);
    }
    std::vector<int> expected_keys = { 10 };
    ASSERT_EQ(inorder_keys, expected_keys);
}

TEST(bst_tree_suite, iterator_traversal_multiple_nodes) {
    BSTree<int, std::string> tree;
    tree.insert(10, "ten");
    tree.insert(5, "five");
    tree.insert(15, "fifteen");
    tree.insert(3, "three");
    tree.insert(7, "seven");

    std::vector<int> inorder_keys;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        inorder_keys.push_back(it->key);
    }

    std::vector<int> expected_keys = { 3, 5, 7, 10, 15 };
    ASSERT_EQ(inorder_keys, expected_keys);
}