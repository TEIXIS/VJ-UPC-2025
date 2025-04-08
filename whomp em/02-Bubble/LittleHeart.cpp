#include "Collectible.h"

class LifeUp : public Collectible {
public:
    void onCollect(Player& player) override {
        player.heal1Live();
        destroy();
    }
};