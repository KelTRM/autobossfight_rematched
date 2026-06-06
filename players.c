// Temporary solution to define players

#include"entity.h"

Entity_t CreatePlayer(const char *Name, Health_t MaxHP, Health_t MinHeal, Health_t MaxHeal);

Entity_t Players[3];

void InitPlayers() {
	Players[0] = CreatePlayer("Player1", 30000, 2000, 7000);
	Players[1] = CreatePlayer("Player2", 30000, 2000, 7000);
	Players[2] = CreatePlayer("Player3", 30000, 2000, 7000);
}
