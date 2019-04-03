add_executable( algo_trees trees.cpp
                tree_node.h 
                bst_print_level_order.h
                bst_print_pretty.h
                bst_create.h
                tree_traversal.h )

set_property(TARGET algo_trees PROPERTY FOLDER "trees")
