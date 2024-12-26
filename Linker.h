#pragma once

#include <memory>

class Horse;

class Linker {
public:
    explicit Linker(std::shared_ptr<Horse> horse); // Update to accept shared_ptr
    ~Linker() = default;

    // Modifies the counter
    void increaseCounter();
    void decreaseCounter();

    // Accessors
    int getCount() const;
    std::shared_ptr<Horse> getHorse() const; // Update to return shared_ptr

    // Utility
    bool isFollowing() const;

    // Reset horse reference
    void resetHorse();

private:
    std::weak_ptr<Horse> horseToFollow; // Update to weak_ptr
    int counter;
};

