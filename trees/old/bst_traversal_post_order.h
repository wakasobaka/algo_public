#pragma once

#include "tree_node.h"
#include "bst_create.h"
#include "bst_print_pretty.h"

#include <iostream>

// http://www.leetcode.com/2010/10/binary-tree-post-order-traversal.html

//Pre-Order: Traverse current node, then left node, then right node.
void traverse_post_order(TreeNode* node)
{
    if(!node)
        return;
    
    traverse_post_order(node->m_left);
    traverse_post_order(node->m_right);
    std::cout << node->m_value << ", ";
}

void test_traversal_post_order()
{
    std::cout << "IN-ORDER TRAVERSAL" << std::endl;

    int input[] = {7, 9, 13, 21, 22, 28, 35, 42};
    int size = sizeof(input)/sizeof(input[0]);

    TreeNode* head = createMinimalBST(input, size);

    printPretty(head, 1, 0, std::cout);
    traverse_post_order(head);
}