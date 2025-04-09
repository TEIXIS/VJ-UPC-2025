#include "LittleHeart.h"
#include <iostream>

void LittleHeart::onCollect(Player& player) {
    player.heal1Live();
    destroy();
}
