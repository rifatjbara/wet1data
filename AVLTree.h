//
// Created by rifat on 12/25/2024.
//
#pragma once

#ifndef DATAHW1_AVLTREE_H
#define DATAHW1_AVLTREE_H

#include "AVLNode.h"
#include "wet1util.h"


template <typename Key, typename Value>
class AVLTree {
private:
    AVLNode<Key, Value>* root;
    int size;

    void clear(AVLNode<Key, Value>* node);

    AVLNode<Key, Value>* insertAux(AVLNode<Key, Value>* node, const Key& key, Value* value, bool* isNewNode);

    AVLNode<Key, Value>* removeAux(AVLNode<Key, Value>* node, const Key& key);

    AVLNode<Key, Value>* balance(AVLNode<Key, Value>* node);

    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* node);

    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* node);

    AVLNode<Key, Value>* findMin(AVLNode<Key, Value>* node) const;

    AVLNode<Key, Value>* removeMin(AVLNode<Key, Value>* node);

    template <typename Func>
    void inOrderTraversal(Func func, AVLNode<Key, Value>* node) const;

public:
    AVLTree();

    ~AVLTree();

    void clear();

    StatusType insert(const Key& key, const Value& value);

    StatusType remove(const Key& key);

    output_t<Value> find(const Key& key) const;

    int getSize() const;

    bool isEmpty() const;

    void inOrder() const;

    template <typename Func>
    void inOrderTraversal(Func func) const;
};


template <typename Key, typename Value>
AVLTree<Key, Value>::AVLTree() : root(nullptr), size(0) {}

template <typename Key, typename Value>
AVLTree<Key, Value>::~AVLTree() {
    clear();
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::clear(AVLNode<Key, Value>* node) {
    if (!node) return;

    clear(node->getLeft());
    clear(node->getRight());

    delete node->getValue();
    delete node;
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::clear() {
    clear(root);
    root = nullptr;
    size = 0;
}

template <typename Key, typename Value>
StatusType AVLTree<Key, Value>::insert(const Key& key, const Value& value) {
    bool isNewNode = false;
    Value* newValue = new Value(value);
    root = insertAux(root, key, newValue, &isNewNode);
    if (isNewNode) {
        size++;
        return StatusType::SUCCESS;
    }
    delete newValue;
    return StatusType::FAILURE;
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::insertAux(AVLNode<Key, Value>* node, const Key& key, Value* value, bool* isNewNode) {
    if (!node) {
        *isNewNode = true;
        return new AVLNode<Key, Value>(key, value);
    }
    if (key < node->getKey()) {
        node->setLeft(insertAux(node->getLeft(), key, value, isNewNode));
    } else if (key > node->getKey()) {
        node->setRight(insertAux(node->getRight(), key, value, isNewNode));
    } else {
        delete value;
        return node;
    }
    node->updateHeight();
    return balance(node);
}

template <typename Key, typename Value>
StatusType AVLTree<Key, Value>::remove(const Key& key) {
    if (find(key).status() == StatusType::FAILURE) {
        return StatusType::FAILURE;
    }
    root = removeAux(root, key);
    size--;
    return StatusType::SUCCESS;
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::removeAux(AVLNode<Key, Value>* node, const Key& key) {
    if (!node) {
        return nullptr;
    }

    if (key < node->getKey()) {
        node->setLeft(removeAux(node->getLeft(), key));
    } else if (key > node->getKey()) {
        node->setRight(removeAux(node->getRight(), key));
    } else {
        AVLNode<Key, Value>* left = node->getLeft();
        AVLNode<Key, Value>* right = node->getRight();
        delete node;

        if (!right) return left;
        if (!left) return right;

        AVLNode<Key, Value>* minNode = findMin(right);
        minNode->setRight(removeMin(right));
        minNode->setLeft(left);
        minNode->updateHeight();
        return balance(minNode);
    }

    node->updateHeight();
    return balance(node);
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findMin(AVLNode<Key, Value>* node) const {
    while (node && node->getLeft()) {
        node = node->getLeft();
    }
    return node;
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::removeMin(AVLNode<Key, Value>* node) {
    if (!node->getLeft()) return node->getRight();
    node->setLeft(removeMin(node->getLeft()));
    node->updateHeight();
    return balance(node);
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* newRoot = node->getRight();
    node->setRight(newRoot->getLeft());
    newRoot->setLeft(node);

    node->updateHeight();
    newRoot->updateHeight();
    return newRoot;
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
    AVLNode<Key, Value>* newRoot = node->getLeft();
    node->setLeft(newRoot->getRight());
    newRoot->setRight(node);

    node->updateHeight();
    newRoot->updateHeight();
    return newRoot;
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::balance(AVLNode<Key, Value>* node) {
    int balanceFactor = node->getBalanceFactor();

    if (balanceFactor > 1) {
        if (node->getLeft() && node->getLeft()->getBalanceFactor() < 0) {
            node->setLeft(rotateLeft(node->getLeft()));
        }
        return rotateRight(node);
    }
    if (balanceFactor < -1) {
        if (node->getRight() && node->getRight()->getBalanceFactor() > 0) {
            node->setRight(rotateRight(node->getRight()));
        }
        return rotateLeft(node);
    }
    return node;
}

template <typename Key, typename Value>
output_t<Value> AVLTree<Key, Value>::find(const Key& key) const {
    AVLNode<Key, Value>* current = root;
    while (current) {
        if (key == current->getKey()) {
            return output_t<Value>(*(current->getValue()));
        }
        current = (key < current->getKey()) ? current->getLeft() : current->getRight();
    }
    return output_t<Value>(StatusType::FAILURE);
}

template <typename Key, typename Value>
template <typename Func>
void AVLTree<Key, Value>::inOrderTraversal(Func func) const {
    inOrderTraversal(func, root);
}

template <typename Key, typename Value>
template <typename Func>
void AVLTree<Key, Value>::inOrderTraversal(Func func, AVLNode<Key, Value>* node) const {
    if (!node) return;
    inOrderTraversal(func, node->getLeft());
    func(node->getKey(), *(node->getValue()));
    inOrderTraversal(func, node->getRight());
}

template <typename Key, typename Value>
int AVLTree<Key, Value>::getSize() const {
    return size;
}

template <typename Key, typename Value>
bool AVLTree<Key, Value>::isEmpty() const {
    return root == nullptr;
}




#endif //DATAHW1_AVLTREE_H
