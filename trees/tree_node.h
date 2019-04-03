#pragma once

class Node
{
public:

    Node(int value)
    : value (value)
    {}

    int value = 0;

    Node* left   = nullptr;
    Node* right  = nullptr;
};
