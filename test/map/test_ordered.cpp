#include <gtest.h>
#include "ordered.h"

TEST(OrderedTableTest, EmptyTable) {
	OrderedTable<int, std::string> table;
	EXPECT_EQ(table.size(), 0);
	EXPECT_TRUE(table.empty());
	EXPECT_EQ(table.find(10), table.end());
	EXPECT_EQ(table.erase(10), table.end());
}

TEST(OrderedTableTest, InsertElementsInOrder) {
	OrderedTable<int, std::string> table;

	auto it1 = table.insert(2, "two");
	auto it2 = table.insert(1, "one");
	auto it3 = table.insert(3, "three");

	EXPECT_EQ(table.size(), 3);
	EXPECT_FALSE(table.empty());

	auto it = table.begin();
	EXPECT_EQ(it.key(), 1);
	EXPECT_EQ(it.value(), "one");
	++it;
	EXPECT_EQ(it.key(), 2);
	EXPECT_EQ(it.value(), "two");
	++it;
	EXPECT_EQ(it.key(), 3);
	EXPECT_EQ(it.value(), "three");
	++it;
	EXPECT_EQ(it, table.end());
}

TEST(OrderedTableTest, FindAndIndexing) {
	OrderedTable<int, std::string> table;
	table.insert(1, "one");
	table.insert(2, "two");

	EXPECT_EQ(table.find(1).value(), "one");
	EXPECT_EQ(table.find(2).value(), "two");
	EXPECT_THROW(table[3], std::out_of_range);
	EXPECT_EQ(table[1], "one");
}

TEST(OrderedTableTest, EraseExistingAndNonExisting) {
	OrderedTable<int, std::string> table;
	table.insert(1, "one");
	table.insert(2, "two");
	table.insert(3, "three");

	EXPECT_EQ(table.size(), 3);

	auto it = table.erase(2);
	EXPECT_EQ(table.size(), 2);
	EXPECT_EQ(table.find(2), table.end());

	it = table.erase(1);
	EXPECT_EQ(table.size(), 1);
	EXPECT_EQ(table.find(1), table.end());

	it = table.erase(3);
	EXPECT_EQ(table.size(), 0);
	EXPECT_TRUE(table.empty());

	// Erase from empty
	it = table.erase(10);
	EXPECT_EQ(it, table.end());
}

TEST(OrderedTableTest, IteratorIncrementOperators) {
	OrderedTable<int, std::string> table;
	table.insert(1, "a");
	table.insert(2, "b");

	auto it = table.begin();
	EXPECT_EQ(it.key(), 1);
	EXPECT_EQ((*it).second, "a");

	auto it2 = it++;
	EXPECT_EQ(it2.key(), 1);
	EXPECT_EQ(it.key(), 2);

	++it;
	EXPECT_EQ(it, table.end());
}
