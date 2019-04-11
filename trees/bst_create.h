#pragma once

#include "tree_node.h"
#include "binary_tree_print_pretty.h"

#include <vector>

// http://www.leetcode.com/2010/11/convert-sorted-array-into-balanced.html
// Given a sorted (increasing order) array, write an algorithm to create a binary tree with minimal height.

// 0  1  2  3  4  5  6   7   8

// 7, 9, 13, 21, 22, 28, 42, 35

// 1, 3, 4, 6, 7, 8, 10, 14, 13

// 9, 12, 14, 17, 19, 23, 50, 54, 67, 72, 76
//             ______50______
//            /              \
//        ___17___          __72__
//       /        \        /      \
//    __12__     _23_    _54_     76
//   /      \   /            \67
//   9      14 19
// We will try to create a binary tree such that for each node, the number of nodes in the left subtree and the right subtree are equal, if possible.
// Algorithm:
// 1. Insert into the tree the middle element of the array.
// 2. Insert (into the left subtree) the left subarray elements
// 3. Insert (into the right subtree) the right subarray elements
// 4. Recurse

namespace binary_tree
{

Node* create_min_bst_resursive(const std::vector<int>& v, int l, int r)
{
    if (l > r)
        return nullptr;

    int m = (l + r) / 2;

    Node * node = new Node(v[m]);
    node->left = create_min_bst_resursive(v, l, m - 1);
    node->right = create_min_bst_resursive(v, m + 1, r);

    return node;
}

Node* create_min_bst(const std::vector<int>& v)
{
    return create_min_bst_resursive(v, 0, v.size() - 1);
}

Node* test_create_from_sorted_array()
{
    //std::vector<int> v = {1, 4, 6, 12, 14, 22, 67, 111, 115};

    std::vector<int> v = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

    Node* root = create_min_bst(v);

    binary_tree::printPretty(root);

    return root;
}

// http://www.careercup.com/question?id=14942556
// Print Level order traversal of a binary tree in reverse way.
// If tree is 1
// / \
// 2 3
// / \ / \
// 4 5 6 7
// Output will be 4 5 6 7 2 3 1

} // namespace binary_tree