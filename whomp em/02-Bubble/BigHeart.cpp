#include "BigHeart.h"
#include <iostream>

void BigHeart::onCollect(Player& player) {
    player.healallLives();
    destroy();
}
