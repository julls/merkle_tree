pub mod hashing;
pub mod node;

#[derive(Debug)]
pub struct MerkleTree {
    _root: Option<Box<node::Node>>,
    _transactions: Vec<String>,
}

impl MerkleTree {
    pub fn new(data: Vec<String>) -> MerkleTree {
        MerkleTree {
            _root: MerkleTree::build(&data),
            _transactions: data,
        }
    }

    fn build(data: &Vec<String>) -> Option<Box<node::Node>> {
        let n_leafs = data.len();

        if (n_leafs != 0) && (n_leafs & (n_leafs - 1) != 0) {
            println!(
                "Error: the number of transactions ({}) has to be a power of 2!",
                n_leafs
            );
            return None;
        }

        let height = n_leafs.trailing_zeros() + 1;
        let n_nodes = (n_leafs * 2) - 1;

        // vector of nodes - would it be better to store this vector instead of the root?
        let mut nodes: Vec<node::Node> = vec![];
        nodes.reserve_exact(n_nodes);

        // hash the transactions and create the first (bottom) layer of the tree
        nodes.extend(data.iter().map(|val| node::Node::new_leaf(&val)));

        // then go up the tree layer by layer, calculating the width of the layer (number of nodes)
        // by just dividing it by 2, since we use 2 nodes from the lower layer to create 1 node in the next
        let mut layer = 1;
        let mut width = n_leafs;
        // l0 := last layer index
        while layer < height {
            let tree_size = nodes.len();
            let range = (tree_size - width)..tree_size;
            for i in range.step_by(2) {
                nodes.push(node::Node::new(nodes[i].clone(), nodes[i + 1].clone()));
            }

            // the next layer is going to have half the number of nodes (half tree-width) as the current layer
            width /= 2;
            layer += 1;
        }

        // return the root
        Some(Box::new(nodes[n_nodes - 1].clone()))
    }

    pub fn get_root_hash(&self) -> &str {
        match &self._root {
            Some(n) => n.get_value(),
            None => "",
        }
    }

    pub fn generate_proof(&self, query: &str) -> Vec<String> {
        if self._transactions.iter().find(|&s| s == query) == None || self._root.is_none() {
            println!("Error: the query transaction is not part of this merkle tree!");
            return Vec::new();
        }

        if self._root.is_none() {
            println!("Error: the tree root is empty!");
            return Vec::new();
        }

        let mut proof: Vec<String> = vec![];
        self.get_proof(self._root.as_ref(), &hashing::hash_leaf(query), &mut proof);
        proof
    }

    pub fn verify_proof(&self, query: &str, proof: &Vec<String>) -> bool {
        if self._transactions.iter().find(|&s| s == query) == None || self._root.is_none() {
            println!("Error: the query transaction is not part of this merkle tree!");
            return false;
        }

        if self._root.is_none() {
            println!("Error: the tree root is empty!");
            return false;
        }

        if proof.is_empty() {
            println!("Error: the proof array is empty!");
            return false;
        }

        let mut index = self._transactions.iter().position(|s| s == query).unwrap();
        let mut hash = hashing::hash_leaf(query);

        for node_hash in proof {
            // if the current index is even, we want to concat the current hash with the given proof hash
            if index % 2 == 0 {
                hash = hashing::concat_and_hash(&hash, node_hash);
            }
            // if the current index is odd, we want to concat the given proof hash with the current hash
            else {
                hash = hashing::concat_and_hash(node_hash, &hash);
            }

            // when we move up the tree, the index just gets divided by 2
            index /= 2;
        }

        // if the final computed hash is equal to this tree root hash, then this proof is valid
        hash == self.get_root_hash()
    }

    fn get_proof(
        &self,
        root: Option<&Box<node::Node>>,
        hashed_query: &str,
        proof: &mut Vec<String>,
    ) -> bool {
        match root {
            None => return false,
            Some(nodo) => {
                if nodo.is_leaf() {
                    if nodo.get_value() == hashed_query {
                        return true;
                    } else {
                        return false;
                    }
                }

                let left = self.get_proof(nodo.get_left(), hashed_query, proof);
                let right = self.get_proof(nodo.get_right(), hashed_query, proof);

                if left {
                    proof.push(nodo.get_right().unwrap().get_value().to_string());
                    return true;
                }
                if right {
                    proof.push(nodo.get_left().unwrap().get_value().to_string());
                    return true;
                }

                return false;
            }
        }
    }
}
