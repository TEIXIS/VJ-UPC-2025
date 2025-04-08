#include "Capa.h"
#include <iostream>

void Capa::onCollect(Player& player) {
    player.setCapaActiva(true); // reemplazá por tu método real de sumar vidas
    destroy();
}
