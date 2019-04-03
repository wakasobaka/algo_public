#pragma once

#include "tree_node.h"
#include "bst_create.h"
#include "bst_print_pretty.h"

#include <iostream>

// http://www.leetcode.com/2010/04/binary-search-tree-in-order-traversal.html

//In-Order: Traverse left node, current node, then right [usually used for binary search trees]
void traverse_in_order(TreeNode* node)
{
    if(!node)
        return;
    
    traverse_in_order(node->m_left);
    std::cout << node->m_value << ", ";
    traverse_in_order(node->m_right);
}

void test_traversal_in_order()
{
    std::cout << "IN-ORDER TRAVERSAL" << std::endl;

    int input[] = {7, 9, 13, 21, 22, 28, 35, 42};
    int size = sizeof(input)/sizeof(input[0]);

    TreeNode* head = createMinimalBST(input, size);

    printPretty(head, 1, 0, std::cout);
    traverse_in_order(head);
}