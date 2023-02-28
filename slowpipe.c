/*
 * MIT License
 *
 * Copyright (c) 2023 David Riesz
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

void usage(int argc, char **argv);
void dobreak();

static int finished = 0;

int main(int argc, char **argv)
{
	long ns, ss;
  double ms=0, mst;
	struct timespec tsp;
	char c;
  int ii;
  int debug=0;
  int valset=0;
  int silent=0;
  int *invalidargs = NULL;
  int invalidargcount = 0;
  char *endptr = NULL;
  size_t arglen;
  
  for(ii=1 ; ii<argc ; ii++)
  {
    if     (!(strcmp(argv[ii], "-g"))) { debug = 1; }
    else if(!(strcmp(argv[ii], "-h"))) { usage(argc, argv); }
    else if(!(strcmp(argv[ii], "-s"))) { silent = 1; }
  }

  if(silent)
  {
    printf("\e[?25l"); fflush(stdout);
    signal(SIGINT, dobreak);
  }
  
  if(debug)
  {
    fprintf(stderr, "\n****************************** Debug Info ******************************\n\n");
    fprintf(stderr, "Debug statements enabled.\n\n");
  }
  
  for(ii=1 ; ii<argc ; ii++)
  {
    if     (!(strcmp(argv[ii], "-g"))) {}
    else if(!(strcmp(argv[ii], "-s"))) {}
    else
    {
      arglen = strlen(argv[ii]);
      mst = strtod(argv[ii], &endptr);
      if(debug)
      {
        fprintf(stderr, "mst = %f (from %s) (error=%d)\n", mst, argv[ii], errno);
        fprintf(stderr, "  &(argv[%2d]) = %p\n", ii, argv[ii]);
        fprintf(stderr, "  endptr      = %p\n", endptr);
        fprintf(stderr, "  diff        = %ld\n", (endptr - argv[ii]));
        fprintf(stderr, "  arglen      = %ld\n", arglen);
        fprintf(stderr, "\n");
      }
      if(arglen != (endptr - argv[ii]))
      {
        invalidargcount++;
        invalidargs = (int *)realloc(invalidargs, (invalidargcount * sizeof(int)));
        invalidargs[invalidargcount-1] = ii;
        if(debug)
        {
          fprintf(stderr, "Detected invalid argument number %d (argv[%.2d] = \"%s\")\n", invalidargcount, ii, argv[ii]);
        }
      }
      else
      {
        ms = mst;
        if(debug) { fprintf(stderr, "Valid integer detected: \"%s\" == %f\n", argv[ii], ms); }
        valset = 1;
      }
    }
  }

	if(valset)
	{
    if(debug)
    {
      fprintf(stderr, "\nUsing ms value: %lf\n", ms);
    }
		ns = (long)(ms * 1000000.0);
		tsp.tv_nsec = ns % 1000000000;
		tsp.tv_sec  = ( ns - tsp.tv_nsec ) / 1000000000;

    if(debug)
    {
      fprintf(stderr, "tsp.tv_sec  = %ld\n", tsp.tv_sec);
      fprintf(stderr, "tsp.tv_nsec = %ld\n", tsp.tv_nsec);
    }

	}

  if(debug)
  {
    fprintf(stderr, "\n**************************** End Debug Info ****************************\n\n");
  }

  if(invalidargcount > 0)
  {
    for(ii=0 ; ii<invalidargcount ; ii++)
    {
      fprintf(stderr, "Invalid argument (%2d): \"%s\"\n", invalidargs[ii], argv[invalidargs[ii]]);
    }
    return 1;
  }

	while((read(STDIN_FILENO, &c, 1) > 0) && !finished)
	{
		write(STDOUT_FILENO, &c, 1); fflush(stdout);
		if(valset)
    {
      nanosleep(&tsp, NULL);
    }
	}

  if(silent)
  {
    printf("\e[?25h"); fflush(stdout);
  }

	return 0;
}

void usage(int argc, char **argv)
{
  char *cp = NULL;
  if(cp=strrchr(argv[0], '/')) { cp++; }
  else { cp = argv[0]; }
  
  fprintf(stderr, "\nslowpipe: print from STDIN to STDOUT slowly.\n\n");
  fprintf(stderr, "usage:\n\n");
  fprintf(stderr, "    %s <interval> [-g] [-h]\n\n", cp);
  fprintf(stderr, "    interval: sleep interval between characters in milliseconds\n");
  fprintf(stderr, "              e.g.: \"10\" or \"5.6\"\n");
  fprintf(stderr, "    -s      : silent mode (suppress curor)\n");
  fprintf(stderr, "    -g      : print debugging information\n");
  fprintf(stderr, "    -h      : print this help information\n");
  fprintf(stderr, "\n");
  exit(1);
}

void dobreak()
{
  printf("\n");
  finished = 1;
}
