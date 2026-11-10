/**
  ******************************************************************************
  * @file      syscalls.c
  * @brief     Minimal System Calls File for GCC toolchain
  ******************************************************************************
  */

#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

#include "main.h"

extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

register char * stack_ptr asm("sp");

void * _sbrk(ptrdiff_t incr)
{
  extern char end;
  static char *heap_end;
  char *prev_heap_end;

  if (heap_end == 0)
    heap_end = &end;

  prev_heap_end = heap_end;
  if (heap_end + incr > stack_ptr)
  {
    errno = ENOMEM;
    return (void *)-1;
  }

  heap_end += incr;
  return (void *)prev_heap_end;
}

int _write(int file, char *ptr, int len)
{
  (void)file;
  (void)ptr;
  return len;
}

int _close(int file)
{
  (void)file;
  return -1;
}

int _fstat(int file, struct stat *st)
{
  (void)file;
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file)
{
  (void)file;
  return 1;
}

int _lseek(int file, int ptr, int dir)
{
  (void)file;
  (void)ptr;
  (void)dir;
  return 0;
}

int _read(int file, char *ptr, int len)
{
  (void)file;
  (void)ptr;
  return len;
}

int _getpid(void)
{
  return 1;
}

int _kill(int pid, int sig)
{
  (void)pid;
  (void)sig;
  errno = EINVAL;
  return -1;
}

void _exit(int status)
{
  (void)status;
  while (1) {}
}
