#pragma once

#include "tree_node.h"
#include "bst_create.h"
#include "binary_tree_print_pretty.h"

#include <stack>

namespace binary_tree
{

namespace traversal_dfs
{

void test_print()
{
    Node* root = new Node(20);
    root->left = new Node(10);
    root->right = new Node(30);

    binary_tree::printPretty(root);
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

    /*
             ____5_____
            /          \
         __2_         __7_
        /    \       /    \
       1      3_    6      8_
                \            \
                 4            9
    */


// Link: https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
// inorder: 1 2 3 4 5 6 7 8 9
// 1. left
// 2. current
// 3. right
void print_inorder(const Node* node)
{
    std::stack<const Node*> s;

    // push 4
    // push 2
    // push 1
    // print/pop 1
    // right -> 0
    // print/pop 2
    // goto right subtree

    const Node* cur_node = node;

    while (cur_node || !s.empty())
    {
        while (cur_node)
        {
            s.push(cur_node);
            cur_node = cur_node->left;
        }

        if(!s.empty())
        {
            cur_node = s.top();
            s.pop();
        }

        if(cur_node)
        {
            std::cout << cur_node->value << " ";
            cur_node = cur_node->right;
        }
    }
}

// preorder: 5 2 1 3 4 7 6 8 9
// 1. current
// 2. left
// 3. right
void print_preorder(const Node* node)
{
    std::stack<const Node*> s;
    const Node* cur_node = node;

    while (cur_node || !s.empty())
    {
        while (cur_node)
        {
            std::cout << cur_node->value << " ";
            if(cur_node->right)
                s.push(cur_node->right);
            cur_node = cur_node->left;
        }

        if(!s.empty())
        {
            cur_node = s.top();
            s.pop();
        }
    }
}

/*
         ____5_____
        /          \
     __2_         __7_
    /    \       /    \
   1      3_    6      8_
            \            \
             4            9
*/

// https://www.geeksforgeeks.org/iterative-postorder-traversal-using-stack/

// postorder: 1 4 3 2 6 9 8 7 5
// 1. left
// 2. right
// 3. current
void print_postorder(const Node* node)
{
    std::stack<const Node*> s;
    
    const Node* cur_node = node;

    do
    {
        // Move to leftmost node
        while (cur_node)
        {
            // Push root's right child and then root to stack.
            if(cur_node->right)
                s.push(cur_node->right);
            s.push(cur_node);
            
            // Set root as root's left child
            cur_node = cur_node->left;
        }

        cur_node = s.top();
        s.pop();

        // If the popped item has a right child and the right child is not 
        // processed yet, then make sure right child is processed before root 
        if (cur_node->right && (!s.empty() && (s.top() == cur_node->right)))
        {
            s.pop();                    // remove right child from stack
            s.push(cur_node);           // push root back to stack 
            cur_node = cur_node->right; // change root so that the right  
                                        // child is processed nex
        }
        else
        {
            std::cout << cur_node->value << " ";
        }

    }
    while((!s.empty()));


    /*
    while(!s.empty())
    {
        const Node* node = s.top();
        std::cout << node->value << " ";
        s.pop();
    }
    */
}

} // namespace iterative

} // namespace traversal_dfs
} // namespace binary_tree
