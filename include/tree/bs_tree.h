//#pragma once
//
//#include <iostream>
//#include <stdexcept>
//#include <algorithm>
//#include <cmath>
//
//template <class TKey, class TValue>
//class BSTree {
//
//	class Node {
//
//		friend class BSTree<TKey, TValue>;
//
//		TKey key;
//		TValue value;
//		Node* left;
//		Node* right;
//		Node* parent;
//
//	public:
//
//		Node(const TKey& key, const TValue& value, Node* parent = nullptr) : key(key), value(value),
//			left(nullptr), right(nullptr), parent(parent) {
//		};
//
//	};
//
//	Node* root;
//
//	void print(Node* curr) const {
//		if (!curr) return;
//		print(curr->left);
//		std::cout << curr->key << " (" << curr->value << ") ";
//		print(curr->right);
//	}
//
//	Node* find(const TKey& key, Node* curr) const {
//		if (!curr) return nullptr;
//		if (key == curr->key) return curr;
//		if (key > curr->key) return find(key, curr->right);
//		return find(key, curr->left);
//	}
//
//	Node* insert(const TKey& key, const TValue& value,
//		Node* curr, Node* nodeParent) {
//		if (!curr) return new Node(key, value, nodeParent);
//		if (key == curr->key) throw std::runtime_error("This key is already there");
//		if (key < curr->key) {
//			curr->left = insert(key, value, curr->left, curr);
//		}
//		else {
//			curr->right = insert(key, value, curr->right, curr);
//		}
//		return curr;
//	}
//
//	Node* erase(const TKey& key, Node* curr) {
//		if (!curr) throw std::runtime_error("There is no such key available");
//		if (key < curr->key) {
//			curr->left = erase(key, curr->left);
//			return curr;
//		}
//		if (key > curr->key) {
//			curr->right = erase(key, curr->right);
//			return curr;
//		}
//		if (!curr->left && !curr->right) {
//			delete curr;
//			return nullptr;
//		}
//		if (!curr->left) {
//			Node* tmp = curr->right;
//			tmp->parent = curr->parent;
//			delete curr;
//			return tmp;
//		}
//		if (!curr->right) {
//			Node* tmp = curr->left;
//			tmp->parent = curr->parent;
//			delete curr;
//			return tmp;
//		}
//		Node* m = findMin(curr->right);
//		curr->key = m->key;
//		curr->value = m->value;
//		curr->right = erase(m->key, curr->right);
//		return curr;
//	}
//
//	Node* findMin(Node* curr) const {
//		if (!curr) return nullptr;
//		Node* min = curr;
//		while (min->left) {
//			min = min->left;
//		}
//		return min;
//	}
//
//	Node* findMax(Node* curr) const {
//		if (!curr) return nullptr;
//		Node* max = curr;
//		while (max->right) {
//			max = max->right;
//		}
//		return max;
//	}
//
//	void clear_curr(Node* curr) {
//		if (curr) {
//			clear_curr(curr->left);
//			clear_curr(curr->right);
//			delete curr;
//		}
//	}
//
//	class Iterator {
//
//		Node* curr;
//
//	public:
//		Iterator(Node* it) : curr(it) {}
//
//		Node& operator*() { return *curr; }
//		Node* operator->() { return curr; }
//
//		Iterator& operator++() {
//			if (curr->right) {
//				curr = curr->right;
//				while (curr->left) {
//					curr = curr->left;
//				}
//			}
//			else {
//				if (curr->parent) {
//					while (curr != curr->parent->left) {
//						curr = curr->parent;
//						if (!curr->parent) break;
//					}
//				}
//				if (curr->parent) curr = curr->parent;
//				else curr = nullptr;
//			}
//			return *this;
//		}
//
//		Iterator operator++(int) {
//			Iterator tmp = *this;
//			++(*this);
//			return tmp;
//		}
//
//		friend bool operator==(const Iterator& it1, const Iterator& it2) {
//			return it1.curr == it2.curr;
//		}
//
//		friend bool operator!=(const Iterator& it1, const Iterator& it2) {
//			return it1.curr != it2.curr;
//		}
//
//		Iterator left() {
//			if (curr != nullptr) return curr->left;
//		}
//
//		Iterator right() {
//			if (curr != nullptr) return curr->right;
//		}
//
//		Iterator parent() {
//			return curr->parent;
//		}
//
//		TKey key() {
//			return curr->key;
//		}
//
//		TValue value() {
//			return curr->value;
//		}
//	}
//
//	int is_balance(Node* curr) {
//		if (!curr) return 0;
//		int left_h = is_balance(curr->left);
//		if (left_h == -1) return -1;
//		int right_h = is_balance(curr->right);
//		if (right_h == -1) return -1;
//
//		if (std::abs(left_h - right_h) > 1) return -1;
//		return 1 + std::max(left_h, right_h);
//	}
//
//public:
//
//	BSTree() { root = nullptr; }
//
//	~BSTree() { clear(); }
//
//	BSTree(const BSTree&) = delete;
//
//	BSTree& operator=(const BSTree&) = delete;
//
//	Iterator begin() {
//		return Iterator(findMin(root));
//	}
//
//	Iterator end() {
//		return Iterator(nullptr);
//	}
//
//	Iterator root() const {
//		return Iterator(root);
//	}
//
//	Iterator find_it(const TKey& key) const {
//		return Iterator(find(key, root));
//	}
//
//	Iterator insert_it(const TKey& key, const TValue& value) {
//		root = insert(key, value, root, nullptr);
//		return find_it(key);
//	}
//
//	Iterator erase_it(const TKey& key) {
//		root = erase(key, root);
//		return root();
//	}
//
//	bool empty() const { return root == nullptr; }
//
//	void print() const {
//		print(root);
//	}
//
//	Node* find(const TKey& key) const { return find(key, root); }
//
//	Node* insert(const TKey& key, const TValue& value) {
//		root = insert(key, value, root, nullptr);
//		return find(key);
//	}
//
//	void erase(const TKey& key) { root = erase(key, root); }
//
//	void clear() {
//		clear_curr(root);
//		root = nullptr;
//	}
//
//	bool is_balance() {
//		if (!root) return true;
//
//		return is_balance(root) != -1;
//	}
//};