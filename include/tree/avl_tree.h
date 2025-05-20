#pragma once

#include "bs_tree.h"
#include <algorithm> // std::max
#include <cmath>     // std::abs

template <class TKey, class TValue>
class AVLTree : public BSTree<TKey, TValue> {
private:
    using Node = typename BSTree<TKey, TValue>::Node;
    using BSTree<TKey, TValue>::root_node;
    // Базовые вращения будут вызываться из этого класса
    // using BSTree<TKey, TValue>::perform_rotate_left_base; 
    // using BSTree<TKey, TValue>::perform_rotate_right_base;

    // --- AVL-специфичные вспомогательные методы ---
    void update_height(Node* node) { // Специфично для AVL
        if (node) {
            int left_h = node->left ? node->left->height : -1;
            int right_h = node->right ? node->right->height : -1;
            node->height = 1 + std::max(left_h, right_h);
        }
    }

    int get_height(Node* node) const { // Специфично для AVL
        return node ? node->height : -1;
    }

    int get_balance_factor(Node* node) const {
        return node ? (get_height(node->left) - get_height(node->right)) : 0;
    }

    // Вращения с обновлением высот
    Node* avl_rotate_left(Node* x) {
        Node* y = this->perform_rotate_left_base(x); // Используем базовое вращение
        update_height(x); // Обновляем высоту бывшего корня поддерева
        update_height(y); // Обновляем высоту нового корня поддерева
        return y;
    }

    Node* avl_rotate_right(Node* y) {
        Node* x = this->perform_rotate_right_base(y); // Используем базовое вращение
        update_height(y);
        update_height(x);
        return x;
    }

    Node* balance_node(Node* node) {
        if (!node) return nullptr;

        update_height(node);
        int balance = get_balance_factor(node);

        if (balance > 1) {
            if (get_balance_factor(node->left) < 0) {
                node->left = avl_rotate_left(node->left);
            }
            return avl_rotate_right(node);
        }
        if (balance < -1) {
            if (get_balance_factor(node->right) > 0) {
                node->right = avl_rotate_right(node->right);
            }
            return avl_rotate_left(node);
        }
        return node;
    }

    Node* insert_avl_recursive(Node* current, Node* parent_node, const TKey& key, const TValue& value, Node*& inserted_node_ref) {
        if (current == nullptr) {
            inserted_node_ref = new Node(key, value);
            inserted_node_ref->parent = parent_node;
            // update_height(inserted_node_ref); // Высота 0 по умолчанию
            return inserted_node_ref;
        }

        if (key < current->key) {
            current->left = insert_avl_recursive(current->left, current, key, value, inserted_node_ref);
        }
        else if (key > current->key) {
            current->right = insert_avl_recursive(current->right, current, key, value, inserted_node_ref);
        }
        else {
            throw std::runtime_error("AVLTree: Duplicate key.");
        }
        return balance_node(current);
    }

    Node* erase_avl_recursive(Node* current, const TKey& key, bool& erased_flag) {
        if (current == nullptr) {
            erased_flag = false;
            return nullptr;
        }

        if (key < current->key) {
            current->left = erase_avl_recursive(current->left, key, erased_flag);
        }
        else if (key > current->key) {
            current->right = erase_avl_recursive(current->right, key, erased_flag);
        }
        else {
            erased_flag = true;
            if (current->left == nullptr || current->right == nullptr) {
                Node* temp = current->left ? current->left : current->right;
                if (temp) temp->parent = current->parent;
                delete current;
                current = temp;
            }
            else {
                Node* successor = this->find_min_node_recursive(current->right);
                current->key = successor->key;
                current->value = successor->value;
                bool successor_erased_dummy;
                current->right = erase_avl_recursive(current->right, successor->key, successor_erased_dummy);
            }
        }
        if (current == nullptr) return nullptr;

        return balance_node(current);
    }

    int check_avl_balance_recursive(Node* node) const {
        if (node == nullptr) {
            return -1; // Высота пустого дерева
        }

        int left_height = check_avl_balance_recursive(node->left);
        if (left_height == -2) return -2; // Нарушение в левом поддереве

        int right_height = check_avl_balance_recursive(node->right);
        if (right_height == -2) return -2; // Нарушение в правом поддереве

        if (std::abs(left_height - right_height) > 1) {
            // std::cerr << "AVL property violated at node " << node->key 
            //           << " (LH: " << left_height << ", RH: " << right_height << ")" << std::endl;
            return -2; // Нарушение AVL
        }

        // Проверка, что сохраненная высота корректна (если вы доверяете вычисленным)
        int calculated_height = 1 + std::max(left_height, right_height);
        if (node->height != calculated_height) {
            // std::cerr << "AVL height mismatch at node " << node->key 
            //           << " (Stored: " << node->height << ", Calc: " << calculated_height << ")" << std::endl;
            return -2; // Несоответствие высоты
        }

        return calculated_height;
    }


public:
    AVLTree() = default;

    bool insert(const TKey& key, const TValue& value) override {
        Node* inserted_node_ref = nullptr;
        try {
            root_node = insert_avl_recursive(root_node, nullptr, key, value, inserted_node_ref);
            return true;
        }
        catch (const std::runtime_error&) {
            return false;
        }
    }

    bool erase(const TKey& key) override {
        bool erased_flag = false;
        if (this->find_node_recursive(root_node, key) == nullptr) return false;
        root_node = erase_avl_recursive(root_node, key, erased_flag);
        return erased_flag;
    }

    bool is_avl_balanced() const {
        return check_avl_balance_recursive(root_node) != -2;
    }
};