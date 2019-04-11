#pragma once

#include "tree_node.h"
#include "bst_create.h"
#include "binary_tree_print_pretty.h"

namespace binary_tree
{

namespace traversal_bfs
{

namespace recursive
{

int calc_height(const Node* node)
{
    if(!node)
        return 0;

    int l_height = calc_height(node->left);
    int r_height = calc_height(node->right);

    if(l_height > r_height)
        return l_height + 1;
    else
        return r_height + 1;
}

void print_level_order(const Node* node)
{
    int height = calc_height(node);

    for(int i = 0; i < height; ++i)
    {
        
    }
}

} // namespace recursive

namespace iterative
{

} // namespace iterative

} // namespace traversal_bfs
} // namespace binary_tree
