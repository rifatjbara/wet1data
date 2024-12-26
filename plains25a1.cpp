// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"

Plains::Plains() : nonEmptyHerdsTree(), emptyHerdsTree(), horsesTree() {}

Plains::~Plains() {
    nonEmptyHerdsTree.clear();
    emptyHerdsTree.clear();
    horsesTree.clear();
}


StatusType Plains::add_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }


    if (nonEmptyHerdsTree.find(herdId).status() == StatusType::SUCCESS ||
        emptyHerdsTree.find(herdId).status() == StatusType::SUCCESS) {
        return StatusType::FAILURE;
    }

    try {

        std::shared_ptr<Herd> newHerd = std::make_shared<Herd>(herdId);


        StatusType status = emptyHerdsTree.insert(herdId, newHerd);
        if (status != StatusType::SUCCESS) {
            return status;
        }
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}


StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }


    output_t<std::shared_ptr<Herd>> result = emptyHerdsTree.find(herdId);
    if (result.status() == StatusType::SUCCESS) {
        emptyHerdsTree.remove(herdId);
        return StatusType::SUCCESS;
    }

    return StatusType::FAILURE;
}


StatusType Plains::add_horse(int horseId, int speed) {
    if (horseId <= 0 || speed <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (horsesTree.find(horseId).status() == StatusType::SUCCESS) {
        return StatusType::FAILURE;
    }

    try {

        std::shared_ptr<Horse> newHorse = std::make_shared<Horse>(horseId, speed);


        StatusType status = horsesTree.insert(horseId, newHorse);
        if (status != StatusType::SUCCESS) {

            return status;
        }
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}


StatusType Plains::join_herd(int horseId, int herdId) {
    if (horseId <= 0 || herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }


    auto horseResult = horsesTree.find(horseId);
    if (horseResult.status() != StatusType::SUCCESS) {
        return StatusType::FAILURE;
    }

    std::shared_ptr<Horse> horse = horseResult.ans();

    if (horse->isInHerd()) {
        return StatusType::FAILURE;
    }


    auto herdResult = nonEmptyHerdsTree.find(herdId);
    if (herdResult.status() == StatusType::SUCCESS) {
        std::shared_ptr<Herd> herd = herdResult.ans();
        try {
            herd->addHorse(horse);
        } catch (std::bad_alloc&) {
            return StatusType::ALLOCATION_ERROR;
        }
        horse->joinHerd(herd);
        return StatusType::SUCCESS;
    }


    auto emptyHerdResult = emptyHerdsTree.find(herdId);
    if (emptyHerdResult.status() == StatusType::SUCCESS) {
        std::shared_ptr<Herd> herd = emptyHerdResult.ans();
        try {
            emptyHerdsTree.remove(herdId);
            nonEmptyHerdsTree.insert(herdId, herd);
            herd->addHorse(horse);
        } catch (std::bad_alloc&) {
            return StatusType::ALLOCATION_ERROR;
        }

        horse->joinHerd(herd);
        return StatusType::SUCCESS;
    }

    return StatusType::FAILURE;
}


StatusType Plains::follow(int horseId, int horseToFollowId) {
    if (horseId <= 0 || horseToFollowId <= 0 || horseId == horseToFollowId) {
        return StatusType::INVALID_INPUT;
    }


    auto horseResult = horsesTree.find(horseId);
    auto horseToFollowResult = horsesTree.find(horseToFollowId);

    if (horseResult.status() != StatusType::SUCCESS || horseToFollowResult.status() != StatusType::SUCCESS) {
        return StatusType::FAILURE;
    }

    std::shared_ptr<Horse> horse = horseResult.ans();
    std::shared_ptr<Horse> horseToFollow = horseToFollowResult.ans();


    if (!horse->isSameHerd(*horseToFollow)) {
        return StatusType::FAILURE;
    }

    try {

        std::shared_ptr<Linker> leadersLink = horseToFollow->makeLink();
        horseToFollow->getHerd()->increaseConnections();
        horse->leadersLinkToFollow(leadersLink);
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}


StatusType Plains::leave_herd(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }


    auto horseResult = horsesTree.find(horseId);
    if (horseResult.status() != StatusType::SUCCESS) {
        return StatusType::FAILURE;
    }

    std::shared_ptr<Horse> horse = horseResult.ans();


    if (!horse->isInHerd()) {
        return StatusType::FAILURE;
    }


    std::shared_ptr<Herd> herd = horse->getHerd();
    try {
        herd->removeHorse(horse);


        if (herd->getSize() == 0) {
            nonEmptyHerdsTree.remove(herd->getId());
            emptyHerdsTree.insert(herd->getId(), herd);
        }
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }


    horse->leaveHerdProperly();

    return StatusType::SUCCESS;
}


output_t<int> Plains::get_speed(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }


    auto result = horsesTree.find(horseId);
    if (result.status() == StatusType::FAILURE) {
        return StatusType::FAILURE;
    }


    std::shared_ptr<Horse> horse = result.ans();


    int speed = horse->getSpeed();
    return output_t<int>(speed);
}


output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0 || horseId == otherHorseId) {
        return StatusType::INVALID_INPUT;
    }

    auto horseResult = horsesTree.find(horseId);
    auto otherHorseResult = horsesTree.find(otherHorseId);

    if (horseResult.status() == StatusType::FAILURE || otherHorseResult.status() == StatusType::FAILURE) {
        return StatusType::FAILURE;
    }

    std::shared_ptr<Horse> horse = horseResult.ans();
    std::shared_ptr<Horse> otherHorse = otherHorseResult.ans();

    if (!horse->isSameHerd(*otherHorse)) {
        return output_t<bool>(false);
    }

    std::shared_ptr<Horse> current = horse;

    while (current) {
        if (current == otherHorse) {
            resetAllVisited(horse->getHerd());
            return output_t<bool>(true);
        }

        if (current->checkIfVisited()) {
            break;
        }

        current->markVisited();

        auto leaderLink = current->getLeaderLink().lock();
        if (!leaderLink) {
            break;
        }

        current = leaderLink->getHorse();
    }

    resetAllVisited(horse->getHerd());
    return output_t<bool>(false);
}




output_t<bool> Plains::can_run_together(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    auto result = nonEmptyHerdsTree.find(herdId);
    if (result.status() == StatusType::FAILURE) {
        return StatusType::FAILURE;
    }

    std::shared_ptr<Herd> herd = result.ans();
    bool canRunTogether = true;

    // מציאת שורש העדר
    std::shared_ptr<Horse> root = nullptr;
    herd->getHorsesTree().inOrderTraversal([&root](int, std::shared_ptr<Horse> horse) {
        if (!horse->getLeader()) {
            if (root) {
                root = nullptr;
                return;
            }
            root = horse;
        }
    });

    if (!root) {
        return output_t<bool>(false);
    }


    herd->getHorsesTree().inOrderTraversal([&](int, std::shared_ptr<Horse> horse) {
        if (!canRunTogether) return;

        std::shared_ptr<Horse> current = horse;

        while (current) {
            if (current->checkIfVisited()) {
                canRunTogether = false;
                return;
            }

            current->markVisited();

            if (current == root) {
                break;
            }

            auto leaderLink = current->getLeaderLink().lock();
            if (!leaderLink) {
                canRunTogether = false;
                return;
            }

            current = leaderLink->getHorse();
        }
    });

    resetAllVisited(herd);
    return output_t<bool>(canRunTogether);
}




void Plains::resetAllVisited(const std::shared_ptr<Herd>& herd) {
    herd->getHorsesTree().inOrderTraversal([](int, std::shared_ptr<Horse> horse) {
        horse->resetVisited();
    });
}