#ifndef MERKLE_TREE_UTILS_HPP
#define MERKLE_TREE_UTILS_HPP

#include <vector>
#include <algorithm>
#include "MerkleNode.hpp"

namespace utils
{
    static bool isPowerOfTwo(const int n)
    {
        return (n != 0) && (n & (n - 1));
    }

    static int trailingZeros(const int n)
    {
        return __builtin_ctz(static_cast<unsigned int>(n));
    }

    static void print(const std::vector<MerkleNode::ConstPtr> &nodes, const std::string &option = "")
    {
        std::cout << option << std::endl;
        std::for_each(nodes.begin(), nodes.end(), [&](MerkleNode::ConstPtr n)
                      { std::cout << n->getValue() << ", "; });
        std::cout << std::endl;
    }

    static void print(const std::vector<std::string> &values, const std::string &option = "")
    {
        std::cout << option << std::endl;
        std::for_each(values.begin(), values.end(), [&](const std::string v)
                      { std::cout << v << ", "; });
        std::cout << std::endl;
    }
}

#endif // MERKLE_TREE_UTILS_HPP