#pragma once

#include "tree_node.h"

#include <stack>

// http://www.leetcode.com/2010/04/maximum-height-of-binary-tree.html

int maxHeight(TreeNode *node)
{
    if (!node) 
        return 0;
    int left_height  = maxHeight(node->m_left);
    int right_height = maxHeight(node->m_right);
    return (left_height > right_height) ? left_height + 1 : right_height + 1;
}

// using DFS post-order traversal
int maxDepthIterative(TreeNode *root)
{
    if (!root)
        return 0;
    std::stack<TreeNode*> s;
    s.push(root);
    int maxDepth = 0;
    TreeNode *prev = NULL;
    while (!s.empty())
    {
        TreeNode *curr = s.top();
        if(!prev || (prev->m_left == curr) || (prev->m_right == curr))
        {
            if (curr->m_left)
                s.push(curr->m_left);
            else if (curr->m_right)
                s.push(curr->m_right);
        } 
        else if (curr->m_left == prev)
        {
            if (curr->m_right)
                s.push(curr->m_right);
        } 
        else
        {
            s.pop();
        }
        prev = curr;
        if (s.size() > maxDepth)
            maxDepth = s.size();
    }
    return maxDepth;
}