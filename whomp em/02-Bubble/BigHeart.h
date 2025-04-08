#pragma once
#include "Collectible.h"

class BigHeart : public Collectible {
public:
    using Collectible::init; // reutiliza init() de la clase base
    void onCollect(Player& player) override;
};
