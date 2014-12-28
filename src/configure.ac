#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(embedded, v1.0, txianb@gmail.com)
AC_CONFIG_SRCDIR([mmap/mmap_write.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(embedded, v1.0)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h malloc.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MMAP
AC_CHECK_FUNCS([memset mkfifo munmap socket])

AC_OUTPUT([
		Makefile
		fifo/Makefile
		unix_socket/Makefile
		mmap/Makefile
		systemv/Makefile
		])
