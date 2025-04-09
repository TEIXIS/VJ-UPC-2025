#include "Calabaza.h"
#include <iostream>

void Calabaza::onCollect(Player& player) {
    player.actCalabaza(); 
    destroy();
}
