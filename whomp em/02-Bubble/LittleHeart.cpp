#include "LittleHeart.h"
#include <iostream>

void LittleHeart::onCollect(Player& player) {
    player.heal1Live(); // reemplazá por tu método real de sumar vidas
    destroy();
}
