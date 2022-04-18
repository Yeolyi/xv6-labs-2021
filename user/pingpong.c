#include "kernel/types.h"
#include "user/user.h"

int main(void) {
	int p[2];
	pipe(p);
	if (fork()) {
		write(p[1], "0", 1);
		read(p[0], 0, 1); 
		printf("%d: received pong\n", getpid());
		close(p[0]);
		close(p[1]);
		exit(0);
	} else {
		read(p[0], 0, 1);
		printf("%d: received ping\n", getpid());
		write(p[1], "0", 1);	
		close(p[0]);
		close(p[1]);
		exit(0);
	}
}

