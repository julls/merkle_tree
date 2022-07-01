#ifndef MERKLE_TREE_HPP
#define MERKLE_TREE_HPP

#include "MerkleNode.hpp"
#include "Hashing.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

//! @brief Simplified implementation of a merkle tree.
//!        The tree is a balanced, binary tree of "hashes".
class MerkleTree
{
public:
    //! @brief construct a new Merkle Tree.
    //! @param transactions  array of transactions (not hashed)
    MerkleTree(const std::vector<std::string> &transactions) : _transactions{transactions}
    {
        _root = build();
    }

    //! @brief generates the proof needed to verify that the query transaction was included in this tree.
    //!
    //! @param query  query transaction
    //! @return std::vector<std::string>  array of proof hashes
    std::vector<std::string> generateProof(const std::string &query)
    {
        if (std::find(_transactions.begin(), _transactions.end(), query) == _transactions.end())
        {
            std::cerr << "Error: Transaction not part of this Merkle Tree!" << std::endl;
            return {};
        }

        std::vector<std::string> proof;
        proof.reserve(_height - 1);
        static_cast<void>(getProof(_root, hashing::hashFunction(query), proof));
        return proof;
    }

    //! @brief verify a proof path based on the transaction it was generated from.
    //!
    //! @param query   query transaction
    //! @param proof   proof array of hashes
    //! @return true   if the final hash matches this tree root hash
    //! @return false  otherwise
    bool verifyProof(const std::string &query, const std::vector<std::string> &proof)
    {
        auto foundTX = std::find(_transactions.begin(), _transactions.end(), query);
        if (foundTX == _transactions.end())
        {
            std::cerr << "Error: Transaction not part of this Merkle Tree!" << std::endl;
            return false;
        }

        if (proof.empty())
        {
            std::cerr << "Error: The proof array is empty!" << std::endl;
            return false;
        }

        // get the index of the transaction on the bottom layer
        size_t index = std::distance(_transactions.begin(), foundTX);
        std::string hash = hashing::hashFunction(query);

        for (const auto &nodeHash : proof)
        {
            // if the current index is even, we want to concat the current hash with the given proof hash
            if (index % 2 == 0)
            {
                hash = hashing::hashFunction(hash, nodeHash);
            }
            // if the current index is odd, we want to concat the given proof hash with the current hash
            else
            {
                hash = hashing::hashFunction(nodeHash, hash);
            }

            // when we move up the tree, the index just gets divided by 2
            index /= 2;
        }

        // if the final computed hash is equal to this tree root hash, then this proof is valid
        return hash == getRootHash();
    }

    std::string getRootHash()
    {
        return _root->getValue();
    }

    friend std::ostream &operator<<(std::ostream &o, MerkleTree const &tree)
    {
        o << tree._root;
        return o;
    }

private:
    //! @brief populates the tree based on the _transactions
    //! @return MerkleNode::ConstPtr root
    MerkleNode::ConstPtr build()
    {
        const int nLeafs{static_cast<int>(_transactions.size())};

        if (utils::isPowerOfTwo(nLeafs))
        {
            std::cerr << "Error: the number of transactions (" << nLeafs << ") has to be a power of 2!" << std::endl;
            return nullptr;
        }

        const int height{utils::trailingZeros(nLeafs) + 1};
        const int nNodes{(nLeafs * 2) - 1};

        // hash the transactions and create the first (bottom) layer of the tree
        // do I need to hash the transactions or can I just use them as they are?
        std::vector<MerkleNode::ConstPtr> nodes;
        nodes.reserve(nNodes);
        std::for_each(_transactions.begin(), _transactions.end(), [&](const std::string &t)
                      { nodes.push_back(std::make_shared<MerkleNode::IType>(MerkleNode(t))); });

        // then go up the tree layer by layer, calculating the width of the layer (number of nodes)
        // by just dividing it by 2, since we use 2 nodes from the lower layer to create 1 node in the next
        int layer = 1;
        int width = nLeafs;
        // l0 := last layer index
        while (layer < height)
        {
            const int treeSize{static_cast<int>(nodes.size())};
            for (int l0 = treeSize - width; l0 < treeSize; l0 += 2)
            {
                auto left = nodes[l0];
                auto right = nodes[l0 + 1];
                nodes.push_back(std::make_shared<MerkleNode::IType>(MerkleNode(left, right)));
            }

            // the next layer is going to have half the number of nodes (half tree-width) as the current layer
            width /= 2;
            layer++;
        }

        _height = height;
        return nodes[nNodes - 1]; // return the root
    }

    //! @brief generates the proof array by recursively going down the tree
    //!
    //! @param root         root of the tree (or current node when traversing the tree)
    //! @param hashedQuery  hashed of the transaction for which to generate the proof
    //! @param proof        generated array of hashes
    //! @return true        if the next proof hash was found or if the whole proof array was found
    //! @return false       otherwise
    bool getProof(MerkleNode::ConstPtr root, const std::string &hashedQuery, std::vector<std::string> &proof)
    {
        if (root == nullptr)
        {
            return false;
        }

        if (root->isLeaf())
        {
            if (root->getValue() == hashedQuery)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool left = getProof(root->getLeft(), hashedQuery, proof);
        bool right = getProof(root->getRight(), hashedQuery, proof);

        if (left)
        {
            proof.push_back(root->getRight()->getValue());
            return true;
        }

        if (right)
        {
            proof.push_back(root->getLeft()->getValue());
            return true;
        }

        return false;
    }

    int _height{0};
    MerkleNode::ConstPtr _root{nullptr};
    const std::vector<std::string> _transactions;
};

#endif