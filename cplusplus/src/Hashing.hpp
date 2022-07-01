#ifndef MERKLE_TREE_HASHING_HPP
#define MERKLE_TREE_HASHING_HPP

#include <string>

namespace hashing
{
    static std::string hashFunction(const std::string &val)
    {
        return val;
    }

    static std::string hashFunction(const std::string &lhs, const std::string &rhs)
    {
        return lhs + "-" + rhs;
    }
}

#endif // MERKLE_TREE_HASHING_HPP