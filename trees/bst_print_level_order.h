#pragma once

#include "tree_node.h"

#include <queue>
#include <iostream>

// http://www.leetcode.com/2010/09/printing-binary-tree-in-level-order.html

namespace bst
{

//        ______30______           level 0
//       /              \
//    __20__          __40__       level 1
//   /      \        /      \
//  10      25      35      50     level 2
// /  \       \            /
// 5  15      28          41       level 3
void printLevelOrderTwoQueues(Node *root)
{
    if (!root) 
        return;

    std::queue<Node*> currentLevel, nextLevel;
    currentLevel.push(root);
  
    while (!currentLevel.empty()) 
    {
        Node *currNode = currentLevel.front();
        currentLevel.pop();
        if(currNode)
        {
            std::cout << currNode->value << " ";
            nextLevel.push(currNode->left);
            nextLevel.push(currNode->right);
        }
        if (currentLevel.empty())
        {
            std::cout << std::endl;
            std::swap(currentLevel, nextLevel);
        }
    }
}

void printLevelOrderOneQueue(Node *root)
{
    if (!root) 
        return;
    std::queue<Node*> nodesQueue;
    int nodesInCurrentLevel = 1;
    int nodesInNextLevel = 0;
    nodesQueue.push(root);
  
    while (!nodesQueue.empty()) 
    {
        Node *currNode = nodesQueue.front();
        nodesQueue.pop();
        nodesInCurrentLevel--;
        if (currNode) 
        {
            std::cout << currNode->value << " ";
            nodesQueue.push(currNode->left);
            nodesQueue.push(currNode->right);
            nodesInNextLevel += 2;
        }
        if (nodesInCurrentLevel == 0) 
        {
            std::cout << std::endl;
            nodesInCurrentLevel = nodesInNextLevel;
            nodesInNextLevel = 0;
        }
    }
}


int test_print_level_order()
{
    std::cout << "BST PRINT LEVEL ORDER" << std::endl;
  
    Node *root = new Node(30);
    root->left = new Node(20);
    root->right = new Node(40);
    root->left->left = new Node(10);
    root->left->right = new Node(25);
    root->right->left = new Node(35);
    root->right->right = new Node(50);
    root->left->left->left = new Node(5);
    root->left->left->right = new Node(15);
    root->left->right->right = new Node(28);
    root->right->right->left = new Node(41);

    std::cout << "Using one queue:" << std::endl;
    printLevelOrderOneQueue(root);

    std::cout << "Using two queue:" << std::endl;
    printLevelOrderTwoQueues(root);

    return 0;
}

// http://www.leetcode.com/2010/09/binary-tree-level-order-traversal-using_17.html

void printLevel(Node *p, int level) {
  if (!p) return;
  if (level == 1) {
    std::cout  << p->value << " ";
  } else {
    printLevel(p->left, level-1);
    printLevel(p->right, level-1);
  }
}
 
void printLevelOrderDST(Node *root) {
  int height = maxHeight(root);
  for (int level = 1; level <= height; level++) {
    printLevel(root, level);
    std::cout << std::endl;
  }
}

} // namespace bst