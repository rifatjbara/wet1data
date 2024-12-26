//
// Created by rifat on 12/25/2024.
//
#pragma once

#include "AVLTree.h"
#include "Horse.h"
#include <memory>


class Horse;

class Herd {
public:
    explicit Herd(int herdId);
    ~Herd();

    int getNumOfFollowers() const;
    int getNumOfConnections() const;
    AVLTree<int, std::shared_ptr<Horse>>& getHorsesTree(); // Update to shared_ptr
    int getSize() const;
    int getId() const;
    void addHorse(std::shared_ptr<Horse> horse); // Update to shared_ptr
    void increaseSize();
    void increaseConnections();
    void decreaseSize();
    void decreaseConnections(int numOfLostConnections);
    void removeHorse(std::shared_ptr<Horse> horse); // Update to shared_ptr

private:
    int herdId;
    int size;
    int numOfConnections;
    int numOfFollowers;
    AVLTree<int, std::shared_ptr<Horse>> horses; // Store shared_ptr<Horse> in the tree
};

