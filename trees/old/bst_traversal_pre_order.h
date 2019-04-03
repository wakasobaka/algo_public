#pragma once

#include "tree_node.h"
#include "bst_create.h"
#include "bst_print_pretty.h"

#include <iostream>

//Pre-Order: Traverse current node, then left node, then right node.
void traverse_pre_order(TreeNode* node)
{
    if(!node)
        return;
    
    std::cout << node->m_value << ", ";
    traverse_pre_order(node->m_left);
    traverse_pre_order(node->m_right);
}

void test_traversal_in_order()
{
    std::cout << "PRE-ORDER TRAVERSAL" << std::endl;

    int input[] = {7, 9, 13, 21, 22, 28, 35, 42};
    int size = sizeof(input)/sizeof(input[0]);

    TreeNode* head = createMinimalBST(input, size);

    printPretty(head, 1, 0, std::cout);
    traverse_pre_order(head);
}