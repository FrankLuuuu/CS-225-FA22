/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    // your code here
    Node*& node = find(root, key);
    if(node)
        return node->value;

    return root->value;
}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    // Your code here
    if(subtree == NULL || subtree->key == key)
        return subtree;

    if(subtree->key > key)
        return find(subtree->left, key);
    if(subtree->key < key)
        return find(subtree->right, key);

    return subtree;
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    // your code here
    insert(root, key, value);
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    Node*& node = find(subtree, key);
    if(node == NULL)
        node = new Node(key, value);
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    // your code here
    Node* node = first;
    first = second;
    second = node;
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    // your code here
    if (key == root->key) {
        remove(root, root, false);
    } else {
        remove(root, key);
    }
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    // your code here
    if (subtree == NULL) return;
    if (key < subtree->key) {
        if (subtree->left != NULL && subtree->left->key == key) {
            Node* to_remove = subtree->left;
            remove(to_remove, subtree, true);
        } else {
            remove(subtree->left, key);
        }
    } else if (key > subtree->key) {
        if (subtree->right != NULL && key == subtree->right->key) {
            Node* to_remove = subtree->right;
            remove(to_remove, subtree, false);
        } else {
            remove(subtree->right, key);
        }
    }
}

template <class K, class V>
void BST<K, V>::remove(Node*& to_remove, Node*& parent, bool is_left) {
    if (to_remove->left == NULL) {
        if (to_remove == root) {
            root = to_remove->right;
        } else if (is_left) {
            parent->left = to_remove->right;
        } else {
            parent->right = to_remove->right;
        }
    } else {
        Node* new_ = to_remove->left;
        Node* prev = to_remove;
        while (new_->right != NULL) {
            prev = new_;
            new_ = new_->right;
        }
        if (prev != to_remove) {
            prev->right = new_->left;
        }
        if (to_remove == root) {
            if (root->left != new_) {
                new_->left = root->left;
            }
            new_->right = root->right;
            root = new_;
        } else if (is_left) {
            parent->left = new_;
            new_->left = to_remove->left;
            new_->right = to_remove->right;
        } else {
            parent->right = new_;
            new_->left = to_remove->left;
            new_->right = to_remove->right;
        }  
    }
}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    BST<K, V> tree;
    for (std::pair<K, V> p : inList) 
        tree.insert(p.first, p.second);
    return tree;
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    std::vector<int> res = std::vector<int>(inList.size() - 1);
    std::sort(inList.begin(), inList.end());
    res.push_back(0);
    do {
        BST<K,V> tree = listBuild(inList);
        int height = tree.height();
        res[height]++;
    } while (std::next_permutation(inList.begin(), inList.end()));
    return res;
}