add_executable( algo_trees trees.cpp
                tree_node.h 
                binary_tree_print_level_order.h
                binary_tree_print_pretty.h
                bst_create.h
                binary_tree_traversal_dfs.h
                binary_tree_traversal_bfs.h )

set_property(TARGET algo_trees PROPERTY FOLDER "algo")
