#include "cyclic_list.h"
#include "polynoms.h"

#include <gtest.h>

#include <string>
#include <vector>
#include <cmath>
 
#include <algorithm>
#include <random>


template <typename T>
std::vector<T> toVector(const CyclicList<T>& list) {
    std::vector<T> result;
    if (!list.empty()) {
        for (size_t i = 0; i < list.size(); ++i) {
            result.push_back(list[i]);
        }
    }
    return result;
}

// Helper function to check equality of CyclicList and vector
template <typename T>
bool areEqual(const CyclicList<T>& list, const std::vector<T>& vec) {
    return toVector(list) == vec;
}


TEST(CyclicList, DefaultConstructor) {
    CyclicList<int> list;
    ASSERT_TRUE(list.empty());
    ASSERT_EQ(list.size(), 0);
}

TEST(CyclicList, PushBack) {
    CyclicList<int> list;
    list.push_back(1);
    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list[0], 1);

    list.push_back(2);
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list[0], 1);
    ASSERT_EQ(list[1], 2);

    list.push_back(3);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list[0], 1);
    ASSERT_EQ(list[1], 2);
    ASSERT_EQ(list[2], 3);

    std::vector<int> expected = { 1, 2, 3 };
    ASSERT_TRUE(areEqual(list, expected));
}

TEST(CyclicList, PushFront) {
    CyclicList<int> list;
    list.push_front(1);
    ASSERT_FALSE(list.empty());
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list[0], 1);

    list.push_front(2);
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list[0], 2);
    ASSERT_EQ(list[1], 1);

    list.push_front(3);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list[0], 3);
    ASSERT_EQ(list[1], 2);
    ASSERT_EQ(list[2], 1);

    std::vector<int> expected = { 3, 2, 1 };
    ASSERT_TRUE(areEqual(list, expected));
}

TEST(CyclicList, PopBack) {
    CyclicList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_back();
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list[0], 1);
    ASSERT_EQ(list[1], 2);

    list.pop_back();
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list[0], 1);

    list.pop_back();
    ASSERT_TRUE(list.empty());

    ASSERT_THROW(list.pop_back(), std::out_of_range);
}

TEST(CyclicList, PopFront) {
    CyclicList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.pop_front();
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list[0], 2);
    ASSERT_EQ(list[1], 3);

    list.pop_front();
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list[0], 3);

    list.pop_front();
    ASSERT_TRUE(list.empty());

    ASSERT_THROW(list.pop_front(), std::out_of_range);
}

TEST(CyclicList, Size) {
    CyclicList<int> list;
    ASSERT_EQ(list.size(), 0);

    list.push_back(1);
    ASSERT_EQ(list.size(), 1);

    list.push_back(2);
    ASSERT_EQ(list.size(), 2);

    list.pop_front();
    ASSERT_EQ(list.size(), 1);

    list.pop_back();
    ASSERT_EQ(list.size(), 0);
}

TEST(CyclicList, IndexOperator) {
    CyclicList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    ASSERT_EQ(list[0], 10);
    ASSERT_EQ(list[1], 20);
    ASSERT_EQ(list[2], 30);

    list[0] = 15;
    ASSERT_EQ(list[0], 15);

    const CyclicList<int>& constList = list;
    ASSERT_EQ(constList[1], 20);


    ASSERT_THROW(list[3], std::out_of_range);
    ASSERT_THROW(constList[3], std::out_of_range);
}

TEST(CyclicList, Find) {
    CyclicList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_back(20);


    ASSERT_EQ(list.find(20), 1);
    ASSERT_EQ(list.find(10), 0);
    ASSERT_EQ(list.find(30), 2);
    ASSERT_EQ(list.find(40), -1);

    CyclicList<int> emptyList;
    ASSERT_EQ(emptyList.find(5), -1);
}

TEST(CyclicList, Insert) {
    CyclicList<int> list;
    list.insert(0, 10);
    ASSERT_TRUE(areEqual(list, { 10 }));

    list.insert(0, 5);
    ASSERT_TRUE(areEqual(list, { 5, 10 }));

    list.insert(2, 15);
    ASSERT_TRUE(areEqual(list, { 5, 10, 15 }));

    list.insert(1, 7);
    ASSERT_TRUE(areEqual(list, { 5, 7, 10, 15 }));


    ASSERT_THROW(list.insert(5, 20), std::out_of_range);
}

TEST(CyclicList, Erase) {
    CyclicList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_back(40);

    list.erase(1);
    ASSERT_TRUE(areEqual(list, { 10, 30, 40 }));

    list.erase(0);
    ASSERT_TRUE(areEqual(list, { 30, 40 }));

    list.erase(1);
    ASSERT_TRUE(areEqual(list, { 30 }));

    list.erase(0);
    ASSERT_TRUE(list.empty());

    CyclicList<int> empty_list;
    ASSERT_THROW(empty_list.erase(0), std::out_of_range);
    list.push_back(10);
    ASSERT_THROW(list.erase(1), std::out_of_range);

}

TEST(CyclicList, CopyConstructor) {
    CyclicList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    CyclicList<int> copyList(list);
    ASSERT_TRUE(areEqual(list, { 1, 2, 3 }));
    ASSERT_TRUE(areEqual(copyList, { 1, 2, 3 }));

    copyList[0] = 10;
    ASSERT_TRUE(areEqual(list, { 1, 2, 3 }));  // Ensure deep copy
    ASSERT_TRUE(areEqual(copyList, { 10, 2, 3 }));


    CyclicList<int> empty_list;
    CyclicList<int> copy_empty(empty_list);
    ASSERT_TRUE(copy_empty.empty());

}

TEST(CyclicList, AssignmentOperator) {
    CyclicList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    CyclicList<int> assignedList;
    assignedList = list;
    ASSERT_TRUE(areEqual(list, { 1, 2, 3 }));
    ASSERT_TRUE(areEqual(assignedList, { 1, 2, 3 }));

    assignedList[0] = 10;
    ASSERT_TRUE(areEqual(list, { 1, 2, 3 }));  // Ensure deep copy
    ASSERT_TRUE(areEqual(assignedList, { 10, 2, 3 }));

    CyclicList<int> empty_list;
    CyclicList<int> assigned_empty;
    assigned_empty = empty_list;
    ASSERT_TRUE(assigned_empty.empty());

    assigned_empty = list;
    list.clear();
    ASSERT_FALSE(assigned_empty.empty()); // Check assigned list still valid

    assignedList = assignedList; //self assignment
    ASSERT_TRUE(areEqual(assignedList, { 10, 2, 3 }));


    CyclicList<int> shortList;
    shortList.push_back(5);
    assignedList = shortList;
    ASSERT_TRUE(areEqual(assignedList, { 5 }));

}

TEST(CyclicList, Clear) {
    CyclicList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.clear();
    ASSERT_TRUE(list.empty());
    ASSERT_EQ(list.size(), 0);

    list.clear(); // Clear an empty list
    ASSERT_TRUE(list.empty());
    ASSERT_EQ(list.size(), 0);
}

TEST(CyclicList, Sort) {
    CyclicList<int> list;
    list.push_back(3);
    list.push_back(1);
    list.push_back(4);
    list.push_back(1);
    list.push_back(5);
    list.push_back(9);
    list.push_back(2);
    list.push_back(6);

    list.sort();
    ASSERT_TRUE(areEqual(list, { 1, 1, 2, 3, 4, 5, 6, 9 }));

    list.sort(true);
    ASSERT_TRUE(areEqual(list, { 9, 6, 5, 4, 3, 2, 1, 1 }));

    CyclicList<int> empty_list;
    empty_list.sort();
    ASSERT_TRUE(empty_list.empty());
}

TEST(CyclicList, Reverse) {
    CyclicList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    list.reverse();
    ASSERT_TRUE(areEqual(list, { 5, 4, 3, 2, 1 }));

    CyclicList<int> empty_list;
    empty_list.reverse();
    ASSERT_TRUE(empty_list.empty());
}

TEST(CyclicList, Count) {
    CyclicList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(2);
    list.push_back(3);
    list.push_back(2);

    ASSERT_EQ(list.count(2), 3);
    ASSERT_EQ(list.count(1), 1);
    ASSERT_EQ(list.count(3), 1);
    ASSERT_EQ(list.count(4), 0);

    CyclicList<int> empty_list;
    ASSERT_EQ(empty_list.count(5), 0);
}

// Heavy tests to try to trigger errors

TEST(CyclicList, LargePushBack) {
    CyclicList<int> list;
    std::vector<int> expected;
    const int numElements = 10000;

    for (int i = 0; i < numElements; ++i) {
        list.push_back(i);
        expected.push_back(i);
    }

    ASSERT_EQ(list.size(), numElements);
    ASSERT_TRUE(areEqual(list, expected));
}

TEST(CyclicList, LargePushFront) {
    CyclicList<int> list;
    std::vector<int> expected;
    const int numElements = 10000;

    for (int i = 0; i < numElements; ++i) {
        list.push_front(i);
        expected.insert(expected.begin(), i); // Insert at beginning to match push_front
    }

    ASSERT_EQ(list.size(), numElements);
    ASSERT_TRUE(areEqual(list, expected));
}

TEST(CyclicList, LargeInsert) {
    CyclicList<int> list;
    std::vector<int> expected;
    const int numElements = 5000;

    for (int i = 0; i < numElements; ++i) {
        list.insert(i / 2, i);
        expected.insert(expected.begin() + i / 2, i);
    }

    ASSERT_EQ(list.size(), numElements);
    ASSERT_TRUE(areEqual(list, expected));
}

TEST(CyclicList, LargeErase) {
    CyclicList<int> list;
    std::vector<int> expected;
    const int numElements = 5000;

    for (int i = 0; i < numElements; ++i) {
        list.push_back(i);
        expected.push_back(i);
    }

    for (int i = 0; i < numElements; i += 2)
    {
        list.erase(i / 2);
        expected.erase(expected.begin() + i / 2);
    }

    ASSERT_EQ(list.size(), expected.size());
    ASSERT_TRUE(areEqual(list, expected));
}



TEST(CyclicList, RandomOperations) {
    CyclicList<int> list;
    std::vector<int> expected;
    const int numOperations = 2000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 5); // Choose operation: 0-push_back, 1-push_front, 2-insert, 3-erase, 4-sort, 5-reverse

    for (int i = 0; i < numOperations; ++i) {
        int operation = distrib(gen);

        switch (operation) {
        case 0: { // push_back
            int value = i;
            list.push_back(value);
            expected.push_back(value);
            break;
        }
        case 1: { // push_front
            int value = i;
            list.push_front(value);
            expected.insert(expected.begin(), value);
            break;
        }
        case 2: { // insert
            if (!expected.empty()) {
                std::uniform_int_distribution<> insertDistrib(0, expected.size());
                size_t index = insertDistrib(gen);
                int value = i;
                try {
                    list.insert(index, value);
                    expected.insert(expected.begin() + index, value);
                }
                catch (const std::out_of_range& e) {
                    // Ignore out of range errors since they are expected sometimes
                }
            }
            else {
                int value = i;
                list.push_back(value);
                expected.push_back(value);
            }
            break;
        }
        case 3: { // erase
            if (!expected.empty()) {
                std::uniform_int_distribution<> eraseDistrib(0, expected.size() - 1);
                size_t index = eraseDistrib(gen);
                try {
                    list.erase(index);
                    expected.erase(expected.begin() + index);
                }
                catch (const std::out_of_range& e) {
                    // Ignore out of range errors since they are expected sometimes
                }
            }
            break;
        }
        case 4: { // sort
            list.sort();
            std::sort(expected.begin(), expected.end());
            break;
        }
        case 5: { // reverse
            list.reverse();
            std::reverse(expected.begin(), expected.end());
            break;
        }
        }

        if (i % 1000 == 0) { // Check every 1000 operations to avoid excessive overhead
            ASSERT_EQ(list.size(), expected.size());
            ASSERT_TRUE(areEqual(list, expected));
        }
    }
}

TEST(CyclicList, SelfAssignment) {
    CyclicList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list = list; // Self-assignment
    ASSERT_TRUE(areEqual(list, { 1, 2, 3 }));

    CyclicList<int> emptyList;
    emptyList = emptyList; // Self-assignment on empty list
    ASSERT_TRUE(emptyList.empty());
}

TEST(CyclicList, MoveConstructorNotImplemented) {
    CyclicList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    // This will call the copy constructor, since move constructor is not explicitly defined
    CyclicList<int> movedList = std::move(list);

    // After the "move," the original list 'list' should still be valid in some state
    // (it's implementation defined what that state is if there's no explicit move constructor)
    ASSERT_TRUE(areEqual(movedList, { 1, 2, 3 }));
    //The code does NOT define move constructor, so `list` will contain {1,2,3}. If move constructor was defined, list would be empty.
    ASSERT_TRUE(areEqual(list, { 1, 2, 3 }));
}