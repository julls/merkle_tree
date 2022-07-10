#[cfg(test)]
mod tests {
    use rust_impl::hashing;
    use rust_impl::MerkleTree;

    fn create_tree() -> MerkleTree {
        let transactions = vec![
            "p1".to_string(),
            "p2".to_string(),
            "p3".to_string(),
            "p4".to_string(),
            "p5".to_string(),
            "p6".to_string(),
            "p7".to_string(),
            "p8".to_string(),
        ];
        MerkleTree::new(transactions)
    }

    #[test]
    fn tree_creation_test() {
        let tree = create_tree();
        assert_eq!(
            tree.get_root_hash(),
            hashing::concat_and_hash(
                &hashing::concat_and_hash(
                    &hashing::concat_and_hash("p1", "p2"),
                    &hashing::concat_and_hash("p3", "p4")
                ),
                &hashing::concat_and_hash(
                    &hashing::concat_and_hash("p5", "p6"),
                    &hashing::concat_and_hash("p7", "p8")
                )
            )
        );
    }

    #[test]
    fn tree_proof() {
        let tree = create_tree();
        let p3_proof = tree.generate_proof("p3");
        assert_eq!(true, tree.verify_proof("p3", &p3_proof));
    }
}
