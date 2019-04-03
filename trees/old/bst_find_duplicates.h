#pragma once

// http://www.careercup.com/question?id=14952824

// Write an algorithm to print out how many extra duplicates there are in a binary search tree.
//
// input 1:
//  2 
// / \
// 1 2
// output 1:
// 2 1
//
// input 2:
//       3
//      / \ 
//     2   3
//    / \   \
//   1   2   4
//          / \
//         3   4
//              \
//               5
//                \
//                 5
// output 2: 
// 2 1
// 3 2
// 4 1
// 5 1
//
// Given:
// Node 
// {
//     int value;
//     Node left;
//     Node right; 
// }
