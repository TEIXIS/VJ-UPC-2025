#include "Lampara.h"
#include <iostream>

void Lampara::onCollect(Player& player) {
    player.actLamp(); // reemplazá por tu método real de sumar vidas
    destroy();
}
