CROSS = arm-none-linux-gnueabi-gcc
#CROSS = gcc
flags=-o
all:mmap_read mmap_write

mmap_read:mmap_read.c
	$(CROSS) $(flags) mmap_read mmap_read.c
mmap_write:mmap_write.c
	$(CROSS) $(flags) mmap_write mmap_write.c

clean:
	rm -rf mmap_read mmap_write test

