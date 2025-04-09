#pragma once
#include "Collectible.h"

class LittleHeart : public Collectible {
public:
    using Collectible::init;
    void onCollect(Player& player) override;
};
