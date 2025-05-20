#pragma once

#include "bs_tree.h"
#include <stdexcept>

template <class TKey, class TValue>
class RBTree : public BSTree<TKey, TValue> {
private:
    using Node = typename BSTree<TKey, TValue>::Node;
    using Color = typename BSTree<TKey, TValue>::Node::Color;
    using BSTree<TKey, TValue>::root_node;

    Node* get_grandparent_rb(Node* n) const { return (n && n->parent) ? n->parent->parent : nullptr; }
    Node* get_sibling_rb(Node* n) const { return (!n || !n->parent) ? nullptr : (n == n->parent->left ? n->parent->right : n->parent->left); }
    Node* get_uncle_rb(Node* n) const { Node* p = (n ? n->parent : nullptr); return get_grandparent_rb(n) ? get_sibling_rb(p) : nullptr; }
    Color get_node_color_rb(Node* n) const { return (n == nullptr) ? Color::BLACK : n->color; }
    void set_node_color_rb(Node* n, Color c) { if (n) n->color = c; }

 
    Node* rb_rotate_left(Node* x) { return this->perform_rotate_left_base(x); }
    Node* rb_rotate_right(Node* y) { return this->perform_rotate_right_base(y); }


    void fix_after_insert(Node* x) {
        while (x != root_node && get_node_color_rb(x->parent) == Color::RED) {
            Node* parent_node = x->parent;
            Node* grandparent_node = get_grandparent_rb(x);

            if (grandparent_node == nullptr) break;

            if (parent_node == grandparent_node->left) {
                Node* uncle_node = get_uncle_rb(x);
                if (get_node_color_rb(uncle_node) == Color::RED) {
                    set_node_color_rb(parent_node, Color::BLACK);
                    set_node_color_rb(uncle_node, Color::BLACK);
                    set_node_color_rb(grandparent_node, Color::RED);
                    x = grandparent_node;
                }
                else {
                    if (x == parent_node->right) {
                        x = parent_node;
                        rb_rotate_left(x);
                        parent_node = x->parent;
                    }
                    set_node_color_rb(parent_node, Color::BLACK);
                    set_node_color_rb(grandparent_node, Color::RED);
                    rb_rotate_right(grandparent_node);
                }
            }
            else {
                Node* uncle_node = get_uncle_rb(x);
                if (get_node_color_rb(uncle_node) == Color::RED) {
                    set_node_color_rb(parent_node, Color::BLACK);
                    set_node_color_rb(uncle_node, Color::BLACK);
                    set_node_color_rb(grandparent_node, Color::RED);
                    x = grandparent_node;
                }
                else {
                    if (x == parent_node->left) {
                        x = parent_node;
                        rb_rotate_right(x);
                        parent_node = x->parent;
                    }
                    set_node_color_rb(parent_node, Color::BLACK);
                    set_node_color_rb(grandparent_node, Color::RED);
                    rb_rotate_left(grandparent_node);
                }
            }
        }
        set_node_color_rb(root_node, Color::BLACK);
    }

    void transplant_for_rb_delete(Node* u, Node* v) {
        if (u->parent == nullptr) {
            root_node = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        if (v) {
            v->parent = u->parent;
        }
    }

    void fix_after_delete(Node* x, Node* parent_of_x) {
        while (x != root_node && get_node_color_rb(x) == Color::BLACK) {
            if (parent_of_x == nullptr && x != root_node) break;

            if (x == parent_of_x->left) {
                Node* w = parent_of_x->right;
                if (w == nullptr) break;

                if (get_node_color_rb(w) == Color::RED) {
                    set_node_color_rb(w, Color::BLACK);
                    set_node_color_rb(parent_of_x, Color::RED);
                    rb_rotate_left(parent_of_x);
                    w = parent_of_x->right;
                }
                if (w == nullptr) break;

                if (get_node_color_rb(w->left) == Color::BLACK && get_node_color_rb(w->right) == Color::BLACK) {
                    set_node_color_rb(w, Color::RED);
                    x = parent_of_x;
                    parent_of_x = x->parent;
                }
                else {
                    if (get_node_color_rb(w->right) == Color::BLACK) {
                        set_node_color_rb(w->left, Color::BLACK);
                        set_node_color_rb(w, Color::RED);
                        rb_rotate_right(w);
                        w = parent_of_x->right;
                    }
                    if (w == nullptr) break;
                    set_node_color_rb(w, get_node_color_rb(parent_of_x));
                    set_node_color_rb(parent_of_x, Color::BLACK);
                    set_node_color_rb(w->right, Color::BLACK);
                    rb_rotate_left(parent_of_x);
                    x = root_node;
                }
            }
            else {
                Node* w = parent_of_x->left;
                if (w == nullptr) break;

                if (get_node_color_rb(w) == Color::RED) {
                    set_node_color_rb(w, Color::BLACK);
                    set_node_color_rb(parent_of_x, Color::RED);
                    rb_rotate_right(parent_of_x);
                    w = parent_of_x->left;
                }
                if (w == nullptr) break;

                if (get_node_color_rb(w->right) == Color::BLACK && get_node_color_rb(w->left) == Color::BLACK) {
                    set_node_color_rb(w, Color::RED);
                    x = parent_of_x;
                    parent_of_x = x->parent;
                }
                else {
                    if (get_node_color_rb(w->left) == Color::BLACK) {
                        set_node_color_rb(w->right, Color::BLACK);
                        set_node_color_rb(w, Color::RED);
                        rb_rotate_left(w);
                        w = parent_of_x->left;
                    }
                    if (w == nullptr) break;
                    set_node_color_rb(w, get_node_color_rb(parent_of_x));
                    set_node_color_rb(parent_of_x, Color::BLACK);
                    set_node_color_rb(w->left, Color::BLACK);
                    rb_rotate_right(parent_of_x);
                    x = root_node;
                }
            }
        }
        if (x) set_node_color_rb(x, Color::BLACK);
    }


    bool check_bst_property_recursive(Node* node, const TKey* min_key, const TKey* max_key) const {
        if (!node) return true;
        if ((min_key && node->key <= *min_key) || (max_key && node->key >= *max_key)) {
            return false;
        }
        return check_bst_property_recursive(node->left, min_key, &(node->key)) &&
            check_bst_property_recursive(node->right, &(node->key), max_key);
    }

    bool check_red_children_property_recursive(Node* node) const {
        if (!node) return true;
        if (get_node_color(node) == Color::RED) {
            if (get_node_color(node->left) == Color::RED || get_node_color(node->right) == Color::RED) {
                return false;
            }
        }
        return check_red_children_property_recursive(node->left) &&
            check_red_children_property_recursive(node->right);
    }

    int check_black_height_property_recursive(Node* node) const {
        if (node == nullptr) {
            return 1;
        }

        int left_bh = check_black_height_property_recursive(node->left);
        if (left_bh == -1) return -1;

        int right_bh = check_black_height_property_recursive(node->right);
        if (right_bh == -1) return -1;

        if (left_bh != right_bh) {
            return -1;
        }
        return left_bh + (get_node_color(node) == Color::BLACK ? 1 : 0);
    }

public:

    RBTree() {
        if (root_node) set_node_color_rb(root_node, Color::BLACK);
    }

    bool insert(const TKey& key, const TValue& value) override {
        Node* current = root_node;
        Node* parent = nullptr;
        while (current != nullptr) {
            parent = current;
            if (key < current->key) current = current->left;
            else if (key > current->key) current = current->right;
            else return false;
        }
        Node* new_node = new Node(key, value);
        new_node->parent = parent;
        if (!parent) root_node = new_node;
        else if (key < parent->key) parent->left = new_node;
        else parent->right = new_node;
        fix_after_insert(new_node);
        return true;
    }

    bool erase(const TKey& key) override {
        Node* z = this->find_node_recursive(root_node, key);
        if (z == nullptr) return false;

        Node* y = z;
        Node* x = nullptr;
        Node* parent_of_x = nullptr;
        Color original_color_of_y = get_node_color_rb(y);

        if (z->left == nullptr) {
            x = z->right;
            parent_of_x = z->parent;
            transplant_for_rb_delete(z, z->right);
        }
        else if (z->right == nullptr) {
            x = z->left;
            parent_of_x = z->parent;
            transplant_for_rb_delete(z, z->left);
        }
        else {
            y = this->find_min_node_recursive(z->right);
            original_color_of_y = get_node_color_rb(y);
            x = y->right;

            if (y->parent == z) {
                parent_of_x = y;
            }
            else {
                parent_of_x = y->parent;
                transplant_for_rb_delete(y, y->right);
                y->right = z->right;
                if (y->right) y->right->parent = y;
            }
            transplant_for_rb_delete(z, y);
            y->left = z->left;
            if (y->left) y->left->parent = y;
            set_node_color_rb(y, get_node_color_rb(z));
        }
        delete z;
        if (original_color_of_y == Color::BLACK) {
            fix_after_delete(x, parent_of_x);
        }
        return true;
    }

    bool check_all_rb_properties() const {
        if (root_node == nullptr) return true;
        if (get_node_color(root_node) != Color::BLACK) {
            return false;
        }
        if (!check_bst_property_recursive(root_node, nullptr, nullptr)) {
            return false;
        }
        if (!check_red_children_property_recursive(root_node)) {
            return false;
        }
        if (check_black_height_property_recursive(root_node) == -1) {
            return false;
        }
        return true;
    }
};