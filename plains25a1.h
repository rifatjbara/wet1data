#ifndef PLAINS25WINTER_WET1_H_
#define PLAINS25WINTER_WET1_H_

#include "wet1util.h"
#include "Herd.h"
#include "AVLTree.h"
#include <memory> // For std::shared_ptr and std::weak_ptr

class Plains {
private:
    AVLTree<int, std::shared_ptr<Herd>> nonEmptyHerdsTree; // עץ לעדרים שאינם ריקים
    AVLTree<int, std::shared_ptr<Herd>> emptyHerdsTree;   // עץ לעדרים ריקים
    AVLTree<int, std::shared_ptr<Horse>> horsesTree;      // עץ לסוסים

public:
    // <DO-NOT-MODIFY> {
    Plains();

    ~Plains();

    StatusType add_herd(int herdId);

    StatusType remove_herd(int herdId);

    StatusType add_horse(int horseId, int speed);

    StatusType join_herd(int horseId, int herdId);

    StatusType follow(int horseId, int horseToFollowId);

    StatusType leave_herd(int horseId);

    output_t<int> get_speed(int horseId);

    output_t<bool> leads(int horseId, int otherHorseId);

    output_t<bool> can_run_together(int herdId);
    // } </DO-NOT-MODIFY>

    void resetAllVisited(const std::shared_ptr<Herd>& herd);
};

#endif // PLAINS25WINTER_WET1_H_
