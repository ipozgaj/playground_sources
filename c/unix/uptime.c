#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROC_LINE 256

static __inline__ unsigned long long rdtsc(void)
{
  unsigned hi, lo;

  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));

  return ((unsigned long long) lo) | (((unsigned long long) hi) << 32);
}

float get_proc_speed(void)
{
  FILE *proc;
  char buff[MAX_PROC_LINE];
  float speed;

  if ((proc = fopen("/proc/cpuinfo", "rb")) == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  while (fgets(buff, MAX_PROC_LINE, proc)) {
    if (strstr(buff, "cpu MHz")) {
      sscanf(buff, "cpu MHz   : %f", &speed);
      return speed;
    }
  }

  return 0;
}

int main()
{
  unsigned long long tsc = rdtsc();

  printf("TSC: %llu (%3.2f days)\n", tsc, tsc/(get_proc_speed()*10e5)/24.0/60/60);

  return EXIT_SUCCESS;
}
