#pragma once
#include "Collectible.h"

class Calabaza : public Collectible {
public:
    using Collectible::init; // reutiliza init() de la clase base
    void onCollect(Player& player) override;
};
