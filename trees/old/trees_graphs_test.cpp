// trees.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "bst_create.h"
#include "bst_print_pretty.h"
#include "bst_print_level_order.h"
#include "bst_traversal_in_order.h"

// Check whether given binary tree is a BST or not
// url: http://crackinterviewtoday.wordpress.com/2010/03/12/check-whether-given-binary-tree-is-a-bst-or-not/

// Diameter of a binary tree
// url: http://crackinterviewtoday.wordpress.com/2010/03/11/diameter-of-a-binary-tree/

// Rebuild a binary tree from Inorder and Preorder traversals
// http://crackinterviewtoday.wordpress.com/2010/03/15/rebuild-a-binary-tree-from-inorder-and-preorder-traversals/


int _tmain(int argc, _TCHAR* argv[])
{
    //test_bst_create_from_sorted_array();

    test_pretty_print();

    test_print_level_order();

    test_traversal_in_order();
    
    return 0;
}

