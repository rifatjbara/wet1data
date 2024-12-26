#pragma once

#include "Herd.h"
#include "Linker.h"
#include <memory>

// Forward declarations
class Herd;
class Linker;

class Horse : public std::enable_shared_from_this<Horse> {
public:
    Horse(int horseId, int speed);
    ~Horse();

    int getID() const;
    int getSpeed() const;
    std::shared_ptr<Herd> getHerd() const ;
    std::shared_ptr<Linker> getFollowers() const;
    bool isInHerd() const;
    void setInHerd(bool value);
    void joinHerd(const std::shared_ptr<Herd>& herd);
    bool isSameHerd(Horse& anotherHorse) const;
    void leadersLinkToFollow(std::weak_ptr<Linker> leadersLink);
    void unfollowLeader();
    std::shared_ptr<Linker> makeLink();
    bool checkIfVisited() const;
    void markVisited();
    void resetVisited();
    void leaveHerdProperly();
    std::shared_ptr<Horse> getLeader() const;
    std::weak_ptr<Linker> getLeaderLink() const;

private:
    int horseId;
    int speed;
    std::shared_ptr<Herd> herd; // Pointer to the herd
    std::weak_ptr<Linker> leader;    // Leader is weak_ptr to avoid circular dependencies
    std::shared_ptr<Linker> followers; // Followers are shared_ptr for shared ownership
    bool visited;                    // Used for traversal
    bool inHerd;
};
