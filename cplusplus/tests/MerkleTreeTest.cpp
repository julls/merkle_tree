#include "../src/MerkleNode.hpp"
#include "../src/MerkleTree.hpp"
#include "../src/Utils.hpp"

int main()
{
    std::vector<std::string> leafs = {"p1", "p2", "p3", "p4", "p5", "p6", "p7", "p8"};
    MerkleTree tree(leafs);

    std::cout << "Tree Structure" << std::endl;
    std::cout << tree << std::endl;

    const std::string &query = "p10"; // leafs[5];
    const std::vector<std::string> proof = tree.generateProof(query);
    utils::print(proof, "proof");

    std::cout << "is the proof right? " << tree.verifyProof(query, proof) << std::endl;

    return 1;
}