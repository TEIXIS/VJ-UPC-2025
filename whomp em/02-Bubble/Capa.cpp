#include "Capa.h"
#include <iostream>

void Capa::onCollect(Player& player) {
    player.setCapaActiva(true); // reemplaz� por tu m�todo real de sumar vidas
    destroy();
}
