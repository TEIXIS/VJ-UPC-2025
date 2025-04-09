#include "Capa.h"
#include <iostream>

void Capa::onCollect(Player& player) {
    player.setCapaActiva(true); 
    destroy();
}
