use crate::hashing;

#[derive(Debug, Clone)]
pub struct Node {
    _right: Option<Box<Node>>,
    _left: Option<Box<Node>>,
    _value: String,
}

impl Node {
    pub fn new_leaf(value: &str) -> Node {
        Node {
            _value: hashing::hash_leaf(value),
            _right: None,
            _left: None,
        }
    }

    pub fn new(right: Node, left: Node) -> Node {
        Node {
            _value: hashing::concat_and_hash(right.get_value(), left.get_value()),
            _right: Some(Box::new(right)),
            _left: Some(Box::new(left)),
        }
    }

    pub fn get_value(&self) -> &str {
        &self._value
    }

    pub fn is_leaf(&self) -> bool {
        self._left.is_none() && self._right.is_none()
    }

    pub fn get_left(&self) -> Option<&Box<Node>> {
        self._left.as_ref()
    }

    pub fn get_right(&self) -> Option<&Box<Node>> {
        self._right.as_ref()
    }
}
