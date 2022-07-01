#ifndef MERKLE_NODE_HPP
#define MERKLE_NODE_HPP

#include <memory>
#include <iostream>
#include "Hashing.hpp"

class MerkleNode
{
public:
    using IType = const MerkleNode;
    using ConstPtr = std::shared_ptr<IType>;

    //! \brief constructor to add a leaf
    explicit MerkleNode(const std::string &val) : _value{hashing::hashFunction(val)} {}

    //! \brief constructor of an intermediate node
    MerkleNode(ConstPtr l, ConstPtr r) : _right{r}, _left{l}, _value{hashing::hashFunction(_left->getValue(), _right->getValue())} {}

    bool isLeaf() const
    {
        return _right == nullptr && _left == nullptr;
    }

    friend std::ostream &operator<<(std::ostream &o, ConstPtr node)
    {
        if (node == nullptr)
        {
            return o;
        }

        std::cout << node->_left;
        std::cout << node->_right;
        o << node->_value << std::endl;

        return o;
    }

    std::string getValue() const { return _value; }
    ConstPtr getRight() const { return _right; }
    ConstPtr getLeft() const { return _left; }

private:
    ConstPtr _right{nullptr};
    ConstPtr _left{nullptr};
    const std::string _value = "";
};

#endif