#include "LittleHeart.h"
#include <iostream>

void LittleHeart::onCollect(Player& player) {
    player.heal1Live(); // reemplaz� por tu m�todo real de sumar vidas
    destroy();
}
