/********************************************************************************/
/* 				Anas Rchid - fall 2016 				*/
/*				   Lisence: LGPL (v3)				*/
/* 	   |\. 	   _          _       _     _   _       _                   	*/
/* 	  /   '.  | | ___ __ (_) __ _| |__ | |_( ___   | |_ ___  _   _ _ __ 	*/
/*	 /_.'-  \ | |/ | '_ \| |/ _` | '_ \| __|/ __|  | __/ _ \| | | | '__|	*/
/*  	    /   | |   <| | | | | (_| | | | | |_ \__ \  | || (_) | |_| | |   	*/
/* 	   /____| |_|\_|_| |_|_|\__, |_| |_|\__||___/   \__\___/ \__,_|_|   	*/
/* 	  `.____.'              |___/                                       	*/
/********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _GNU_LINUX
#include <getopt.h>
#endif

#define DIM(n, m) (n*m)

/* <--------------------( Enumerations )--------------------> */
/* Boolean type.  */
typedef enum BOOLEAN {
  false = (1 == 0),
  true = !false
} bool;

/* The status of an individual square on the board 
 * if it was visited by the Knight(1) or not(0).
 */
enum STATUS {
  NOTVISITED = 0,
  VISTED = 1
};
typedef enum ARGUMENTS {
  DEBUG = 0, RECUR,
  ALGEB, ROW, COLMN
}arg_t;

/* <--------------------( Data Structures )--------------------> */
/* The knight move is unusual among chess pieces. When it moves,
 * it can move to a square that is two squares horizontally(left/right) and one
 * square vertically(up/down), or two squares vertically and one square 
 * horizontally. The complete move therefore looks like the letter L.
 * The moves here are represented by pair {0 <= i < 8 | (x[i], y[i])} which 
 * represente all the possible moves of the night on the board. 
 */
struct KNIGHT_MOVES {
#define MOVES 8
  int x[MOVES], y[MOVES];
#undef MOVES
} moveon = {
  {2,1,-1,-2,-2,-1,1,2},	/* HORIZONTAL */
  {1,2,2,1,-1,-2,-2,-1}		/* VERTICAL */
};

/* Each pair represente a square or a positoin on the board, 
 * it is construted by a row and a column. */
typedef struct PAIR{
  int row, col;
} pair_t;

/* the Chess board */
typedef struct CHESS_BOARD {
  int **board;
  pair_t dim;
  int size;			/* (Width * Height) */
} board_t;

/* <--------------------( Prototypes )--------------------> */
/* isvisited(row, col, wid, hei)
 * 	Test if the current postion is on the board. 
 * 
 * @ARGS:
 * 	row, col: Coordinate of the square.
 * 	wid, hei: The board dimensions.
 * 
 * @RETURN:
 * 	1 if the square is ni the board.
 * 	0 if it's not.
 */
static inline bool isvisited(pair_t, pair_t);

/* ktour(board, steps, squr)
 * @ARGS:
 *	board: 	The chess board.
 *	steps: 	How much the Knight moves on the board.
 *	squr :	The next position on the board.
 * @RETURN:
 *	1 if the knight complete his tour on the board.
 *	0 if it is not possible to complete his tour.
 */
bool ktour(board_t *, int, pair_t);

/*
 * 
 */
board_t *initboard(int, char **, pair_t *);

/*
 * 
 */
void dropboard(board_t *);

/*
 *
 */
void checkargs(int, char **, pair_t *);

/* <--------------------( Global Variables )--------------------> */
bool debug 	= false;			/* -d --debug */
bool recursive 	= false;			/* -R --recursive */
bool algn 	= false;			/* -A --algebric-notation */
int argrow 	= 0;				/* -r --row */
int argcol 	= 0;				/* -c --column */

/* <---------------------------------------------------------> 
 * <--------------------( Main Function )--------------------> 
 * <--------------------------------------------------------->
 */
int
main (int argc, char **argv)
{
  srand(time(NULL));
  
  pair_t start = { -1, -1 };
  puts("@");
  checkargs(argc, argv, &start);
  
  board_t *chess = initboard(argc, argv, &start);

  
  dropboard(chess);
  return 0;
  
}

void
checkargs(int c, char **v, pair_t *s)
{
#define ARGS_NUMBER 5

  if(c > 2)
    while(true)
      {
	int j;
	int i, opt = 0;

	for(i = opt; i < c; ++i)	/* parse arguments */
	  for(j = 0; j < ARGS_NUMBER; ++j) /* fich arguments */
	    {
	      char short_args[] = "dRArc";
	      char *long_args[] = {
		"debug",			/* -d */
		"recursive",		/* -R */
		"algebric-notation",	/* -A */
		"row",			/* -r */
		"column"			/* -c */
	      };
	      char *sh_arg = NULL;
	      char *l_arg = NULL;
	      sprintf(sh_arg, "-%c", short_args[j]);
	      sprintf(l_arg, "--%s", long_args[j]);
	      if (!strcmp(v[i + 1], sh_arg) ||
		  !strcmp(v[i + 1], l_arg))
		{
		  int stemp;

		  switch(j)
		    {
		    case DEBUG:debug     = true; break;
		    case RECUR:recursive = true; break;
		    case ALGEB:algn      = true; break;
		    case ROW  :s->row = (stemp = atoi(v[i + 2])) <= 0 ? stemp : 0; break;
		    case COLMN:s->col = (stemp = atoi(v[i + 2])) <= 0 ? stemp : 0; break;
		    }
		  opt += 1;
		}
	    }
      }
#undef ARGS_NUMBER
}

board_t *
initboard(int c, char **v, pair_t *s)
{
  int i;
  int w = (c < 2 || (w = atoi(v[1])) <= 0) ? 8 : w;
  int h = (c < 3 || (h = atoi(v[2])) <= 0) ? w : h;
  board_t *t = (board_t *) malloc(sizeof(board_t ));

  /* starting position */
  if(s->row <= 0 || s->col <= 0)
    {
      s->row = rand() % w;
      s->col = rand() % h;
    }
  
  /* board dimensions */
  t->dim.col = h;
  t->dim.row = w;
  t->size = (w * h);
  
  /* allocate memory for the board */
  t->board = (int **) calloc(w, sizeof(int *));
  for(i = 0; i < h; ++i) t->board[i] = (int *) calloc(1, sizeof(int ));
  
  if(debug) printf("(%d*%d)%d\t{%d,%d}\n", w, h, t->size, s->row, s->col); 
  return t;
  /* return 0; */
}

void
dropboard(board_t *foo)
{
  int i;
  int n = foo->dim.row;
  
  for(i = 0; i < n; ++i)
    free(foo->board[i]);
  free(foo);
}

static inline bool
isvisited (pair_t square, pair_t dim)
{
  return (0 <= square.row && square.row < dim.row) &&
    (0 <= square.col && square.col < dim.col);
}

bool
ktour (board_t *board, int steps, pair_t squr)
{
  /* int move; */
  /* if(board == NULL) initboard(); */
  /* if(!isvisited(squr, board->dim) || ) */
  /*   return false; */
  return false;
}
