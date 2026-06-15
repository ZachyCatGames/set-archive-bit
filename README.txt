A Linux CLI tool for setting the archive bit on fat32 files.
I wrote this for dealing with Nintendo Switch concatenation file stuff, but it works for other stuff too ofc.

compile using:
  gcc main.c -o set-archive-bit -O2

usage:
  ./set-archive-bit [option] file1 [file2...]
  option:
          -d: clear the archive bit
          -a: set the archive (default)

This will attempt to set the archive bit on all passed file / directory paths.
