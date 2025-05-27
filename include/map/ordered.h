#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>

template<class TKey, class TValue>
class OrderedTable {

	std::vector<std::pair<TKey, TValue>> data;

public:

	class Iterator {
		friend class OrderedTable;
		std::pair<TKey, TValue>* curr;

	public:
		Iterator(std::pair<TKey, TValue>* it) : curr(it) {}

		std::pair<TKey, TValue>& operator*() { return *curr; }
		std::pair<TKey, TValue>* operator->() { return curr; }

		TKey key() const { return curr->first; }
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
		return Iterator(data.data());
	}

	Iterator end() {
		return Iterator(data.data() + data.size());
	}

	Iterator insert(const TKey& key, const TValue& value) {
		int low = 0;
		int high = static_cast<int>(data.size()) - 1;
		size_t insert_idx = data.size();

		while (low <= high) {
			int mid = low + (high - low) / 2;

			if (data[mid].first < key) {
				low = mid + 1;
			}
			else {
				insert_idx = mid;
				high = mid - 1;
			}
		}

		auto inserted_it_vec = data.insert(data.begin() + insert_idx, std::make_pair(key, value));
		return Iterator(&(*inserted_it_vec));
	};

	Iterator erase(const TKey& key) {
		Iterator it_to_erase = find(key);

		if (it_to_erase == end()) {
			return end();
		}

		size_t offset = static_cast<size_t>(it_to_erase.curr - data.data());
		auto vec_it_to_erase = data.begin() + offset;

		auto next_vec_it = data.erase(vec_it_to_erase);

		return Iterator(data.data() + (next_vec_it - data.begin()));
	};

	Iterator find(const TKey& key) {
		if (data.empty()) {
			return end();
		}

		int low = 0;
		int high = static_cast<int>(data.size()) - 1;

		while (low <= high) {
			int mid = low + (high - low) / 2; 

			if (data[mid].first == key) {
				return Iterator(&data[mid]);
			}
			else if (data[mid].first < key) {
				low = mid + 1;
			}
			else {
				high = mid - 1;
			}
		}
		return end();
	};

	size_t size() const {
		return data.size();
	}

	bool empty() const {
		return data.empty();
	}

	TValue& operator[](const TKey& key) {
		Iterator it = find(key);

		if (it == end()) {
			throw std::out_of_range("Key not found in OrderedTable");
		}
		return it.value();
	}
};