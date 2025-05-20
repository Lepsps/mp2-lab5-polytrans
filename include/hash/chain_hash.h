#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <list>
#include <cmath>
#include <string>

template<class TKey, class TValue, class THash = std::hash<TKey>>
class ChainHash {
private:
    std::vector<std::list<std::pair<TKey, TValue>>> buckets;
    size_t num_elements = 0;
    const float max_load_factor = 2.0f;

    size_t hash_function(const TKey& key) const {
        if (buckets.empty()) return 0;
        return THash{}(key) % buckets.size();
    }

    void rehash(size_t new_bucket_count) {
        if (new_bucket_count < 1) new_bucket_count = 8;
        std::vector<std::list<std::pair<TKey, TValue>>> new_buckets(new_bucket_count);
        for (auto& bucket : buckets) {
            for (auto& kv : bucket) {
                size_t idx = THash{}(kv.first) % new_bucket_count;
                new_buckets[idx].push_back(std::move(kv));
            }
        }
        buckets.swap(new_buckets);
    }

public:
    class Iterator {
    private:
        ChainHash* table;
        size_t bucket_idx;
        typename std::list<std::pair<TKey, TValue>>::iterator list_it;

        void skip_empty_buckets() {
            while (bucket_idx < table->buckets.size() &&
                list_it == table->buckets[bucket_idx].end()) {
                ++bucket_idx;
                if (bucket_idx < table->buckets.size()) {
                    list_it = table->buckets[bucket_idx].begin();
                }
            }
        }

    public:
        Iterator(ChainHash* tbl, size_t b_idx,
            typename std::list<std::pair<TKey, TValue>>::iterator it) :
            table(tbl), bucket_idx(b_idx), list_it(it) {
            if (bucket_idx < table->buckets.size()) {
                skip_empty_buckets();
            }
        }

        std::pair<TKey, TValue>& operator*() const { return *list_it; }
        std::pair<TKey, TValue>* operator->() const { return &*list_it; }

        Iterator& operator++() {
            ++list_it;
            skip_empty_buckets();
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return table == other.table
                && bucket_idx == other.bucket_idx
                && (bucket_idx == table->buckets.size()
                    || list_it == other.list_it);
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    ChainHash(size_t initial_buckets = 8) {
        if (initial_buckets == 0) {
            buckets.resize(8);
        }
        else {
            buckets.resize(initial_buckets);
        }
    }

    Iterator insert(const TKey& key, const TValue& value) {
        size_t current_bucket_idx = hash_function(key);
        for (auto it = buckets[current_bucket_idx].begin(); it != buckets[current_bucket_idx].end(); ++it) {
            if (it->first == key) {
                throw std::runtime_error("Key already exists. Insert failed.");
            }
        }

        bool will_rehash = (static_cast<float>(num_elements + 1) / buckets.size()) > max_load_factor;
        if (buckets.size() > 1 && will_rehash) {
            rehash(buckets.size() * 2);
            current_bucket_idx = hash_function(key);
        }

        buckets[current_bucket_idx].emplace_back(key, value);
        ++num_elements;

        auto list_it = std::prev(buckets[current_bucket_idx].end());
        return Iterator(this, current_bucket_idx, list_it);
    }

    TValue& at(const TKey& key) {
        size_t idx = hash_function(key);
        for (auto& kv : buckets[idx]) {
            if (kv.first == key) {
                return kv.second;
            }
        }
        throw std::out_of_range("Key not found");
    }

    TValue& operator[](const TKey& key) {
        size_t current_bucket_idx = hash_function(key);
        for (auto it = buckets[current_bucket_idx].begin(); it != buckets[current_bucket_idx].end(); ++it) {
            if (it->first == key) {
                return it->second;
            }
        }

        bool will_rehash = (static_cast<double>(num_elements + 1) / buckets.size()) > max_load_factor;
        if (buckets.size() > 1 && will_rehash) {
            rehash(buckets.size() * 2);
            current_bucket_idx = hash_function(key);
        }

        buckets[current_bucket_idx].emplace_back(key, TValue{});
        ++num_elements;

        return buckets[current_bucket_idx].back().second;
    }

    Iterator erase(const TKey& key) {
        size_t idx = hash_function(key);
        auto& lst = buckets[idx];
        for (auto it = lst.begin(); it != lst.end(); ++it) {
            if (it->first == key) {
                auto next_it = it;
                ++next_it;

                lst.erase(it);
                --num_elements;

                return Iterator(this, idx, next_it);
            }
        }
        return end();
    }

    Iterator find(const TKey& key) {
        size_t idx = hash_function(key);
        for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it) {
            if (it->first == key) {
                return Iterator(this, idx, it);
            }
        }
        return end();
    }

    size_t size() const {
        return num_elements;
    }

    bool empty() const {
        return num_elements == 0;
    }

    Iterator begin() {
        if (num_elements == 0) {
            return end();
        }
        return Iterator(this, 0, buckets[0].begin());
    }

    Iterator end() {
        return Iterator(this,
            buckets.size(),
            typename std::list<std::pair<TKey, TValue>>::iterator{});
    }
};
