#include <gtest.h>
#include "avl_tree.h"


template <class TKey, class TValue>
bool is_avl_balanced_check(AVLTree<TKey, TValue>& tree) {
    return tree.is_avl_balanced();
}

TEST(avl_tree_suite, stress_test) {
    AVLTree <int, std::string> r1;

    for (int i = 0; i < 1000000; i++) {
        r1.insert(i, std::to_string(i));
    }
    for (int i = 0; i < 1000000; i++) {
        r1.erase(i);
    }

    EXPECT_EQ(r1.begin(), r1.end());
}

TEST(avl_tree_suite, insert_simple_no_rotation) {
    AVLTree<int, std::string> avl_tree;

    ASSERT_TRUE(avl_tree.insert(10, "ten"));
    ASSERT_TRUE(avl_tree.insert(5, "five"));
    ASSERT_TRUE(avl_tree.insert(15, "fifteen"));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    auto keys = avl_tree.get_all_keys_inorder();
    std::vector<int> expected = { 5, 10, 15 };
    ASSERT_EQ(keys, expected);
}

TEST(avl_tree_suite, insert_left_left_case) {
    AVLTree<int, std::string> avl_tree;

    ASSERT_TRUE(avl_tree.insert(30, "thirty"));
    ASSERT_TRUE(avl_tree.insert(20, "twenty"));
    ASSERT_TRUE(avl_tree.insert(10, "ten"));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    auto keys = avl_tree.get_all_keys_inorder();
    std::vector<int> expected = { 10, 20, 30 };
    ASSERT_EQ(keys, expected);
}

TEST(avl_tree_suite, insert_right_right_case) {
    AVLTree<int, std::string> avl_tree;

    ASSERT_TRUE(avl_tree.insert(10, "ten"));
    ASSERT_TRUE(avl_tree.insert(20, "twenty"));
    ASSERT_TRUE(avl_tree.insert(30, "thirty"));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    auto keys = avl_tree.get_all_keys_inorder();
    std::vector<int> expected = { 10, 20, 30 };
    ASSERT_EQ(keys, expected);
}

TEST(avl_tree_suite, insert_left_right_case) {
    AVLTree<int, std::string> avl_tree;

    ASSERT_TRUE(avl_tree.insert(30, "thirty"));
    ASSERT_TRUE(avl_tree.insert(10, "ten"));
    ASSERT_TRUE(avl_tree.insert(20, "twenty"));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    auto keys = avl_tree.get_all_keys_inorder();
    std::vector<int> expected = { 10, 20, 30 };
    ASSERT_EQ(keys, expected);
}

TEST(avl_tree_suite, insert_right_left_case) {
    AVLTree<int, std::string> avl_tree;

    ASSERT_TRUE(avl_tree.insert(10, "ten"));
    ASSERT_TRUE(avl_tree.insert(30, "thirty"));
    ASSERT_TRUE(avl_tree.insert(20, "twenty"));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    auto keys = avl_tree.get_all_keys_inorder();
    std::vector<int> expected = { 10, 20, 30 };
    ASSERT_EQ(keys, expected);
}

TEST(avl_tree_suite, erase_and_rebalance) {
    AVLTree<int, std::string> avl_tree;

    avl_tree.insert(10, "");
    avl_tree.insert(5, ""); 
    avl_tree.insert(15, "");
    avl_tree.insert(3, ""); 
    avl_tree.insert(7, ""); 
    avl_tree.insert(12, ""); 
    avl_tree.insert(17, "");
    avl_tree.insert(1, "");

    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    ASSERT_TRUE(avl_tree.erase(17));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    ASSERT_TRUE(avl_tree.erase(12));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    ASSERT_TRUE(avl_tree.erase(10));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    auto keys = avl_tree.get_all_keys_inorder();
    std::vector<int> expected = { 1, 3, 5, 7, 15 };
    ASSERT_EQ(keys, expected);
}

TEST(avl_tree_suite, series_of_rotations_complex_insert) {
    AVLTree<int, std::string> avl_tree;

    ASSERT_TRUE(avl_tree.insert(10, "")); ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    ASSERT_TRUE(avl_tree.insert(20, "")); ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    ASSERT_TRUE(avl_tree.insert(5, ""));  ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    ASSERT_TRUE(avl_tree.insert(30, "")); ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    ASSERT_TRUE(avl_tree.insert(25, "")); ASSERT_TRUE(is_avl_balanced_check(avl_tree));

    auto keys = avl_tree.get_all_keys_inorder();
    std::vector<int> expected = { 5, 10, 20, 25, 30 };
    ASSERT_EQ(keys, expected);

    ASSERT_TRUE(avl_tree.erase(5));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    ASSERT_TRUE(avl_tree.erase(30));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));
    ASSERT_TRUE(avl_tree.erase(10));
    ASSERT_TRUE(is_avl_balanced_check(avl_tree));

    auto keys_after_erase = avl_tree.get_all_keys_inorder();
    std::vector<int> expected_after_erase = { 20, 25 };
    ASSERT_EQ(keys_after_erase, expected_after_erase);
}
