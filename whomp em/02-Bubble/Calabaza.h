#pragma once
#include "Collectible.h"

class Calabaza : public Collectible {
public:
    using Collectible::init; 
    void onCollect(Player& player) override;
};
