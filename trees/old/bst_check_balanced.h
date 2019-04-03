#pragma once

#include "tree_node.h"

#include <cmath>

int maxDepth(TreeNode* root)
{
    if(!root)
        return 0;

    return 1 + std::max(maxDepth(root->m_left), maxDepth(root->m_right));
}

int minDepth(TreeNode* root)
{
    if(!root)
        return 0;

    return 1 + std::min(minDepth(root->m_left), minDepth(root->m_right));
}

bool isBalanced(TreeNode* root)
{
    return (maxDepth(root) - minDepth(root) <= 1);
}