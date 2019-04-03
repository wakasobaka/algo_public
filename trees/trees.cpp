#include "tree_node.h"
#include "bst_create.h"
#include "bst_print_pretty.h"
#include "tree_traversal.h"

#include <cstdlib>

//test_pretty_print();
//test_print_level_order();


// https://leetcode.com/explore/learn/card/data-structure-tree/

int main()
{
    Node* node = bst::test_create_from_sorted_array();

    std::cout << "recursive inorder: ";
    bst::traversal::recursive::print_inorder(node);
    std::cout << std::endl;

    std::cout << "recursive preorder: ";
    bst::traversal::recursive::print_preorder(node);
    std::cout << std::endl;

    std::cout << "recursive postorder: ";
    bst::traversal::recursive::print_postorder(node);
    std::cout << std::endl;

    std::cout << "iterative inorder: ";
    bst::traversal::recursive::print_inorder(node);
    std::cout << std::endl;


    return EXIT_SUCCESS;
}
