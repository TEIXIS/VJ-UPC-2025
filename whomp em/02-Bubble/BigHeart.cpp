#include "BigHeart.h"
#include <iostream>

void BigHeart::onCollect(Player& player) {
    player.healallLives(); // reemplazá por tu método real de sumar vidas
    destroy();
}
