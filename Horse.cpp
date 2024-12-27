#include "Horse.h"
#include <memory>

Horse::Horse(int horseId, int speed)
        : horseId(horseId), speed(speed), herd(nullptr),
          leader(), followers(nullptr), visited(UNVISITED), inHerd(false) {}

Horse::~Horse() {
    if (followers) {
        followers->resetHorse();
        followers.reset(); // Reset the shared_ptr
    }
}

int Horse::getID() const {
    return horseId;
}

int Horse::getSpeed() const {
    return speed;
}

std::shared_ptr<Herd> Horse::getHerd() const {
    return herd;
}


std::shared_ptr<Linker> Horse::getFollowers() const {
    return followers;
}

bool Horse::isInHerd() const {
    return inHerd;
}

void Horse::setInHerd(bool value) {
    inHerd = value;
}

void Horse::joinHerd(const std::shared_ptr<Herd>& herd) {
    if (this->herd == herd) return;
    this->herd = herd;
    this->inHerd = (herd != nullptr);
}


bool Horse::isSameHerd(Horse& anotherHorse) const {
    return (herd == anotherHorse.getHerd() && herd != nullptr);
}

void Horse::leadersLinkToFollow(std::weak_ptr<Linker> leadersLink) {
    if (auto currentLeader = leader.lock()) { // Check if leader exists
        this->unfollowLeader();
    }
    this->leader = leadersLink;
    this->leader.lock()->increaseCounter();
    herd->increaseConnections();
}

void Horse::unfollowLeader() {
    if (auto currentLeader = leader.lock()) { // Lock the weak_ptr to shared_ptr
        if(currentLeader->getHorse() != nullptr) {
            herd->decreaseConnections(1);
        }
        currentLeader->decreaseCounter();
        if (currentLeader->getCount() == 0) {
            if (auto currentHorse = currentLeader->getHorse()) {
                currentHorse->followers.reset(); // Reset followers shared_ptr
            }
        }
    }
    leader.reset(); // Reset the weak_ptr
}

std::shared_ptr<Linker> Horse::makeLink() {
    if (!followers) {
        followers = std::make_shared<Linker>(shared_from_this()); // Pass shared_ptr to this horse
    }
    return followers;
}


bool Horse::checkIfVisited(int num) const {
    return visited == num;
}

void Horse::markVisited(int num) {
    visited = num;
}

void Horse::resetVisited() {
    visited = UNVISITED;
}

void Horse::leaveHerdProperly() {
    if (followers) {
        herd->decreaseConnections(followers->getCount());
        followers->resetHorse();
        followers.reset(); // Prevent dangling pointers
    }

    this->unfollowLeader();

    herd = nullptr;
    inHerd = false;
}

std::shared_ptr<Horse> Horse::getLeader() const {
    if (auto currentLeader = leader.lock()) { // Convert weak_ptr to shared_ptr
        return currentLeader->getHorse(); // Return shared_ptr<Horse>
    }
    return nullptr; // Return nullptr if leader is expired
}


std::weak_ptr<Linker> Horse::getLeaderLink() const {
    return leader;
}
