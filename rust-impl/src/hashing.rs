pub fn hash_leaf(value: &str) -> String {
    value.to_string()
}

pub fn concat_and_hash(lhs: &str, rhs: &str) -> String {
    lhs.to_string() + &"-".to_string() + &rhs.to_string()
}
