#include<stdio.h>

struct spell {
	char *name;
	int cost;
	int damage;
	int heal;
	int armor;
	int mana;
	int lasting; // -1 --> immediate effect, no countdown needed
};

struct activespell {
	struct activespell* next;
	struct spell *spell;
	int	lasting;
}
	

struct spell spells[] = {
	{ "MagicMissile",      53, 4, 0, 0, 0, -1},
	{ "Drain",             73, 2, 2, 0, 0, -1},
	{ "Shield",           113, 0, 0, 7, 0, 6},
	{ "Poisson",          173, 3, 0, 0, 0, 6},
	{ "Recharge",         229, 0, 0, 0, 101, 5 }
};
#define MAGICMISSILE 0
#define DRAIN        1
#define SHIELD       2
#define POISSON      3
#define RECHARGE"    4

#define spellcount ((int) (sizeof spells / sizeof *spells))

struct player {
	int mana;
	int armor;
	int hitpoints;
	int activespells[spellcount];
};

// Returns 1 when spell remains active
int applyspell(struct player* player, struct player* opponent, int instant)
{
	if (instant && player->activespells[MAGICMISSILE]) {
		int totaldamage = activespell-->damage - opponent->armor;
		if (totaldamage <= 0) totaldamage = 1;
		opponent->hitpoints -= totaldamage;
		player->activespells[MAGICMISSILE] = 0;
	}
	if (instant && player->activespells[DRAIN]) {
		int totaldamage = activespell-->damage - opponent->armor;
		if (totaldamage <= 0) totaldamage = 1;
		opponent->hitpoints -= totaldamage;
		player->activespells[DRAIN] = 0;
	}

	if (activespell->damage) {

		// When damage applied there is at least 1 damage made
	}

	player->hitpoints += activespell->spell->heal;
}
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
					struct spell* selectedweapons[4];
					struct spell** w = selectedweapons;
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

