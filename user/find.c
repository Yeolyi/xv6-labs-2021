#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

char* fmtname(char *path) {
	char *p;

	for (p=path+strlen(path); p >= path && *p != '/'; p--);
	p++;

	return p;
}

void find(char *keyword, char* dir) {
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	if ((fd = open(dir, 0)) < 0) {
		printf("find: cannot open %s\n", dir);
		return;
	}

	if (fstat(fd, &st) < 0) {
		close(fd);
		return;
	}

	switch (st.type) {
		case T_DIR:
			while (read(fd, &de, sizeof(de)) == sizeof(de)) {
				if (de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, "..")) { continue; }
				strcpy(buf, dir);
				p = buf + strlen(buf);
				*p++ = '/';
				strcpy(p++, de.name);
				find(keyword, buf);
			}
			break;
		case T_FILE:
			if (!strcmp(keyword, fmtname(dir))) {
				printf("%s\n", dir);
			}
			break;
	}
	close(fd);
}

int main(int argc, char *argv[]) {
	find(argv[2], argv[1]);
	exit(0);
}
