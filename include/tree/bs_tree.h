#pragma once

#include "tree_node.h"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm> 
#include <stack>     
#include <functional> 

template <class TKey, class TValue>
class BSTree {
public:
    using Node = TreeNode<TKey, TValue>;
    using Color = typename Node::Color;

    class Iterator {
    private:
        Node* current_node;
        std::stack<Node*> traversal_stack;
        
        void push_left_branch(Node* node) {
            while (node != nullptr) {
                traversal_stack.push(node);
                node = node->left;
            }
        }

    public:

        Iterator(Node* start_node, Node* root_of_tree_for_begin = nullptr) : current_node(nullptr) {
            if (start_node == nullptr && root_of_tree_for_begin != nullptr) {
                push_left_branch(root_of_tree_for_begin);
                if (!traversal_stack.empty()) {
                    current_node = traversal_stack.top();
                }
                else {
                    current_node = nullptr;
                }
            }
            else {
                current_node = start_node;
            }
        }

        Iterator() : current_node(nullptr) {}

        Node& operator*() const {
            if (!current_node) throw std::runtime_error("Dereferencing end or null iterator");
            return *current_node;
        }
        Node* operator->() const {
            if (!current_node) throw std::runtime_error("Dereferencing end or null iterator (->)");
            return current_node;
        }

        Iterator& operator++() {
            if (!current_node) {
                return *this;
            }
            if (current_node->right != nullptr) {
                if (!traversal_stack.empty() && traversal_stack.top() == current_node) {
                    traversal_stack.pop();
                }
                push_left_branch(current_node->right);
            }
            else {
                if (!traversal_stack.empty() && traversal_stack.top() == current_node) {
                    traversal_stack.pop();
                }
            }
            if (!traversal_stack.empty()) {
                current_node = traversal_stack.top();
            }
            else {
                current_node = nullptr;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const {
            return current_node == other.current_node;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    Iterator begin() const {
        return Iterator(nullptr, root_node);
    }
    Iterator end() const {
        return Iterator(nullptr);
    }


protected:
    Node* root_node = nullptr;

    virtual Node* insert_node_recursive(Node* current, Node* parent_node, const TKey& key, const TValue& value, Node*& inserted_node_ref) {
        if (current == nullptr) {
            inserted_node_ref = new Node(key, value);
            inserted_node_ref->parent = parent_node;
            return inserted_node_ref;
        }
        if (key < current->key) {
            current->left = insert_node_recursive(current->left, current, key, value, inserted_node_ref);
        }
        else if (key > current->key) {
            current->right = insert_node_recursive(current->right, current, key, value, inserted_node_ref);
        }
        else {
            throw std::runtime_error("BSTree: Duplicate key.");
        }
        return current;
    }

    Node* find_node_recursive(Node* current, const TKey& key) const {
        if (current == nullptr || current->key == key) {
            return current;
        }
        return (key < current->key) ? find_node_recursive(current->left, key) : find_node_recursive(current->right, key);
    }

    Node* find_min_node_recursive(Node* current) const {
        return (current && current->left) ? find_min_node_recursive(current->left) : current;
    }

    virtual Node* erase_node_recursive(Node* current, const TKey& key, bool& erased_flag) {
        if (current == nullptr) {
            erased_flag = false;
            return nullptr;
        }

        if (key < current->key) {
            current->left = erase_node_recursive(current->left, key, erased_flag);
        }
        else if (key > current->key) {
            current->right = erase_node_recursive(current->right, key, erased_flag);
        }
        else {
            erased_flag = true;
            if (current->left == nullptr) {
                Node* temp = current->right;
                if (temp) temp->parent = current->parent;
                delete current;
                return temp;
            }
            else if (current->right == nullptr) {
                Node* temp = current->left;
                if (temp) temp->parent = current->parent;
                delete current;
                return temp;
            }
            else {
                Node* successor = find_min_node_recursive(current->right);
                current->key = successor->key;
                current->value = successor->value;
                bool successor_erased_dummy;
                current->right = erase_node_recursive(current->right, successor->key, successor_erased_dummy);
            }
        }
        return current;
    }

    Node* perform_rotate_left_base(Node* x) {
        if (!x || !x->right) return x;
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        y->parent = x->parent;
        x->parent = y;
        if (T2) T2->parent = x;
        if (y->parent == nullptr) {
            root_node = y;
        }
        else if (y->parent->left == x) {
            y->parent->left = y;
        }
        else {
            y->parent->right = y;
        }
        update_node_height_for_avl(x);
        update_node_height_for_avl(y);
        return y;
    }

    Node* perform_rotate_right_base(Node* y) {
        if (!y || !y->left) return y;
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        x->parent = y->parent;
        y->parent = x;
        if (T2) T2->parent = y;
        if (x->parent == nullptr) {
            root_node = x;
        }
        else if (x->parent->left == y) {
            x->parent->left = x;
        }
        else {
            x->parent->right = x;
        }
        update_node_height_for_avl(y);
        update_node_height_for_avl(x);
        return x;
    }

    virtual void update_node_height_for_avl(Node* node) {
        if (node) {
            int left_h = node->left ? node->left->height : -1;
            int right_h = node->right ? node->right->height : -1;
            node->height = 1 + std::max(left_h, right_h);
        }
    }
    int get_node_height_for_avl(Node* node) const {
        return node ? node->height : -1;
    }

    Node* get_grandparent(Node* n) const { return (n && n->parent) ? n->parent->parent : nullptr; }
    Node* get_sibling(Node* n) const { return (!n || !n->parent) ? nullptr : (n == n->parent->left ? n->parent->right : n->parent->left); }
    Node* get_uncle(Node* n) const { Node* p = (n ? n->parent : nullptr); return get_grandparent(n) ? get_sibling(p) : nullptr; }
    Color get_node_color(Node* n) const { return (n == nullptr) ? Color::BLACK : n->color; }
    void set_node_color(Node* n, Color c) { if (n) n->color = c; }

private:

    void collect_keys_inorder_recursive(Node* node, std::vector<TKey>& keys_vec) const {
        if (node) {
            collect_keys_inorder_recursive(node->left, keys_vec);
            keys_vec.push_back(node->key);
            collect_keys_inorder_recursive(node->right, keys_vec);
        }
    }

public:
    BSTree() = default;
    virtual ~BSTree() {
        if (!root_node) return;
        std::stack<Node*> s;
        Node* current = root_node;
        Node* last_visited = nullptr;
        while (current || !s.empty()) {
            if (current) {
                s.push(current);
                current = current->left;
            }
            else {
                Node* peek_node = s.top();
                if (peek_node->right && last_visited != peek_node->right) {
                    current = peek_node->right;
                }
                else {
                    delete peek_node;
                    last_visited = s.top();
                    s.pop();
                }
            }
        }
        root_node = nullptr;
    }

    BSTree(const BSTree&) = delete;
    BSTree& operator=(const BSTree&) = delete;
    BSTree(BSTree&& other) noexcept : root_node(other.root_node) { other.root_node = nullptr; }
    BSTree& operator=(BSTree&& other) noexcept {
        if (this != &other) {
            this->~BSTree();
            root_node = other.root_node;
            other.root_node = nullptr;
        }
        return *this;
    }

    Node* find(const TKey& key) const {
        return find_node_recursive(root_node, key);
    }

    virtual bool insert(const TKey& key, const TValue& value) {
        Node* inserted_node_ref = nullptr;
        try {
            root_node = insert_node_recursive(root_node, nullptr, key, value, inserted_node_ref);
            return true;
        }
        catch (const std::runtime_error&) {
            return false;
        }
    }

    virtual bool erase(const TKey& key) {
        bool erased_flag = false;
        if (find_node_recursive(root_node, key) == nullptr) return false;
        root_node = erase_node_recursive(root_node, key, erased_flag);
        return erased_flag;
    }

    std::vector<TKey> get_all_keys_inorder() const {
        std::vector<TKey> keys_vec;
        collect_keys_inorder_recursive(root_node, keys_vec);
        return keys_vec;
    }

    void print_inorder_for_debug() const {
        std::vector<TKey> keys = get_all_keys_inorder();
        for (const auto& k : keys) { std::cout << k << " "; }
        std::cout << std::endl;
    }
};