/* anasrchid - fall 2016 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Bool{
  false = (1 == 0),
  true = !false
} bool;

struct {
  int x[8], y[8];
} moveon = {{2,1,-1,-2,-2,-1,1,2}, {1,2,2,1,-1,-2,-2,-1}};

/* main function */
int
main (int argc, char **argv)
{
  srand(time(NULL));

  int w = (argc < 2 || (w = atoi(argv[1])) <= 0) ? 8 : w,
    h = (argc < 3 || (h = atoi(argv[2])) <= 0) ? w : h;
  int size = w*h;
  int rstart = rand() % w, cstart = rand() % h;
  int a = moveon.x[0];

  printf("%d", a);
  printf("(%d*%d)%d\t{%d,%d}\n", w, h, size, rstart, cstart);
  return 0;
  
}

