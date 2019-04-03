#pragma once

#include "tree_node.h"
#include "bst_create.h"
#include "bst_print_pretty.h"

#include <stack>

namespace bst
{

namespace traversal
{

void test_print()
{
    Node* root = new Node(20);
    root->left = new Node(10);
    root->right = new Node(30);

    printPretty(root);
}

namespace recursive
{


void print_inorder(const Node* node)
{
    if (!node)
        return;

    print_inorder(node->left);
    std::cout << node->value << " ";
    print_inorder(node->right);
}

void print_preorder(const Node* node)
{
    if (!node)
        return;

    std::cout << node->value << " ";
    print_preorder(node->left);
    print_preorder(node->right);
}

void print_postorder(const Node* node)
{
    if (!node)
        return;

    print_postorder(node->left);
    print_postorder(node->right);
    std::cout << node->value << " ";
}

} // namespace recursive

namespace iterative
{

// Link: https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
// inorder: 1 2 3 4 5 6 7
// 1. left
// 2. current
// 3. right
void print_inorder(Node* node)
{
    std::stack<Node*> s;

    // push 4
    // push 2
    // push 1
    // print/pop 1
    // right -> 0
    // print/pop 2
    // goto right subtree

    Node* cur_node = node;

    while (!cur_node || !s.empty())
    {
        while (cur_node && cur_node->left)
        {
            s.push(cur_node);
            cur_node = cur_node->left;
        }

        cur_node = s.top();
        s.pop();

        std::cout << cur_node << " ";

        cur_node = cur_node->right;
    }
}

/*
         _____4____
        /          \
     __2_        __6_
    /    \      /    \
   1      3    5      7
*/

// preorder: 4 2 1 3 6 5 7
// 1. current
// 2. left
// 3. right
void print_preorder(Node* node)
{
    std::stack<Node*> s;
    Node* cur_node = node;

    while (!cur_node || !s.empty())
    {

    }
}

// postorder: 1 3 2 5 7 6 4
// 1. left
// 2. right
// 3. top
void print_postorder(Node* node)
{
}

} // namespace iterative

} // namespace traversal
} // namespace bst
