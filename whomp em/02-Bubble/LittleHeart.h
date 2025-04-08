#pragma once
#include "Collectible.h"

class LittleHeart : public Collectible {
public:
    void onCollect(Player& player) override;
};
#pragma once
