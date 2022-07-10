#[cfg(test)]
mod tests {
    use rust_impl::hashing;
    use rust_impl::node::Node;
    #[test]
    fn node_creation_test() {
        let tx1 = "p1";
        let tx2 = "p2";
        let leaf1 = Node::new_leaf(tx1);
        let leaf2 = Node::new_leaf(tx2);
        println!("leaf1 = {:?}", leaf1);
        println!("leaf2 = {:?}", leaf2);
        assert_eq!(leaf1.get_value(), hashing::hash_leaf(tx1));
        assert_eq!(leaf2.get_value(), hashing::hash_leaf(tx2));
        let n1 = Node::new(leaf1, leaf2);
        println!("node1 = {:?}", n1);
        assert_eq!(n1.get_value(), hashing::concat_and_hash(tx1, tx2));
    }
}
