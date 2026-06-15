#include <linux/msdos_fs.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

void usage(const char* name) {
	printf(
		"set-archive-bit by zachycatgames\n"
		"%s [option] file1 [file2...]\n"
		"option:\n"
		"\t-d: clear the archive bit\n"
		"\t-a: set the archive (default)\n",
		name
	);
	exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
	if (argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
		usage(argv[0]);
	}

	bool delete = false;
	if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-a") == 0) {
		if (argc < 3) {
			usage(argv[0]);
		}
		if (strcmp(argv[1], "-d")) {
			delete = true;
		}
		// else "-a" is set and nothing needs to be done
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

		if (delete) {
			attrib &= ~ATTR_ARCH;
		} else {
			attrib |= ATTR_ARCH;
		}

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
