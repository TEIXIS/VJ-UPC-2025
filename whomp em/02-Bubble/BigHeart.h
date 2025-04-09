#pragma once
#include "Collectible.h"

class BigHeart : public Collectible {
public:
    using Collectible::init; 
    void onCollect(Player& player) override;
};
