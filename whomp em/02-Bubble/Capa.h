#pragma once
#include "Collectible.h"

class Capa : public Collectible {
public:
    using Collectible::init; // reutiliza init() de la clase base
    void onCollect(Player& player) override;
};
