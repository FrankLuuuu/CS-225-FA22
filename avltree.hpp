/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
using namespace std;

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    Node* node = t->right;
    t->right = node->left;
    node->left = t;
    t = node;
    t->height = getHeight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
    t->height = getHeight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    Node* node = t->left;
    t->left = node->right;
    node->right = t;
    t = node;
    t->height = getHeight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
    t->height = getHeight(t);
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    //your code here
    if (!subtree)
        return;

    int balance = heightOrNeg1(subtree->right) - heightOrNeg1(subtree->left); 

    if (balance == 2) {
        int balanceRight = heightOrNeg1(subtree->right->right) - heightOrNeg1(subtree->right->left);
        
        if (balanceRight > 0) // stick 
            rotateLeft(subtree);
        else // elbow 
            rotateRightLeft(subtree);
    }

    else if (balance == -2) {
        int balanceLeft = heightOrNeg1(subtree->left->right) - heightOrNeg1(subtree->left->left);

        if (balanceLeft < 0) // stick 
            rotateRight(subtree);
        else // elbow 
            rotateLeftRight(subtree);
    }
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    if(subtree == NULL) {
        subtree = new Node(key, value);
        rebalance(subtree);
    }
    else if(key < subtree->key) {
        insert(subtree->left,key,value);
        rebalance(subtree);
    }
    else {
        insert(subtree->right,key,value);
        rebalance(subtree);
    }

    rebalance(subtree);
    subtree->height = getHeight(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        // your code here
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        // your code here
        remove(subtree->right, key);
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            // your code here
            delete subtree;
            subtree = NULL;
            return;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            // your code here
            Node* node = subtree->left;
            while (node->right!=NULL)
              node = node->right;

            swap(subtree, node);
            remove(subtree->left, node->key);
        } else {
            /* one-child remove */
            // your code here
            Node* node = NULL;
            if (subtree->right == NULL) 
                node = subtree->left; 
            else if (subtree->left == NULL)
                node =  subtree->right;

            *subtree = *node;
            delete node;
            node = NULL;
        }
    }

    // your code here
    subtree->height = getHeight(subtree);  
    rebalance(subtree);
}

template <class K, class V>
int AVLTree<K, V>::getHeight(Node*& t) {
    if(!t) 
        return -1;
    
    t->height = 1 + max(getHeight(t->left), getHeight(t->right));
    return t->height;
}