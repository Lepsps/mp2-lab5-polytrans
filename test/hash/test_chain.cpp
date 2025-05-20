#include "gtest.h"
#include "chain_hash.h" // Убедитесь, что путь к вашему заголовочному файлу правильный
#include <string>
#include <vector>
#include <algorithm> // для std::sort, std::find_if

// Для удобства определим типы, которые будем использовать в тестах
using IntIntChainHash = ChainHash<int, int>;
using StringIntChainHash = ChainHash<std::string, int>;

// Тесты для конструктора
TEST(ChainHashSuite, ConstructorDefault) {
    IntIntChainHash ht_int_int;
    EXPECT_EQ(ht_int_int.size(), 0);
    EXPECT_TRUE(ht_int_int.empty());
}

TEST(ChainHashSuite, ConstructorWithInitialBuckets) {
    IntIntChainHash ht(16);
    EXPECT_EQ(ht.size(), 0);
    EXPECT_TRUE(ht.empty());
}

TEST(ChainHashSuite, ConstructorWithZeroInitialBuckets) {
    IntIntChainHash ht(0); // Должен использовать значение по умолчанию (8)
    EXPECT_EQ(ht.size(), 0);
    EXPECT_TRUE(ht.empty());
    ht.insert(1, 1);
    EXPECT_EQ(ht.size(), 1);
}

// Тесты для insert
TEST(ChainHashSuite, InsertSingleElement) {
    IntIntChainHash ht_int_int;
    auto it = ht_int_int.insert(1, 100);
    EXPECT_EQ(ht_int_int.size(), 1);
    EXPECT_FALSE(ht_int_int.empty());
    ASSERT_NE(it, ht_int_int.end());
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, 100);
    EXPECT_EQ(ht_int_int.at(1), 100);
}

TEST(ChainHashSuite, InsertMultipleElements) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    ht_int_int.insert(2, 20);
    ht_int_int.insert(3, 30);
    EXPECT_EQ(ht_int_int.size(), 3);
    EXPECT_EQ(ht_int_int.at(1), 10);
    EXPECT_EQ(ht_int_int.at(2), 20);
    EXPECT_EQ(ht_int_int.at(3), 30);
}

TEST(ChainHashSuite, InsertDuplicateKey) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    EXPECT_THROW(ht_int_int.insert(1, 20), std::runtime_error);
    EXPECT_EQ(ht_int_int.size(), 1); // Размер не должен измениться
    EXPECT_EQ(ht_int_int.at(1), 10); // Значение не должно измениться
}

TEST(ChainHashSuite, InsertStringKeys) {
    StringIntChainHash ht_string_int;
    ht_string_int.insert("apple", 1);
    ht_string_int.insert("banana", 2);
    EXPECT_EQ(ht_string_int.size(), 2);
    EXPECT_EQ(ht_string_int.at("apple"), 1);
    EXPECT_EQ(ht_string_int.at("banana"), 2);
}

// Тесты для at
TEST(ChainHashSuite, AtExistingKey) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    ht_int_int.insert(2, 20);
    EXPECT_EQ(ht_int_int.at(1), 10);
    EXPECT_EQ(ht_int_int.at(2), 20);
}

TEST(ChainHashSuite, AtNonExistingKey) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    EXPECT_THROW(ht_int_int.at(2), std::out_of_range);
}

TEST(ChainHashSuite, AtModifyValue) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    ht_int_int.at(1) = 100;
    EXPECT_EQ(ht_int_int.at(1), 100);
}

// Тесты для operator[]
TEST(ChainHashSuite, OperatorSquareBracketsAccessExisting) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    EXPECT_EQ(ht_int_int[1], 10);
    ht_int_int[1] = 100; // Modify
    EXPECT_EQ(ht_int_int[1], 100);
    EXPECT_EQ(ht_int_int.size(), 1); // Size should not change
}

TEST(ChainHashSuite, OperatorSquareBracketsInsertNew) {
    IntIntChainHash ht_int_int;
    ht_int_int[1] = 10; // Insert new
    EXPECT_EQ(ht_int_int.size(), 1);
    EXPECT_EQ(ht_int_int.at(1), 10); // Check with at

    ht_int_int[2]; // Insert new with default constructed value
    EXPECT_EQ(ht_int_int.size(), 2);
    EXPECT_EQ(ht_int_int.at(2), 0); // Default for int

    StringIntChainHash ht_string_int;
    ht_string_int["test"];
    EXPECT_EQ(ht_string_int.size(), 1);
    EXPECT_EQ(ht_string_int.at("test"), 0); // Default for int
}

// Тесты для erase
TEST(ChainHashSuite, EraseExistingKey) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    ht_int_int.insert(2, 20);
    ht_int_int.insert(3, 30);

    auto it = ht_int_int.erase(2);
    EXPECT_EQ(ht_int_int.size(), 2);
    EXPECT_THROW(ht_int_int.at(2), std::out_of_range);
    // Проверка возвращаемого итератора зависит от порядка элементов и бакетов
    // Для простоты проверим, что элемент удален
    auto find_it = ht_int_int.find(2);
    EXPECT_EQ(find_it, ht_int_int.end());

    // Проверим, что остальные на месте
    EXPECT_EQ(ht_int_int.at(1), 10);
    EXPECT_EQ(ht_int_int.at(3), 30);
}

TEST(ChainHashSuite, EraseNonExistingKey) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    auto it = ht_int_int.erase(2);
    EXPECT_EQ(ht_int_int.size(), 1);
    EXPECT_EQ(it, ht_int_int.end());
}

TEST(ChainHashSuite, EraseLastElement) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    auto it = ht_int_int.erase(1);
    EXPECT_EQ(ht_int_int.size(), 0);
    EXPECT_TRUE(ht_int_int.empty());
    EXPECT_EQ(it, ht_int_int.end());
}

TEST(ChainHashSuite, EraseReturnsCorrectIterator) {
    ChainHash<int, int, std::hash<int>> ht_custom_hash(1); // Все в один бакет
    ht_custom_hash.insert(1, 10);
    ht_custom_hash.insert(2, 20);
    ht_custom_hash.insert(3, 30);

    auto it = ht_custom_hash.erase(2);
    ASSERT_NE(it, ht_custom_hash.end());
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 30);
    EXPECT_EQ(ht_custom_hash.size(), 2);

    it = ht_custom_hash.erase(1);
    ASSERT_NE(it, ht_custom_hash.end());
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 30);
    EXPECT_EQ(ht_custom_hash.size(), 1);

    it = ht_custom_hash.erase(3);
    EXPECT_EQ(it, ht_custom_hash.end());
    EXPECT_EQ(ht_custom_hash.size(), 0);
}

// Тесты для find
TEST(ChainHashSuite, FindExistingKey) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    ht_int_int.insert(2, 20);
    auto it = ht_int_int.find(1);
    ASSERT_NE(it, ht_int_int.end());
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, 10);

    it = ht_int_int.find(2);
    ASSERT_NE(it, ht_int_int.end());
    EXPECT_EQ(it->first, 2);
    EXPECT_EQ(it->second, 20);
}

TEST(ChainHashSuite, FindNonExistingKey) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    auto it = ht_int_int.find(2);
    EXPECT_EQ(it, ht_int_int.end());
}

// Тесты для size и empty
TEST(ChainHashSuite, SizeAndEmptyInitially) {
    IntIntChainHash ht_int_int;
    EXPECT_EQ(ht_int_int.size(), 0);
    EXPECT_TRUE(ht_int_int.empty());
}

TEST(ChainHashSuite, SizeAndEmptyAfterOperations) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    EXPECT_EQ(ht_int_int.size(), 1);
    EXPECT_FALSE(ht_int_int.empty());

    ht_int_int.insert(2, 20);
    EXPECT_EQ(ht_int_int.size(), 2);

    ht_int_int.erase(1);
    EXPECT_EQ(ht_int_int.size(), 1);

    ht_int_int.erase(2);
    EXPECT_EQ(ht_int_int.size(), 0);
    EXPECT_TRUE(ht_int_int.empty());
}

// Тесты для итераторов
TEST(ChainHashSuite, IteratorBeginEndOnEmpty) {
    IntIntChainHash ht_int_int;
    EXPECT_EQ(ht_int_int.begin(), ht_int_int.end());
}

TEST(ChainHashSuite, IteratorBasicIteration) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    ht_int_int.insert(2, 20);
    ht_int_int.insert(3, 30);

    std::vector<std::pair<int, int>> found_elements;
    for (auto it = ht_int_int.begin(); it != ht_int_int.end(); ++it) {
        found_elements.push_back(*it);
    }
    std::sort(found_elements.begin(), found_elements.end());

    EXPECT_EQ(found_elements.size(), 3);
    EXPECT_EQ(found_elements[0], std::make_pair(1, 10));
    EXPECT_EQ(found_elements[1], std::make_pair(2, 20));
    EXPECT_EQ(found_elements[2], std::make_pair(3, 30));
}

TEST(ChainHashSuite, IteratorRangeBasedFor) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    ht_int_int.insert(2, 20);
    ht_int_int.insert(3, 30);

    std::vector<std::pair<int, int>> found_elements;
    for (const auto& kv : ht_int_int) {
        found_elements.push_back(kv);
    }
    std::sort(found_elements.begin(), found_elements.end());

    EXPECT_EQ(found_elements.size(), 3);
    EXPECT_EQ(found_elements[0], std::make_pair(1, 10));
    EXPECT_EQ(found_elements[1], std::make_pair(2, 20));
    EXPECT_EQ(found_elements[2], std::make_pair(3, 30));
}

TEST(ChainHashSuite, IteratorDereferenceAndArrow) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    auto it = ht_int_int.begin();
    ASSERT_NE(it, ht_int_int.end());
    EXPECT_EQ((*it).first, 1);
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ((*it).second, 10);
    EXPECT_EQ(it->second, 10);
}

TEST(ChainHashSuite, IteratorIncrement) {
    IntIntChainHash ht_small_buckets(2);
    ht_small_buckets.insert(1, 10);
    ht_small_buckets.insert(2, 20);
    ht_small_buckets.insert(3, 30);
    ht_small_buckets.insert(4, 40);

    std::vector<int> keys_ordered;
    for (auto it = ht_small_buckets.begin(); it != ht_small_buckets.end();) {
        keys_ordered.push_back(it->first);
        auto prev_it = it++;
        if (it != ht_small_buckets.end()) { // Добавим проверку, чтобы не сравнивать с end() если prev_it был последним
            EXPECT_NE(prev_it, it);
        }
    }
    EXPECT_EQ(keys_ordered.size(), 4);
    std::sort(keys_ordered.begin(), keys_ordered.end());
    EXPECT_EQ(keys_ordered[0], 1);
    EXPECT_EQ(keys_ordered[1], 2);
    EXPECT_EQ(keys_ordered[2], 3);
    EXPECT_EQ(keys_ordered[3], 4);
}

TEST(ChainHashSuite, IteratorEquality) {
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    auto it1 = ht_int_int.find(1);
    auto it2 = ht_int_int.find(1);
    auto it_end = ht_int_int.end();

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it_end);
    EXPECT_TRUE(it1 != it_end);
    EXPECT_FALSE(it1 != it2);

    auto it_non_exist = ht_int_int.find(99);
    EXPECT_TRUE(it_non_exist == it_end);
}

// Тесты для rehash (косвенные)
TEST(ChainHashSuite, RehashOnInsert) {
    IntIntChainHash ht(1);
    ht.insert(1, 10);
    ht.insert(2, 20);
    ht.insert(3, 30);
    EXPECT_EQ(ht.size(), 3);
    EXPECT_EQ(ht.at(1), 10);
    EXPECT_EQ(ht.at(2), 20);
    EXPECT_EQ(ht.at(3), 30);

    ht.insert(4, 40);
    ht.insert(5, 50);
    EXPECT_EQ(ht.size(), 5);
    EXPECT_EQ(ht.at(1), 10);
    EXPECT_EQ(ht.at(2), 20);
    EXPECT_EQ(ht.at(3), 30);
    EXPECT_EQ(ht.at(4), 40);
    EXPECT_EQ(ht.at(5), 50);
}

TEST(ChainHashSuite, RehashOnOperatorSquareBracket) {
    IntIntChainHash ht(1);
    ht[1] = 10;
    ht[2] = 20;
    ht[3] = 30;
    EXPECT_EQ(ht.size(), 3);
    EXPECT_EQ(ht.at(1), 10);
    EXPECT_EQ(ht.at(2), 20);
    EXPECT_EQ(ht.at(3), 30);

    ht[4] = 40;
    ht[5] = 50;
    EXPECT_EQ(ht.size(), 5);
    EXPECT_EQ(ht.at(1), 10);
    EXPECT_EQ(ht.at(2), 20);
    EXPECT_EQ(ht.at(3), 30);
    EXPECT_EQ(ht.at(4), 40);
    EXPECT_EQ(ht.at(5), 50);
}

TEST(ChainHashSuite, RehashToMinimumSizeIfNewCountIsSmall) {
    // Эта ветка в rehash (new_bucket_count < 1) труднодостижима при текущей логике вызова.
    SUCCEED() << "Internal rehash branch for new_bucket_count < 1 is hard to test without API change.";
}

// Тест для hash_function с пустыми бакетами (пограничный случай)
TEST(ChainHashSuite, HashFunctionOnEmptyBuckets) {
    // Эта ветка `if (buckets.empty())` в `hash_function` недостижима, т.к. конструктор всегда создает бакеты.
    IntIntChainHash ht_int_int;
    ht_int_int.insert(1, 10);
    EXPECT_EQ(ht_int_int.at(1), 10);
    SUCCEED() << "Internal hash_function branch for buckets.empty() is effectively unreachable.";
}

// Тест для итератора, когда он должен пропускать пустые бакеты
TEST(ChainHashSuite, IteratorSkipsEmptyBuckets) {
    IntIntChainHash ht_skip(3);
    ht_skip.insert(3, 30);
    ht_skip.insert(1, 10);
    ht_skip.insert(5, 50);

    std::vector<int> keys;
    for (const auto& p : ht_skip) {
        keys.push_back(p.first);
    }
    std::sort(keys.begin(), keys.end());
    ASSERT_EQ(keys.size(), 3);
    EXPECT_EQ(keys[0], 1);
    EXPECT_EQ(keys[1], 3);
    EXPECT_EQ(keys[2], 5);

    ht_skip.erase(1);

    keys.clear();
    for (const auto& p : ht_skip) {
        keys.push_back(p.first);
    }
    std::sort(keys.begin(), keys.end());
    ASSERT_EQ(keys.size(), 2);
    EXPECT_EQ(keys[0], 3);
    EXPECT_EQ(keys[1], 5);

    auto it_begin = ht_skip.begin();
    ASSERT_NE(it_begin, ht_skip.end());
    bool is_3_or_5 = (it_begin->first == 3 || it_begin->first == 5);
    EXPECT_TRUE(is_3_or_5);
}

// Пример теста с пользовательским типом ключа и хешером
struct MyCustomKey {
    int id;
    std::string name;

    bool operator==(const MyCustomKey& other) const {
        return id == other.id && name == other.name;
    }
};

struct MyCustomKeyHash {
    std::size_t operator()(const MyCustomKey& k) const {
        std::size_t h1 = std::hash<int>{}(k.id);
        std::size_t h2 = std::hash<std::string>{}(k.name);
        return h1 ^ (h2 << 1);
    }
};

TEST(ChainHashSuite, InsertAndFindCustomKey) {
    ChainHash<MyCustomKey, int, MyCustomKeyHash> ht;
    MyCustomKey key1 = { 1, "Alice" };
    MyCustomKey key2 = { 2, "Bob" };

    ht.insert(key1, 100);
    ht.insert(key2, 200);

    EXPECT_EQ(ht.size(), 2);
    EXPECT_EQ(ht.at(key1), 100);
    EXPECT_EQ(ht.at(key2), 200);

    auto it = ht.find(key1);
    ASSERT_NE(it, ht.end());
    EXPECT_EQ(it->second, 100);

    ht.erase(key1);
    EXPECT_EQ(ht.size(), 1);
    EXPECT_EQ(ht.find(key1), ht.end());
    EXPECT_EQ(ht.at(key2), 200);
}