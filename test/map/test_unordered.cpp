#include <gtest.h>
#include "unordered.h"  // предположим, что в этом файле твой шаблон

// Для удобства: используем таблицу с int -> std::string
using Table = UnOrderedTable<int, std::string>;

TEST(UnOrderedTableTest, InsertAndFindWorks) {
	Table t;
	auto it = t.insert(1, "one");
	EXPECT_EQ(it.key(), 1);
	EXPECT_EQ(it.value(), "one");

	auto found = t.find(1);
	EXPECT_NE(found, t.end());
	EXPECT_EQ(found.value(), "one");
}

TEST(UnOrderedTableTest, InsertDuplicateThrows) {
	Table t;
	t.insert(1, "one");
	EXPECT_THROW(t.insert(1, "again"), std::runtime_error);
}

TEST(UnOrderedTableTest, OperatorSquareBracketsWorks) {
	Table t;
	t.insert(2, "two");
	EXPECT_EQ(t[2], "two");
}

TEST(UnOrderedTableTest, OperatorSquareBracketsThrowsOnMissingKey) {
	Table t;
	EXPECT_THROW((void)t[99], std::out_of_range);
}

TEST(UnOrderedTableTest, SizeAndEmpty) {
	Table t;
	EXPECT_TRUE(t.empty());
	EXPECT_EQ(t.size(), 0u);

	t.insert(1, "a");
	EXPECT_FALSE(t.empty());
	EXPECT_EQ(t.size(), 1u);
}

TEST(UnOrderedTableTest, EraseWorksAndReturnsValidIterator) {
	Table t;
	t.insert(1, "one");
	t.insert(2, "two");
	t.insert(3, "three");

	auto it = t.erase(2);
	EXPECT_EQ(t.size(), 2u);
	EXPECT_EQ(t.find(2), t.end());
	// Убедимся, что итератор валиден (если не end)
	if (it != t.end()) {
		EXPECT_TRUE(it.key() == 1 || it.key() == 3);
	}
}

TEST(UnOrderedTableTest, EraseMissingKeyReturnsEnd) {
	Table t;
	t.insert(1, "one");
	auto it = t.erase(42);
	EXPECT_EQ(it, t.end());
}

TEST(UnOrderedTableTest, IteratorBeginEnd) {
	Table t;
	t.insert(1, "one");
	t.insert(2, "two");

	std::vector<int> keys;
	for (auto it = t.begin(); it != t.end(); ++it) {
		keys.push_back(it.key());
	}

	EXPECT_EQ(keys.size(), 2u);
	EXPECT_TRUE((keys[0] == 1 && keys[1] == 2) ||
		(keys[0] == 2 && keys[1] == 1));  // порядок не гарантирован
}

TEST(UnOrderedTableTest, IteratorDereference) {
	Table t;
	t.insert(5, "five");

	auto it = t.find(5);
	EXPECT_EQ((*it).first, 5);
	EXPECT_EQ((*it).second, "five");

	EXPECT_EQ(it->first, 5);
	EXPECT_EQ(it->second, "five");
}

TEST(UnOrderedTableTest, IteratorPostfixIncrement) {
	Table t;
	t.insert(1, "a");
	t.insert(2, "b");

	auto it = t.begin();
	auto copy = it++;
	EXPECT_NE(copy, it); // postfix должен сохранить старую позицию
}

TEST(UnOrderedTableTest, BeginOnEmptyReturnsEnd) {
	Table t;
	EXPECT_EQ(t.begin(), t.end());
}
