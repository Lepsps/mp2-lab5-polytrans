#pragma once

template <class TKey, class TValue>
struct TreeNode {
    enum class Color { BLACK, RED };

    TKey key;
    TValue value;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    Color color;
    int height;

    TreeNode(const TKey& k, const TValue& v)
        : key(k), value(v), left(nullptr), right(nullptr), parent(nullptr),
        color(Color::RED),
        height(0)
    {
    }
};