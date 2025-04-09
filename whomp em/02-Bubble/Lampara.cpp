#include "Lampara.h"
#include <iostream>

void Lampara::onCollect(Player& player) {
    player.actLamp(); 
    destroy();
}
