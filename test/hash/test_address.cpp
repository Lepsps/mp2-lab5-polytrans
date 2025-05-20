#include <gtest.h>
#include "address_hash.h"


TEST(AddressHashTableTest, stress_test) {
    AddressHashTable<int, std::string> t1;

    for (int i = 0; i < 1000000; i++) {
        t1.insert(i, std::to_string(i));
    }

    for (int i = 0; i < 1000000; i++) { 
        t1.erase(i);
    }

    ASSERT_TRUE(t1.empty());
}

TEST(AddressHashTableTest, default_constructor) {
    AddressHashTable<int, int> ht;

    EXPECT_EQ(ht.get_capacity(), 128);
    EXPECT_EQ(ht.get_size(), 0);
    EXPECT_EQ(ht.begin(), ht.end());
}

TEST(AddressHashTableTest, constructor_with_zero_size) {
    AddressHashTable<int, int> ht(0);

    EXPECT_EQ(ht.get_capacity(), 0);
    EXPECT_EQ(ht.get_size(), 0);
    EXPECT_EQ(ht.begin(), ht.end());
}

TEST(AddressHashTableTest, constructor_with_custom_size) {
    AddressHashTable<int, int> ht(10);

    EXPECT_EQ(ht.get_capacity(), 10);
    EXPECT_EQ(ht.get_size(), 0);
    EXPECT_EQ(ht.begin(), ht.end());
}

TEST(AddressHashTableTest, insert_single_element) {
    AddressHashTable<int, std::string> ht_int_str;

    auto it = ht_int_str.insert(1, "one");
    ASSERT_NE(it, ht_int_str.end());
    EXPECT_EQ(it->key, 1);
    EXPECT_EQ(it->value, "one");
    EXPECT_EQ(ht_int_str.get_size(), 1);

    auto found_it = ht_int_str.find(1);
    ASSERT_NE(found_it, ht_int_str.end());
    EXPECT_EQ(found_it->value, "one");
}

TEST(AddressHashTableTest, insert_multiple_elements) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(1, "one");
    ht_int_str.insert(2, "two");
    ht_int_str.insert(3, "three");

    EXPECT_EQ(ht_int_str.get_size(), 3);
    EXPECT_NE(ht_int_str.find(1), ht_int_str.end());
    EXPECT_NE(ht_int_str.find(2), ht_int_str.end());
    EXPECT_NE(ht_int_str.find(3), ht_int_str.end());
    EXPECT_EQ(ht_int_str.find(1)->value, "one");
    EXPECT_EQ(ht_int_str.find(2)->value, "two");
    EXPECT_EQ(ht_int_str.find(3)->value, "three");
}

TEST(AddressHashTableTest, insert_duplicate_key) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(1, "one");

    ASSERT_ANY_THROW(ht_int_str.insert(1, "another_one"));
    EXPECT_EQ(ht_int_str.get_size(), 1);
    EXPECT_EQ(ht_int_str.find(1)->value, "one");
}

TEST(AddressHashTableTest, insert_into_zero_capacity_table) {
    AddressHashTable<int, std::string> ht_zero(0);

    EXPECT_EQ(ht_zero.get_capacity(), 0);

    auto it = ht_zero.insert(1, "one");
    EXPECT_NE(ht_zero.get_capacity(), 0);
    EXPECT_EQ(ht_zero.get_capacity(), 1);
    EXPECT_EQ(ht_zero.get_size(), 1);
    ASSERT_NE(it, ht_zero.end());
    EXPECT_EQ(it->key, 1);
    EXPECT_EQ(it->value, "one");
}


TEST(AddressHashTableTest, insert_causes_repack) {
    AddressHashTable<int, std::string> ht(2);
    EXPECT_EQ(ht.get_capacity(), 2);

    ht.insert(1, "one");
    EXPECT_EQ(ht.get_size(), 1);
    EXPECT_EQ(ht.get_capacity(), 2);

    ht.insert(2, "two");
    EXPECT_EQ(ht.get_size(), 2);
    EXPECT_EQ(ht.get_capacity(), 4);

    ASSERT_NE(ht.find(1), ht.end());
    ASSERT_NE(ht.find(2), ht.end());
    EXPECT_EQ(ht.find(1)->value, "one");
    EXPECT_EQ(ht.find(2)->value, "two");

    ht.insert(3, "three");
    EXPECT_EQ(ht.get_size(), 3);
    EXPECT_EQ(ht.get_capacity(), 4);

    ht.insert(4, "four");
    EXPECT_EQ(ht.get_size(), 4);
    EXPECT_EQ(ht.get_capacity(), 8);

    ASSERT_NE(ht.find(3), ht.end());
    ASSERT_NE(ht.find(4), ht.end());
    EXPECT_EQ(ht.find(3)->value, "three");
    EXPECT_EQ(ht.find(4)->value, "four");
}


TEST(AddressHashTableTest, find_existing_element) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(10, "ten");
    auto it = ht_int_str.find(10);

    ASSERT_NE(it, ht_int_str.end());
    EXPECT_EQ(it->key, 10);
    EXPECT_EQ(it->value, "ten");
}

TEST(AddressHashTableTest, find_non_existing_element) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(10, "ten");
    auto it = ht_int_str.find(20);

    EXPECT_EQ(it, ht_int_str.end());
}

TEST(AddressHashTableTest, find_in_empty_table) {
    AddressHashTable<int, std::string> ht_int_str;

    auto it = ht_int_str.find(1);

    EXPECT_EQ(it, ht_int_str.end());
}

// Тесты erase(const TKey&)
TEST(AddressHashTableTest, erase_existing_element_by_key) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(1, "one");
    ht_int_str.insert(2, "two");
    EXPECT_EQ(ht_int_str.get_size(), 2);

    auto it_erase = ht_int_str.erase(1);
    ASSERT_NE(it_erase, ht_int_str.end());

    EXPECT_EQ(ht_int_str.get_size(), 1);
    EXPECT_EQ(ht_int_str.find(1), ht_int_str.end());
    ASSERT_NE(ht_int_str.find(2), ht_int_str.end());
}

TEST(AddressHashTableTest, erase_non_existing_element_by_key) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(1, "one");
    EXPECT_EQ(ht_int_str.get_size(), 1);

    auto it = ht_int_str.erase(2);
    EXPECT_EQ(it, ht_int_str.end());
    EXPECT_EQ(ht_int_str.get_size(), 1);
}

TEST(AddressHashTableTest, erase_from_empty_table_by_key) {
    AddressHashTable<int, std::string> ht_int_str;

    auto it = ht_int_str.erase(1);

    EXPECT_EQ(it, ht_int_str.end());
    EXPECT_EQ(ht_int_str.get_size(), 0);
}

TEST(AddressHashTableTest, erase_by_iterator) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(1, "one");
    ht_int_str.insert(2, "two");
    ht_int_str.insert(3, "three");

    EXPECT_EQ(ht_int_str.get_size(), 3);

    auto it_to_erase = ht_int_str.find(2);
    ASSERT_NE(it_to_erase, ht_int_str.end());

    auto next_it = ht_int_str.erase(it_to_erase);
    EXPECT_EQ(ht_int_str.get_size(), 2);
    EXPECT_EQ(ht_int_str.find(2), ht_int_str.end());

    ASSERT_NE(ht_int_str.find(1), ht_int_str.end());
    ASSERT_NE(ht_int_str.find(3), ht_int_str.end());
}

TEST(AddressHashTableTest, erase_by_end_iterator) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(1, "one");
    auto end_it = ht_int_str.end();
    auto next_it = ht_int_str.erase(end_it);

    EXPECT_EQ(next_it, ht_int_str.end());
    EXPECT_EQ(ht_int_str.get_size(), 1);
}

TEST(AddressHashTableTest, erase_by_invalid_iterator_from_other_table) {
    AddressHashTable<int, std::string> ht_int_str;
    AddressHashTable<int, std::string> other_ht;

    other_ht.insert(100, "hundred");
    auto it_other = other_ht.find(100);

    ht_int_str.insert(1, "one");
    auto next_it = ht_int_str.erase(it_other);

    EXPECT_EQ(next_it, ht_int_str.end());
    EXPECT_EQ(ht_int_str.get_size(), 1);
}

TEST(AddressHashTableTest, erase_last_element_by_iterator) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(1, "one");
    auto it = ht_int_str.find(1);
    ASSERT_NE(it, ht_int_str.end());

    auto next_it = ht_int_str.erase(it);
    EXPECT_EQ(next_it, ht_int_str.end());
    EXPECT_EQ(ht_int_str.get_size(), 0);
    EXPECT_EQ(ht_int_str.begin(), ht_int_str.end());
}


// Тесты итераторов
TEST(AddressHashTableTest, iterate_empty_table) {
    AddressHashTable<int, std::string> ht_int_str;

    EXPECT_EQ(ht_int_str.begin(), ht_int_str.end());
    int count = 0;
    for (const auto& entry : ht_int_str) {
        (void)entry;
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST(AddressHashTableTest, iterate_single_element_table) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(1, "one");

    ASSERT_NE(ht_int_str.begin(), ht_int_str.end());

    auto it = ht_int_str.begin();
    EXPECT_EQ(it->key, 1);
    EXPECT_EQ(it->value, "one");

    it++;
    EXPECT_EQ(it, ht_int_str.end());

    int count = 0;
    for (const auto& entry : ht_int_str) {
        EXPECT_EQ(entry.key, 1);
        count++;
    }
    EXPECT_EQ(count, 1);
}

TEST(AddressHashTableTest, iterate_multiple_elements) {
    AddressHashTable<int, std::string> ht_int_str;
    std::vector<int> keys_found;

    ht_int_str.insert(1, "one");
    ht_int_str.insert(2, "two");
    ht_int_str.insert(105, "one hundred five");
    for (const auto& entry : ht_int_str) {
        keys_found.push_back(entry.key);
    }

    EXPECT_EQ(keys_found.size(), 3);
    EXPECT_NE(std::find(keys_found.begin(), keys_found.end(), 1), keys_found.end());
    EXPECT_NE(std::find(keys_found.begin(), keys_found.end(), 2), keys_found.end());
    EXPECT_NE(std::find(keys_found.begin(), keys_found.end(), 105), keys_found.end());
}

TEST(AddressHashTableTest, iterate_after_erase) {
    AddressHashTable<int, std::string> ht_int_str;

    ht_int_str.insert(1, "one");
    ht_int_str.insert(2, "two");
    ht_int_str.insert(3, "three");

    ht_int_str.erase(2);

    std::vector<int> keys_found;
    for (const auto& entry : ht_int_str) {
        keys_found.push_back(entry.key);
    }

    EXPECT_EQ(keys_found.size(), 2);
    EXPECT_NE(std::find(keys_found.begin(), keys_found.end(), 1), keys_found.end());
    EXPECT_NE(std::find(keys_found.begin(), keys_found.end(), 3), keys_found.end());
    EXPECT_EQ(std::find(keys_found.begin(), keys_found.end(), 2), keys_found.end());
}

TEST(AddressHashTableTest, string_key_int_value_operations) {
    AddressHashTable<std::string, int> ht_str_int;

    ht_str_int.insert("apple", 10);
    ht_str_int.insert("banana", 20);

    EXPECT_EQ(ht_str_int.get_size(), 2);

    auto it_apple = ht_str_int.find("apple");
    ASSERT_NE(it_apple, ht_str_int.end());
    EXPECT_EQ(it_apple->value, 10);

    auto it_banana = ht_str_int.find("banana");
    ASSERT_NE(it_banana, ht_str_int.end());
    EXPECT_EQ(it_banana->value, 20);

    ht_str_int.erase("apple");
    EXPECT_EQ(ht_str_int.get_size(), 1);
    EXPECT_EQ(ht_str_int.find("apple"), ht_str_int.end());
    ASSERT_NE(ht_str_int.find("banana"), ht_str_int.end());
}

TEST(AddressHashTableTest, insert_into_full_table) {
    AddressHashTable<int, int> ht(2);

    ht.insert(1, 10);
    EXPECT_EQ(ht.get_capacity(), 2);
    EXPECT_EQ(ht.get_size(), 1);

    ht.insert(2, 20);
    EXPECT_EQ(ht.get_capacity(), 4);
    EXPECT_EQ(ht.get_size(), 2);
    ASSERT_NE(ht.find(1), ht.end());
    ASSERT_NE(ht.find(2), ht.end());

    ht.insert(3, 30);
    EXPECT_EQ(ht.get_capacity(), 4);
    EXPECT_EQ(ht.get_size(), 3);

    ht.insert(4, 40);
    EXPECT_EQ(ht.get_capacity(), 8);
    EXPECT_EQ(ht.get_size(), 4);

    ht.insert(5, 50);
    ht.insert(6, 60);
    EXPECT_EQ(ht.get_capacity(), 8);
    EXPECT_EQ(ht.get_size(), 6);

    for (int i = 7; i <= 12; ++i) {
        ht.insert(i, i * 10);
    }
    EXPECT_EQ(ht.get_size(), 12);
    EXPECT_EQ(ht.get_capacity(), 16);

    ht.insert(13, 130);
    EXPECT_EQ(ht.get_size(), 13);
    EXPECT_EQ(ht.get_capacity(), 32);

    for (int i = 1; i <= 13; ++i) {
        ASSERT_NE(ht.find(i), ht.end());
        EXPECT_EQ(ht.find(i)->value, i * 10);
    }
}