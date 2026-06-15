A Linux CLI tool for setting the archive bit on fat32 files.

compile using:
  gcc main.c -o set-archive-bit -O2

usage:
  ./a.out file1 [file2...]

This will attempt to set the archive bit on all passed file / directory paths.
