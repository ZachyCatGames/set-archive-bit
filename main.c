#include <linux/msdos_fs.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char** argv) {
	printf("%d\n", argc);
	if (argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
		printf(
			"set-archive-bit by zachycatgames\n"
			"%s file1 [file2...]\n",
			argv[0]
		);
		return EXIT_SUCCESS;
	}

	bool failed = false;
	for (int i = 0; i < argc - 1; i++) {
		char* cur_path = argv[i+1];
		int fd = open(cur_path, O_DIRECTORY | O_RDONLY);
		if (fd < 0) {
			printf("Failed to open file %s: %s\n", cur_path, strerror(errno));
			failed = true;
			goto cleanup;
		}

		uint32_t attrib;
		int res = ioctl(fd, FAT_IOCTL_GET_ATTRIBUTES, &attrib);
		if (res < 0) {
			printf("Failed to read FAT attributes for %s: %s\n", cur_path, strerror(errno));
			failed = true;
			goto cleanup;
		}

		attrib |= ATTR_ARCH;
		res = ioctl(fd, FAT_IOCTL_SET_ATTRIBUTES, &attrib);
		if (res < 0) {
			printf("Failed to write FAT attributes for %s: %s\n", cur_path, strerror(errno));
			failed = true;
		}

		cleanup:
		if (fd > 0) {
			close(fd);
		}
	}

	printf("Done!\n");
	if (failed) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
