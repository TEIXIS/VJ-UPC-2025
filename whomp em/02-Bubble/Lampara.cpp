#include "Lampara.h"
#include <iostream>

void Lampara::onCollect(Player& player) {
    player.actLamp(); // reemplaz� por tu m�todo real de sumar vidas
    destroy();
}
