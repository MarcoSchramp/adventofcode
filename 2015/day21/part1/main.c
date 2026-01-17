#include<stdio.h>

struct weapon {
	char *name;
	int cost;
	int damage;
	int armor;
};

struct weapon weapons[] = {
	{ "Dagger",      8, 4, 0},
	{ "Shortsword", 10, 5, 0},
	{ "Warhammer" , 25, 6, 0},
	{ "Longsword",  40, 7, 0},
	{ "Greataxe",   74, 8, 0}
};

struct weapon armors[] = {
	{ "Leather",      13,     0,       1}, 
	{ "Chainmail",    31,     0,       2}, 
	{ "Splintmail",   53,     0,       3}, 
	{ "Bandedmail",   75,     0,       4}, 
	{ "Platemail",   102,     0,       5}
};

struct weapon rings[]  = {
	{ "Damage +1",    25,     1,       0 },
	{ "Damage +2",    50,     2,       0 },
	{ "Damage +3",   100,     3,       0 },
	{ "Defense +1",   20,     0,       1 },
	{ "Defense +2",   40,     0,       2 },
	{ "Defense +3",   80,     0,       3 }
};

#define weaponcount ((int) (sizeof weapons / sizeof *weapons))
#define armorcount ((int) (sizeof armors / sizeof *armors))
#define ringcount ((int) (sizeof rings / sizeof *rings))

struct player {
	int score;
	int damage;
	int armor;
};

// Total nr of combinations = 5 * 5 * 6 *6 = 900
// Let's brute-force this

// pass deep copies!
// return 1 when player wins, return 0 when boss wins
// return 2 when nobody wins
int playgame(struct player player, struct player boss)
{
	int bossdown = player.damage - boss.armor;
	int playerdown = boss.damage - player.armor;
	if (bossdown < 0) bossdown = 0;
	if (playerdown < 0) playerdown = 0;

	// 
	if (bossdown == 0 && playerdown == 0)
		return 2;
	while(1) {
		boss.score -= bossdown;
		if (boss.score <= 0) return 1;
		player.score -= playerdown;
		if (player.score <= 0) return 0;
	}
}



int main(int argc, char* argv[])
{
	struct player boss = { 104, 8, 1 };
	int mincost = 100000;
	for (int weapon = 0; weapon < weaponcount; weapon++) {
		for (int armor = -1; armor < armorcount; armor++) {
			for (int ring1 = -1 ; ring1 < ringcount; ring1++) {
				for (int ring2 = ring1 +1 ; ring2 <= ringcount; ring2++) {
					struct weapon* selectedweapons[4];
					struct weapon** w = selectedweapons;
					*(w++) = &weapons[weapon];
					if (armor != -1) *(w++) = &armors[armor];
					if (ring1 != -1) *(w++) = &rings[ring1];
					if (ring2 != ringcount) *(w++) = &rings[ring2];
					struct player player = { 100, 0, 0 };
					int cost = 0;
					for (w--;w >= selectedweapons; w--) {
						cost += (*w)->cost;
						player.damage += (*w)->damage;
						player.armor += (*w)->armor;
					}
					// No need to play the game when the cost is more then
					// the minimum cost so far....
					if (cost < mincost) {
						int won = playgame(player, boss);
						if (won == 1)
							mincost = cost;
					}
	
				}

			}
		}
	}
	printf("Mincost = %d\n", mincost);

	return 0;
}

