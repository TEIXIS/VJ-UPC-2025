#pragma once
#include "Collectible.h"

class Lampara : public Collectible {
public:
    using Collectible::init; 
    void onCollect(Player& player) override;
};
