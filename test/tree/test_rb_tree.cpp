#include <gtest.h>
#include "rb_tree.h"
#include <vector>
#include <algorithm> 
#include <set>       
#include <random>

template <class TKey, class TValue>
bool check_rb_properties_func(RBTree<TKey, TValue>& tree) {
    // Предполагаем, что RBTree имеет публичный метод check_all_rb_properties()
    return tree.check_all_rb_properties();
}


TEST(rb_tree_suite, insert_simple) {
    RBTree<int, std::string> rb_tree;
    ASSERT_TRUE(rb_tree.insert(10, "ten"));
    ASSERT_TRUE(check_rb_properties_func(rb_tree));
    ASSERT_TRUE(rb_tree.insert(5, "five"));
    ASSERT_TRUE(check_rb_properties_func(rb_tree));
    ASSERT_TRUE(rb_tree.insert(15, "fifteen"));
    ASSERT_TRUE(check_rb_properties_func(rb_tree));
    ASSERT_FALSE(rb_tree.insert(10, "ten_again"));

    auto keys = rb_tree.get_all_keys_inorder();
    std::vector<int> expected_keys = { 5, 10, 15 };
    ASSERT_EQ(keys, expected_keys);
}

TEST(rb_tree_suite, insert_causing_recolor) {
    RBTree<int, std::string> rb_tree;
    rb_tree.insert(10, "");
    rb_tree.insert(5, "");
    rb_tree.insert(15, "");
    ASSERT_TRUE(check_rb_properties_func(rb_tree)); // 10(B), 5(R), 15(R)
    rb_tree.insert(20, ""); // Вставка 20 (R). Parent 15(R). Uncle 5(R). -> Recolor
    // 10(R), 5(B), 15(B), 20(R). Root 10 становится R, затем B.
    ASSERT_TRUE(check_rb_properties_func(rb_tree));
    auto keys = rb_tree.get_all_keys_inorder();
    std::vector<int> expected_keys = { 5, 10, 15, 20 };
    ASSERT_EQ(keys, expected_keys);
}

TEST(rb_tree_suite, insert_causing_rotations) {
    RBTree<int, std::string> rb_tree_ll;
    rb_tree_ll.insert(30, "");
    rb_tree_ll.insert(20, "");
    rb_tree_ll.insert(10, "");
    ASSERT_TRUE(check_rb_properties_func(rb_tree_ll));
    auto keys1 = rb_tree_ll.get_all_keys_inorder();
    std::vector<int> expected1 = { 10, 20, 30 };
    ASSERT_EQ(keys1, expected1);

    RBTree<int, std::string> rb_tree_lr;
    rb_tree_lr.insert(30, "");
    rb_tree_lr.insert(10, "");
    rb_tree_lr.insert(20, "");
    ASSERT_TRUE(check_rb_properties_func(rb_tree_lr));
    auto keys2 = rb_tree_lr.get_all_keys_inorder();
    std::vector<int> expected2 = { 10, 20, 30 };
    ASSERT_EQ(keys2, expected2);
}

TEST(rb_tree_suite, erase_various_cases_complex) {
    RBTree<int, std::string> rb_tree;
    int nums_to_insert[] = { 10, 5, 15, 3, 7, 12, 17, 1, 4, 6, 8, 11, 14, 16, 18, 20, 2, 9, 13, 19 };
    for (int num : nums_to_insert) {
        ASSERT_TRUE(rb_tree.insert(num, "val_" + std::to_string(num)));
        ASSERT_TRUE(check_rb_properties_func(rb_tree));
    }

    std::vector<int> keys_to_erase = { 1, 10, 20, 7, 15, 13, 4, 18 }; // Разные случаи
    std::set<int> initial_set;
    for (int num : nums_to_insert) initial_set.insert(num);

    for (int key_to_erase : keys_to_erase) {
        ASSERT_TRUE(rb_tree.erase(key_to_erase));
        ASSERT_TRUE(check_rb_properties_func(rb_tree));
        initial_set.erase(key_to_erase);
    }

    ASSERT_FALSE(rb_tree.erase(100));

    auto remaining_keys_vec = rb_tree.get_all_keys_inorder();
    std::set<int> actual_remaining_set(remaining_keys_vec.begin(), remaining_keys_vec.end());
    ASSERT_EQ(actual_remaining_set, initial_set);
}

TEST(rb_tree_suite, delete_root_multiple_times_rb) {
    RBTree<int, std::string> rb_tree;
    rb_tree.insert(10, "");
    rb_tree.insert(5, "");
    rb_tree.insert(15, "");
    ASSERT_TRUE(check_rb_properties_func(rb_tree));

    ASSERT_TRUE(rb_tree.erase(10));
    ASSERT_TRUE(check_rb_properties_func(rb_tree));
    auto keys1 = rb_tree.get_all_keys_inorder();
    ASSERT_EQ(keys1.size(), 2);

    // Удаляем один из оставшихся (который мог стать корнем)
    ASSERT_TRUE(rb_tree.erase(keys1[0]));
    ASSERT_TRUE(check_rb_properties_func(rb_tree));
    auto keys2 = rb_tree.get_all_keys_inorder();
    ASSERT_EQ(keys2.size(), 1);

    ASSERT_TRUE(rb_tree.erase(keys2[0]));
    ASSERT_TRUE(check_rb_properties_func(rb_tree));
    ASSERT_TRUE(rb_tree.get_all_keys_inorder().empty());
}
