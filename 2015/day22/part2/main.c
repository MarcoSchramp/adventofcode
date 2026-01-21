#include<stdio.h>

#define MAGICMISSILE 0
#define DRAIN        1
#define SHIELD       2
#define POISON      3
#define RECHARGE    4
#define MAXSPELL    5

struct player {
	int mana;
	int armor;
	int hitpoints;
	int activespells[MAXSPELL];
};

// Returns 1 when spell remains active
int applyspell(struct player* player, struct player* opponent, int spell)
{
	if (spell == MAGICMISSILE && player->mana >= 53) {
		int totaldamage = 4 - opponent->armor;
		if (totaldamage <= 0) totaldamage = 1;
		opponent->hitpoints -= totaldamage;
		player->mana -= 53;
		return 53;
	}
	if (spell == DRAIN && player->mana >= 73)  {
		int totaldamage = 2 - opponent->armor;
		if (totaldamage <= 0) totaldamage = 1;
		opponent->hitpoints -= totaldamage;
		player->hitpoints += 2;
		player->mana -= 73;
		return 73;
	}
	if (spell == SHIELD && player->mana >= 113 && player->activespells[SHIELD] == 0) {
		player->activespells[SHIELD] = 6;
		player->armor = 7;
		player->mana -= 113;
		return 113;
	}
	if (spell == POISON && player->mana >= 173 && player->activespells[POISON] == 0) {
		player->activespells[POISON] = 6;
		player->mana -= 173;
		return 173;
	}
	if (spell == RECHARGE && player->mana >= 229 && player->activespells[RECHARGE] == 0) {
		player->activespells[RECHARGE] = 5;
		player->mana -= 229;
		return 229;
	}
	return 0;
}

void applybossspell(struct player* player, struct player* opponent)
{
	// Boss can only do one thing
	int totaldamage = 9 - player->armor;
	if (totaldamage <= 0) totaldamage = 1;
		player->hitpoints -= totaldamage;
}

void preroundeffect(struct player* player, struct player* opponent, int playerturn)
{
	if (playerturn)
		player ->hitpoints--;

	if (player->activespells[SHIELD])
		--player->activespells[SHIELD];
	else
		player->armor = 0;
	if (player->activespells[POISON]) {
		--player->activespells[POISON];
		opponent->hitpoints -= 3;
	}
	if (player->activespells[RECHARGE]) {
		--player->activespells[RECHARGE];
		player->mana += 101;
	}
}

int maxmana = 1000000;
void playerround(int spend, struct player player, struct player boss);
void bossround(int spend, struct player player, struct player boss);

// returns mana consumed
void bossround(int spend, struct player player, struct player boss)
{
	if (spend > maxmana)
		return;
	preroundeffect(&player, &boss, 0);
	if (player.hitpoints <= 0)
		return;
	else if (boss.hitpoints <= 0) {
		if (spend < maxmana)
			maxmana = spend;
		return;
	}
	applybossspell(&player, &boss);
	if (player.hitpoints <= 0)
		return;
	else if (boss.hitpoints <= 0 ) {
		if (spend < maxmana)
			maxmana = spend;
		return;
	}
	else
		playerround(spend, player, boss);
}
void playerround(int spend, struct player player, struct player boss)
{
	// no need to go any further
	if (spend > maxmana)
		return;
	preroundeffect(&player, &boss, 1);
	if (player.hitpoints <= 0)
		return;
	else if (boss.hitpoints <= 0) {
		if (spend < maxmana)
			maxmana = spend;
		return;
	}
	for (int spell = 0; spell < MAXSPELL; spell++) {
		struct player nextplayer = player;
		struct player nextboss = boss;
		int spellspend = applyspell(&nextplayer, &nextboss, spell);
		if (spellspend) {
			if (player.hitpoints <= 0) {
				// skip this
			}
			else if (boss.hitpoints <= 0) {
				if (spend < maxmana)
					maxmana = spend;
				return;
			}
			else {
				bossround(spend+spellspend, nextplayer, nextboss);
			}
		}
	}
}



int main(int argc, char* argv[])
{
	struct player player = {
		500 /* mana */, 
		0 /* armor */,
		50 /* hitpoints */,
		{ 0, 0, 0, 0, 0} /* activespells[MAXSPELL] */
	};
	struct player boss = {
		500 /* mana */, 
		0 /* armor */,
		51 /* hitpoints */,
		{ 0, 0, 0, 0, 0} /* activespells[MAXSPELL] */
	};
	playerround(0, player, boss);
	printf ("Minspend = %d\n", maxmana);
}

