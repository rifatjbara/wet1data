#include "Linker.h"

// Constructor
Linker::Linker(std::shared_ptr<Horse> horse)
        : horseToFollow(horse), counter(0) {}

// Increases the follower counter
void Linker::increaseCounter() {
    counter++;
}

// Decreases the follower counter
void Linker::decreaseCounter() {
    if (counter > 0) {
        counter--;
    }
}

// Returns the current follower count
int Linker::getCount() const {
    return counter;
}

// Returns the horse being followed
std::shared_ptr<Horse> Linker::getHorse() const {
    return horseToFollow.lock(); // Convert weak_ptr to shared_ptr
}

// Checks if there are any followers
bool Linker::isFollowing() const {
    return counter > 0;
}

// Resets the horse reference
void Linker::resetHorse() {
    horseToFollow.reset();
}
