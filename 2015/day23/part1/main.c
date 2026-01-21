#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CMD_HALF	1
#define	CMD_TRIPLE	2
#define	CMD_INC		3
#define	CMD_JMP		4
#define CMD_JMPEVEN	5
#define CMD_JMPODD	6
#define CMD_STOP	0

#define REG_A		0
#define REG_B		1

#define MAX_PROG	10000
#define MAX_LENGTH	100

struct instruction {
	int cmd;
	int reg;
	int value;
};

struct instruction* instructions;
unsigned int reg_a = 0;
unsigned int reg_b = 0;
struct instruction* reg_pc;

#define JMP_OFFSET 0

void run()
{
	while(1) {
		printf ("a: %3u b: %3u cmd %d: %d\n", reg_a, reg_b, (int) (reg_pc - instructions), reg_pc->cmd);
		switch(reg_pc->cmd) {
			case CMD_HALF:
				if (reg_pc->reg == REG_A) reg_a /= 2;
				if (reg_pc->reg == REG_B) reg_b /= 2;
				reg_pc++;
				break;
			case CMD_TRIPLE:
				if (reg_pc->reg == REG_A) reg_a *= 3;
				if (reg_pc->reg == REG_B) reg_b *= 3;
				reg_pc++;
				break;
			case CMD_INC:
				if (reg_pc->reg == REG_A) reg_a++;
				if (reg_pc->reg == REG_B) reg_b++;
				reg_pc++;
				break;
			case CMD_JMP:
				reg_pc += reg_pc->value;
				break;
			case CMD_JMPEVEN:
				if (reg_pc->reg == REG_A && reg_a % 2 == 0)
					reg_pc += reg_pc->value - JMP_OFFSET;
				else if (reg_pc->reg == REG_B && reg_b % 2 == 0)
					reg_pc += reg_pc->value - JMP_OFFSET;
				else
					reg_pc++;
				break;
			case CMD_JMPODD:
				if (reg_pc->reg == REG_A && reg_a == 1)
					reg_pc += reg_pc->value - JMP_OFFSET;
				else if (reg_pc->reg == REG_B && reg_b == 1)
					reg_pc += reg_pc->value - JMP_OFFSET;
				else
					reg_pc++;
				break;
			case CMD_STOP:
				return;
		}
	}
}


int main(int argc, char* argv[])
{
	// Allocate on the heap and clear program memory
	instructions = malloc(MAX_PROG * sizeof *instructions);
	memset(instructions, 0, MAX_PROG * sizeof *instructions);

	char line[MAX_LENGTH];
	struct instruction* load = instructions;
	reg_pc = instructions;

	while (fgets(line, sizeof line, stdin) != NULL) {
		fputs(line, stdout);
		char* command = strtok(line, " ,\n");
		if (!strcmp(command, "hlf")) {
			char *reg = strtok(NULL, " ,\n");
			load->cmd = CMD_HALF;
			load->reg = strcmp(reg, "a") ? REG_B : REG_A;
		}
		if (!strcmp(command, "tpl")) {
			char *reg = strtok(NULL, " ,\n");
			load->cmd = CMD_TRIPLE;
			load->reg = strcmp(reg, "a") ? REG_B : REG_A;
		}
		if (!strcmp(command, "inc")) {
			char *reg = strtok(NULL, " ,\n");
			load->cmd = CMD_INC;
			load->reg = strcmp(reg, "a") ? REG_B : REG_A;
		}
		if (!strcmp(command, "jmp")) {
			char *distance = strtok(NULL, " ,\n");
			load->cmd = CMD_JMP;
			load->value = atoi(distance);
		}
		if (!strcmp(command, "jie")) {
			char *reg = strtok(NULL, " ,\n");
			char *distance = strtok(NULL, " ,\n");
			load->cmd = CMD_JMPEVEN;
			load->reg = strcmp(reg, "a") ? REG_B : REG_A;
			load->value = atoi(distance);
		}
		if (!strcmp(command, "jio")) {
			char *reg = strtok(NULL, " ,\n");
			char *distance = strtok(NULL, " ,\n");
			load->cmd = CMD_JMPODD;
			load->reg = strcmp(reg, "a") ? REG_B : REG_A;
			load->value = atoi(distance);
		}
		load ++;
	}

	run();

	printf ("b = %u\n", reg_b);
}
