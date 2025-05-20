#pragma once
#include <vector>
#include <string>
#include <functional>

template <class TKey, class TValue>
class AddressHashTable {
public:
	enum class Status {
		EMPTY,
		OCCUPIED,
		ERASED
	};

	struct Entry {
		TKey key;
		TValue value;
		Status status;

		Entry() : status(Status::EMPTY) {}
		Entry(const TKey& k, const TValue& v) : key(k), value(v), status(Status::OCCUPIED) {}
	};

private:
	size_t m;
	size_t n = 0;
	std::vector<Entry> data;

	size_t hash_function(const TKey& key, size_t attempt) const {
		if (m == 0) {
			throw "zero capacity error";
		}
		return (std::hash<TKey>{}(key)+attempt) % m;
	}

	void repack() {
		size_t old_m = m;
		std::vector<Entry> old_data = std::move(data);

		m = (old_m == 0) ? 1 : old_m * 2;

		data.assign(m, Entry());
		n = 0;

		for (size_t i = 0; i < old_m; ++i) {
			if (old_data[i].status == Status::OCCUPIED) {
				for (size_t j = 0; j < m; ++j) {
					size_t index = hash_function(old_data[i].key, j);
					if (data[index].status != Status::OCCUPIED) {
						data[index].key = old_data[i].key;
						data[index].value = old_data[i].value;
						data[index].status = Status::OCCUPIED;
						n++;
						break;
					}
				}
			}
		}
	}

public:

	AddressHashTable(size_t initial_m = 128) : m(initial_m) {
		if (m > 0) {
			data.resize(m);
		}
	}

	~AddressHashTable() = default;

	struct Iterator {
		AddressHashTable* ht_ptr;
		Entry* ptr;

		Iterator(AddressHashTable* ht, Entry* p) : ht_ptr(ht), ptr(p) {}

		Entry& operator*() const { return *ptr; }
		Entry* operator->() const { return ptr; }

		Iterator& operator++() {
			if (!ht_ptr || ptr == nullptr) return *this;

			Entry* const data_start = ht_ptr->data.data();
			Entry* const data_end = (ht_ptr->m == 0) ? nullptr : data_start + ht_ptr->m;

			if (ptr >= data_end) {
				ptr = data_end;
				return *this;
			}

			do {
				ptr++;
			} while (ptr < data_end && ptr->status != Status::OCCUPIED);
			return *this;
		}

		Iterator operator++(int) {
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		friend bool operator==(const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; }
		friend bool operator!=(const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; }
	};

	Iterator begin() {
		if (m == 0 || n == 0) return end();
		Entry* current = data.data();
		Entry* const data_end_ptr = data.data() + m;
		while (current < data_end_ptr && current->status != Status::OCCUPIED) {
			current++;
		}
		return Iterator(this, current);
	}

	Iterator end() {
		if (m == 0) return Iterator(this, nullptr);
		return Iterator(this, data.data() + m);
	}

	Iterator insert(const TKey& key, const TValue& value) {
		if (m == 0 || (m > 0 && static_cast<double>(n + 1) / m > 0.8)) {
			repack();
		}

		if (m == 0) {
			throw "repack error";
		}

		size_t first_erased_slot = m + 1;

		for (size_t i = 0; i < m; ++i) {
			size_t index = hash_function(key, i);
			if (data[index].status == Status::OCCUPIED && data[index].key == key) {
				throw "Delete a key";
			}
			if (data[index].status == Status::ERASED) {
				if (first_erased_slot > m) {
					first_erased_slot = index;
				}
			}
			if (data[index].status == Status::EMPTY) {
				size_t insert_index = (first_erased_slot <= m) ? first_erased_slot : index;

				data[insert_index].key = key;
				data[insert_index].value = value;
				data[insert_index].status = Status::OCCUPIED;
				n++;
				return Iterator(this, &data[insert_index]);
			}
		}

		if (first_erased_slot <= m) {
			data[first_erased_slot].key = key;
			data[first_erased_slot].value = value;
			data[first_erased_slot].status = Status::OCCUPIED;
			n++;
			return Iterator(this, &data[first_erased_slot]);
		}

		repack();
		for (size_t i = 0; i < m; ++i) { 
			size_t index = hash_function(key, i);
			if (data[index].status != Status::OCCUPIED) {
				data[index].key = key;
				data[index].value = value;
				data[index].status = Status::OCCUPIED;
				n++;
				return Iterator(this, &data[index]);
			}
		}
		throw "Hast table is full";
	}

	Iterator erase(const TKey& key) {
		if (m == 0 || n == 0) return end();

		for (size_t i = 0; i < m; ++i) {
			size_t index = hash_function(key, i);
			if (data[index].status == Status::EMPTY) {
				return end();
			}
			if (data[index].status == Status::OCCUPIED && data[index].key == key) {
				data[index].status = Status::ERASED;
				n--;
				return Iterator(this, &data[index]);
			}
		}
		return end();
	}

	Iterator erase(Iterator pos) {
		if (pos.ht_ptr != this || pos.ptr == nullptr ||
			(m > 0 && (pos.ptr < data.data() || pos.ptr >= data.data() + m)) ||
			(m == 0 && pos.ptr != nullptr)
			) {
			return end();
		}
		if (pos == end()) return end();

		if (pos.ptr->status == Status::OCCUPIED) {
			pos.ptr->status = Status::ERASED;
			n--;
			Iterator next_it = pos;
			return ++next_it;
		}
		Iterator current_pos_copy = pos;
		return ++current_pos_copy;
	}

	Iterator find(const TKey& key) {
		if (m == 0 || n == 0) return end();

		for (size_t i = 0; i < m; ++i) { 
			size_t index = hash_function(key, i);
			if (data[index].status == Status::EMPTY) {
				return end();
			}
			if (data[index].status == Status::OCCUPIED && data[index].key == key) {
				return Iterator(this, &data[index]);
			}
		}
		return end();
	}

	size_t get_size() const {
		return n;
	}

	size_t get_capacity() const {
		return m;
	}

	bool empty() {
		return n == 0;
	}
};