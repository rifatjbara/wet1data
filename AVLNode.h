//
// Created by rifat on 12/25/2024.
//
#pragma once

#ifndef DATAHW1_AVLNODE_H
#define DATAHW1_AVLNODE_H



template <typename Key, typename Value>
class AVLNode {
private:
    Key key;
    Value* value;
    AVLNode *left;
    AVLNode *right;
    AVLNode* parent;
    int height;

public:
    AVLNode(const Key& key, Value* value);

    ~AVLNode();

    Key getKey() const;

    Value *getValue() const;

    AVLNode* getLeft() const;

    AVLNode* getRight() const;

    AVLNode* getParent() const;

    int getHeight() const;

    int getBalanceFactor() const;

    void setValue(Value* newValue);

    void setLeft(AVLNode* leftNode);

    void setRight(AVLNode* rightNode);

    void setParent(AVLNode* parentNode);

    void updateHeight();
};


template <typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, Value* value)
        : key(key), value(value), left(nullptr), right(nullptr), parent(nullptr), height(1) {}


template <typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode() {
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}

template <typename Key, typename Value>
Key AVLNode<Key, Value>::getKey() const {
    return key;
}

template <typename Key, typename Value>
Value* AVLNode<Key, Value>::getValue() const {
    return value;
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const {
    return left;
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const {
    return right;
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const {
    return parent;
}

template <typename Key, typename Value>
int AVLNode<Key, Value>::getHeight() const {
    return height;
}

template <typename Key, typename Value>
int AVLNode<Key, Value>::getBalanceFactor() const {
    int leftHeight = (left != nullptr) ? left->getHeight() : 0;
    int rightHeight = (right != nullptr) ? right->getHeight() : 0;
    return leftHeight - rightHeight;
}

template <typename Key, typename Value>
void AVLNode<Key, Value>::setValue(Value* newValue) {
    //if (value) {
      //  delete value; // Clean up old value
    //}
    value = newValue;
    // לבדוק מי אחראי על שחרור זכרון value
}

template <typename Key, typename Value>
void AVLNode<Key, Value>::setLeft(AVLNode* leftNode) {
    left = leftNode;
}

template <typename Key, typename Value>
void AVLNode<Key, Value>::setRight(AVLNode* rightNode) {
    right = rightNode;
}

template <typename Key, typename Value>
void AVLNode<Key, Value>::setParent(AVLNode* parentNode) {
    parent = parentNode;
}

template <typename Key, typename Value>
void AVLNode<Key, Value>::updateHeight() {
    int leftHeight = (left != nullptr) ? left->getHeight() : 0;
    int rightHeight = (right != nullptr) ? right->getHeight() : 0;
    height = (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
}



#endif //DATAHW1_AVLNODE_H
