#pragma once
#include "Collectible.h"

class Capa : public Collectible {
public:
    using Collectible::init; 
    void onCollect(Player& player) override;
};
