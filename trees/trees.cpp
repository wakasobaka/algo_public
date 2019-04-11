#include "tree_node.h"
#include "bst_create.h"
#include "binary_tree_print_pretty.h"
#include "binary_tree_traversal_dfs.h"
#include "binary_tree_traversal_bfs.h"

#include <cstdlib>

//test_pretty_print();
//test_print_level_order();


// https://leetcode.com/explore/learn/card/data-structure-tree/

int main()
{
    Node* node = bst::test_create_from_sorted_array();

    std::cout << "recursive inorder: ";
    binary_tree::traversal_dfs::recursive::print_inorder(node);
    std::cout << std::endl;

    std::cout << "recursive preorder: ";
    binary_tree::traversal_dfs::recursive::print_preorder(node);
    std::cout << std::endl;

    std::cout << "recursive postorder: ";
    binary_tree::traversal_dfs::recursive::print_postorder(node);
    std::cout << std::endl;

    std::cout << "iterative inorder: ";
    binary_tree::traversal_dfs::iterative::print_inorder(node);
    std::cout << std::endl;

    std::cout << "iterative preorder: ";
    binary_tree::traversal_dfs::iterative::print_preorder(node);
    std::cout << std::endl;

    std::cout << "iterative postorder: ";
    //binary_tree::traversal_dfs::iterative::print_postorder(node);
    std::cout << std::endl;

    std::cout << "height: " << binary_tree::traversal_bfs::recursive::calc_height(node) << std::endl;

    return EXIT_SUCCESS;
}
