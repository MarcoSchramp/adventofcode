#include<stdlib.h>
#include<stdio.h>
#include<string.h>


#define REG_DIRECT	0
#define REG_A	1
#define REG_B	2
#define REG_C	3
#define REG_D	4

#define CMD_CPY_VAL	1
#define CMD_CPY_REG	2
#define CMD_INC	3
#define CMD_DEC	4
#define CMD_JNZ	5

struct command {
	char *line;
	int cmd;
	int value;
	int value2;
	int reg_source;
	int reg_target;
};

struct CPU {	
	int ip;
	long a;
	long b;
	long c;
	long d;
};

struct command program[100];
int  end = 0;

void run()
{
	struct CPU CPU;
	CPU.ip = 0;
	CPU.a = 0;
	CPU.b = 0;
#ifdef PART2
	CPU.c = 1;
#else
	CPU.c = 0;
#endif
	CPU.d = 0;
	

	int value;
	while (CPU.ip != end) {
		struct command *cmd = program + CPU.ip;
//		printf ("(%d %d %d %d %d) --> ", CPU.ip, CPU.a, CPU.b, CPU.c, CPU.d);
		switch (cmd->cmd) {
		case CMD_CPY_VAL:
			switch (cmd->reg_target) {
				case REG_A: CPU.a = cmd->value; break;
				case REG_B: CPU.b = cmd->value; break;
				case REG_C: CPU.c = cmd->value; break;
				case REG_D: CPU.d = cmd->value; break;
			}
			CPU.ip++;
			break;
		case CMD_CPY_REG:
			switch (cmd->reg_source) {
				case REG_A: value = CPU.a; break;
				case REG_B: value = CPU.b; break;
				case REG_C: value = CPU.c; break;
				case REG_D: value = CPU.d; break;
			}
			switch (cmd->reg_target) {
				case REG_A: CPU.a = value; break;
				case REG_B: CPU.b = value; break;
				case REG_C: CPU.c = value; break;
				case REG_D: CPU.d = value; break;
			}
			CPU.ip++;
			break;
		case CMD_INC:
			switch (cmd->reg_target) {
				case REG_A: CPU.a++; break;
				case REG_B: CPU.b++; break;
				case REG_C: CPU.c++; break;
				case REG_D: CPU.d++; break;
			}
			CPU.ip++;
			break;
		case CMD_DEC:
			switch (cmd->reg_target) {
				case REG_A: CPU.a--; break;
				case REG_B: CPU.b--; break;
				case REG_C: CPU.c--; break;
				case REG_D: CPU.d--; break;
			}
			CPU.ip++;
			break;
		case CMD_JNZ:
			switch (cmd->reg_source) {
				case REG_DIRECT: value = cmd->value2; break;
				case REG_A: value = CPU.a; break;
				case REG_B: value = CPU.b; break;
				case REG_C: value = CPU.c; break;
				case REG_D: value = CPU.d; break;
			}
			if (value)
				CPU.ip += cmd->value;
			else
				CPU.ip++;
			break;
		}
// printf ("(%d %d %d %d %d) %s", CPU.ip, CPU.a, CPU.b, CPU.c, CPU.d, cmd->line);
	}
	printf ("a = %ld\n", CPU.a);
}

void load()
{
	char line[100];
	while (fgets(line, sizeof line, stdin) != NULL) {
		struct command cmd;
		static char registers[] = "abcd";
		cmd.line = strdup(line);

		char* cmdtext = strtok(line, " \n");
		if (!strcmp(cmdtext, "cpy")) {
			char *in = strtok(NULL, " \n");
			char *out = strtok(NULL, " \n");
			char *reg = strchr(registers, *in);
			if (reg) {
				cmd.cmd =  CMD_CPY_REG;
				cmd.reg_source = reg - registers + 1;
			}
			else {
				cmd.cmd =  CMD_CPY_VAL;
				cmd.reg_source = REG_DIRECT;
				cmd.value = atoi(in);
			}
			cmd.reg_target = strchr(registers, *out) - registers + 1;
		}
		else if (!strcmp(cmdtext, "inc")) {
			char *out = strtok(NULL, " \n");
			cmd.cmd =  CMD_INC;
			cmd.reg_target = strchr(registers, *out) - registers + 1;
		}
		else if (!strcmp(cmdtext, "dec")) {
			char *out = strtok(NULL, " \n");
			cmd.cmd =  CMD_DEC;
			cmd.reg_target = strchr(registers, *out) - registers + 1;
		}
		else if (!strcmp(cmdtext, "jnz")) {
			char *in = strtok(NULL, " \n");
			char *out = strtok(NULL, " \n");
			cmd.cmd =  CMD_JNZ;
			char *reg = strchr(registers, *in);
			if  (reg)
				cmd.reg_source = strchr(registers, *in) - registers + 1;
			else {
				cmd.reg_source = REG_DIRECT;
				cmd.value2 = atoi(in);
			}
				
			cmd.value = atoi(out);
		}
		program[end++] = cmd;
	}
}

int main(int argc, char* argv[])
{
	load();
	run();

	return 0;
}
