#include "BigHeart.h"
#include <iostream>

void BigHeart::onCollect(Player& player) {
    player.healallLives(); // reemplaz� por tu m�todo real de sumar vidas
    destroy();
}
