#include "kernel/types.h"
#include "user/user.h"

void sieve(int fd) {
	int first;
	int next;
	int result;
	int forked = 0;
	result = read(fd, &first, 4);
	if (!result) {
		close(fd);
		return;
	} else {
		printf("prime %d\n", first);
		int p[2];
		while ((result = read(fd, &next, 4))) {
			if (!forked) {
				pipe(p);
				if (!fork()) {
					close(p[1]);
					close(fd);
					sieve(p[0]);
					exit(0);
				} else { 
					close(p[0]);
					forked = 1;
				}
			}
			if (next % first != 0) {
				write(p[1], &next, 4);
			}
		}
		close(fd);
		close(p[1]);
		wait((void*)0);
		return;
	}
}

int main(void) {
	int p[2];
	pipe(p);
	if (fork()) {
		close(p[1]);
		sieve(p[0]);
		wait((void*)0);
		exit(0);
	} else {
		close(p[0]);
		for (int i=2; i<=35; i++) {
			write(p[1], &i, 4);
		}
		close(p[1]);
		exit(0);
	}
}
