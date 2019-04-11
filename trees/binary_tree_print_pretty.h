#pragma once 

#include "tree_node.h"

// http://www.leetcode.com/2010/09/how-to-pretty-print-binary-tree.html

#include <fstream>
#include <iostream>
#include <deque>
#include <iomanip>
#include <sstream>
#include <string>

namespace binary_tree
{

// Find the maximum height of the binary tree
int maxHeight(Node *p) {
  if (!p) return 0;
  int leftHeight = maxHeight(p->left);
  int rightHeight = maxHeight(p->right);
  return (leftHeight > rightHeight) ? leftHeight + 1: rightHeight + 1;
}

// Convert an integer value to string
std::string intToString(int val) {
  std::ostringstream ss;
  ss << val;
  return ss.str();
}

// Print the arm branches (eg, /    \ ) on a line
void printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const std::deque<Node*>& nodesQueue, std::ostream& out) {
  std::deque<Node*>::const_iterator iter = nodesQueue.begin();
  for (int i = 0; i < nodesInThisLevel / 2; i++) {  
    out << ((i == 0) ? std::setw(startLen-1) : std::setw(nodeSpaceLen-2)) << "" << ((*iter++) ? "/" : " ");
    out << std::setw(2*branchLen+2) << "" << ((*iter++) ? "\\" : " ");
  }
  out << std::endl;
}

// Print the branches and node (eg, ___10___ )
void printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const std::deque<Node*>& nodesQueue, std::ostream& out) {
  std::deque<Node*>::const_iterator iter = nodesQueue.begin();
  for (int i = 0; i < nodesInThisLevel; i++, iter++) {
    out << ((i == 0) ? std::setw(startLen) : std::setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->left) ? std::setfill('_') : std::setfill(' '));
    out << std::setw(branchLen+2) << ((*iter) ? intToString((*iter)->value) : "");
    out << ((*iter && (*iter)->right) ? std::setfill('_') : std::setfill(' ')) << std::setw(branchLen) << "" << std::setfill(' ');
  }
  out << std::endl;
}

// Print the leaves only (just for the bottom row)
void printLeaves(int indentSpace, int level, int nodesInThisLevel, const std::deque<Node*>& nodesQueue, std::ostream& out) {
  std::deque<Node*>::const_iterator iter = nodesQueue.begin();
  for (int i = 0; i < nodesInThisLevel; i++, iter++) {
    out << ((i == 0) ? std::setw(indentSpace+2) : std::setw(2*level+2)) << ((*iter) ? intToString((*iter)->value) : "");
  }
  out << std::endl;
}

// Pretty formatting of a binary tree to the output stream
// @ param
// level  Control how wide you want the tree to sparse (eg, level 1 has the minimum space between nodes, while level 2 has a larger space between nodes)
// indentSpace  Change this to add some indent space to the left (eg, indentSpace of 0 means the lowest level of the left node will stick to the left margin)
void printPretty(Node *root, int level = 2, int indentSpace = 1, std::ostream& out = std::cout) {
  int h = maxHeight(root);
  int nodesInThisLevel = 1;

  int branchLen = 2*((int)pow(2.0,h)-1) - (3-level)*(int)pow(2.0,h-1);  // eq of the length of branch for each node of each level
  int nodeSpaceLen = 2 + (level+1)*(int)pow(2.0,h);  // distance between left neighbor node's right arm and right neighbor node's left arm
  int startLen = branchLen + (3-level) + indentSpace;  // starting space to the first node to print of each level (for the left most node of each level only)
    
  std::deque<Node*> nodesQueue;
  nodesQueue.push_back(root);
  for (int r = 1; r < h; r++) {
    printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
    branchLen = branchLen/2 - 1;
    nodeSpaceLen = nodeSpaceLen/2 + 1;
    startLen = branchLen + (3-level) + indentSpace;
    printNodes(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);

    for (int i = 0; i < nodesInThisLevel; i++) {
      Node *currNode = nodesQueue.front();
      nodesQueue.pop_front();
      if (currNode) {
          nodesQueue.push_back(currNode->left);
          nodesQueue.push_back(currNode->right);
      } else {
        nodesQueue.push_back(nullptr);
        nodesQueue.push_back(nullptr);
      }
    }
    nodesInThisLevel *= 2;
  }
  printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
  printLeaves(indentSpace, level, nodesInThisLevel, nodesQueue, out);
}

int test_pretty_print()
{
  Node *root = new Node(30);
  root->left = new Node(20);
  root->right = new Node(40);
  /*
  root->left->left = new Node(10);
  root->left->right = new Node(25);
  root->right->left = new Node(35);
  root->right->right = new Node(50);
  root->left->left->left = new Node(5);
  root->left->left->right = new Node(15);
  root->left->right->right = new Node(28);
  root->right->right->left = new Node(41);
  */

  std::cout << "Tree pretty print with level=1 and indentSpace=0\n\n";
  // Output to console
  printPretty(root, 2, 1, std::cout);

  //cout << "\n\nTree pretty print with level=5 and indentSpace=3,\noutput to file \"tree_pretty.txt\".\n\n";
  // Create a file and output to that file
  //std::ofstream fout("tree_pretty.txt");
  // Now print a tree that's more spread out to the file
  //printPretty(root, 5, 0, std::cout);

  return 0;
}

} // namespace binary_tree