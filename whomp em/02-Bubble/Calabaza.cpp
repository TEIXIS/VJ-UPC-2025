#include "Calabaza.h"
#include <iostream>

void Calabaza::onCollect(Player& player) {
    player.actCalabaza(); // reemplazá por tu método real de sumar vidas
    destroy();
}
