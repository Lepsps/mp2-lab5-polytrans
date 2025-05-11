#pragma once


#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>


template<class TKey, class TValue>
class OrderedTable {

	std::vector<std::pair<TKey, TValue>> data;
	
public:

	class Iterator {

		std::pair<TKey, TValue>* curr;

	public:
		Iterator(std::pair<TKey, TValue>* it) : curr(it) {}

		std::pair<TKey, TValue>& operator*() { return *curr; }
		std::pair<TKey, TValue>* operator->() { return curr; }

		TKey key() { return curr->first; }
		TValue& value() { return curr->second; }

		Iterator& operator++() {
			curr++;
			return *this;
		}

		Iterator operator++(int) {
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		friend bool operator==(const Iterator& it1, const Iterator& it2) {
			return it1.curr == it2.curr;
		}

		friend bool operator!=(const Iterator& it1, const Iterator& it2) {
			return it1.curr != it2.curr;
		}

	};


	Iterator begin() {
		if (data.empty()) return Iterator(nullptr);
		return Iterator(&data[0]);
	}

	Iterator end() {
		return Iterator(data.data() + data.size());
	}

	Iterator insert(const TKey& key, const TValue& value) {
		size_t i;
		for (i = 0; i < data.size(); i++) {
			if (key < data[i].first) {
				data.emplace_back(key, value);
				for (size_t j = data.size() - 1; j > i; j--) {
					std::swap(data[j], data[j - 1]);
				}

				return Iterator(&data[i]);
			}
		}

		data.insert(data.begin() + i, std::make_pair(key, value));
		return Iterator(&data[i]);
	};

	Iterator erase(const TKey& key) {
		if (data.empty()) return end();

		int erase_index = -1;
		for (size_t i = 0; i < data.size(); i++) {
			if (data[i].first == key) {
				erase_index = i;
				if (i < data.size() - 1) {
					std::swap(data[i], data[data.size() - 1]);
				}
				data.pop_back();

				if (i < data.size()) {
					return Iterator(&data[erase_index]);
				}
				else return end();
			}
		}

		return end();

	};

	Iterator find(const TKey& key) {
		for (size_t i = 0; i < data.size(); i++) {
			if (data[i].first == key) {
				return Iterator(&data[i]);
			}
		}
		return end();
	};

	size_t size() {
		return data.size();
	}

	bool empty() {
		return data.empty();
	}

	TValue& operator[](const TKey& key) {
		auto it = find(key);

		if (it == end()) throw std::out_of_range("Out of range");

		return it.value();
	}
};