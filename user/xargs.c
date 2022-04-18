#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
	char *commandName = argv[1];
	char *arg[MAXARG], *p;

	arg[0] = argv[1];	
	arg[1] = malloc(100);
	arg[2] = 0;

	p = arg[1];
	while (read(0, p, 1)) {
		if (*p == '\n' || *p == 0) {
			*p = 0;
			if (fork()) {
				p = arg[1];
				wait(0);
			} else {
				exec(commandName, arg);
			}
		}
		p++;
	}
	free(arg[1]);
	exit(0);
}
