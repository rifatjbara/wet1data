#include "Herd.h"

Herd::Herd(int herdId)
        : herdId(herdId), size(0), numOfConnections(0), numOfFollowers(0), horses() {}

Herd::~Herd() {
    horses.clear(); // Clear shared_ptrs, which will automatically handle memory
}

int Herd::getNumOfFollowers() const {
    return this->numOfFollowers;
}

int Herd::getNumOfConnections() const {
    return this->numOfConnections;
}

AVLTree<int, std::shared_ptr<Horse>>& Herd::getHorsesTree() {
    return this->horses;
}

int Herd::getSize() const {
    return this->size;
}

int Herd::getId() const {
    return herdId;
}

void Herd::addHorse(std::shared_ptr<Horse> horse) {
    if (horses.find(horse->getID()).status() == StatusType::SUCCESS) {
        return;
    }
    horses.insert(horse->getID(), horse);
    size = horses.getSize();
}

void Herd::increaseSize() {
    this->size++;
}

void Herd::increaseConnections() {
    this->numOfConnections++;
}

void Herd::decreaseSize() {
    this->size = (this->size > 0) ? this->size - 1 : 0;
}

void Herd::decreaseConnections(int numOfLostConnections) {
    this->numOfConnections = std::max(0, this->numOfConnections - numOfLostConnections);
}

void Herd::removeHorse(std::shared_ptr<Horse> horse) {
    if (!horse) {
        return;
    }

    int horseId = horse->getID();

    auto findResult = horses.find(horseId);
    if (findResult.status() == StatusType::FAILURE) {
        return;
    }

    horses.remove(horseId);
    size = horses.getSize();
}
