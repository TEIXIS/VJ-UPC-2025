#include "Calabaza.h"
#include <iostream>

void Calabaza::onCollect(Player& player) {
    player.actCalabaza(); // reemplaz� por tu m�todo real de sumar vidas
    destroy();
}
