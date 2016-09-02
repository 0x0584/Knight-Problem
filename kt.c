/* test */
/********************************************************************************/
/* 				Anas Rchid - fall 2016 				*/
/*				   Lisence: GPL (v2)				*/
/* 	   |\. 	   _          _       _     _   _       _                   	*/
/* 	  /   '.  | | ___ __ (_) __ _| |__ | |_( ___   | |_ ___  _   _ _ __ 	*/
/*	 /_.'-  \ | |/ | '_ \| |/ _` | '_ \| __|/ __|  | __/ _ \| | | | '__|	*/
/*	    /   | |   <| | | | | (_| | | | | |_ \__ \  | || (_) | |_| | |   	*/
/* 	   /____| |_|\_|_| |_|_|\__, |_| |_|\__||___/   \__\___/ \__,_|_|   	*/
/* 	  `.____.'              |___/                                       	*/
/********************************************************************************/

/*	 <--------------------( Abstract )--------------------> 
 * A knight's tour is a sequence of moves of a knight on a chessboard 
 * such that the knight visits every square only once. If the knight ends
 * on a square that is one knight's move from the beginning square 
 * (so that it could tour the board again immediately, following the same path),
 * the tour is closed, otherwise it is open.
 *
 * The knight's tour problem is the mathematical problem of finding a knight's
 * tour. Creating a program to find a knight's tour is a common problem 
 * given to computer science students.[1] Variations of the knight's tour
 * problem involve chessboards of different sizes than the usual 8 × 8,
 * as well as irregular (non-rectangular) boards. (Wikipedia)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <popt.h>

/* the number of the squares in the board */
#define DIM(n, m) (n * m)

/* The status of an individual square on the board 
 * if it was visited by the Knight(1) or not(0).
 */
#define NOTVISITED 0
#define VISITED  1
#define STEP_INIT 0

/* <--------------------( Enumerations )--------------------> */
/* Boolean type.  */
typedef enum __BOOLEAN {
  false = (1 == 0),
  true = !false
} bool;

/* <--------------------( Data Structures )--------------------> */
/* The knight move is unusual among chess pieces. When it moves,
 * it can move to a square that is two squares horizontally(left/right)
 * and one square vertically(up/down), or two squares vertically and
 * one square horizontally. The complete move therefore looks 
 * like the letter L.
 *
 * The moves here are represented by pair {0 <= i < 8 | (x[i], y[i])} which 
 * represente all the possible moves of the night on the board. 
 * 
 * possible moves of the knight:
 */
#define KNIGHT_MOVES  8
/* (1H, 2V) (-1H, -2V) (1H, -2V) (-1, 2V)
 * (2H, 1V) (-2H, 1V) (2H, -1V) (-2H, 1V)
 */
struct __KNIGHT_MOVES {
  int x[KNIGHT_MOVES], y[KNIGHT_MOVES];
} moveon = {
  {2,1,-1,-2,-2,-1,1,2},	/* HORIZONTAL */
  {1,2,2,1,-1,-2,-2,-1}		/* VERTICAL */
};

/* Each pair represente a square or a position on the board, 
 * it is construted by a row and a column. */
typedef struct __PAIR {
  int row, col;
} pair_t;

/* the Chess board */
typedef struct __CHESS_BOARD {
  short **board;
  pair_t dim;
  int size;			/* DIM(Width * Height) */
} board_t;

/* <--------------------( Prototypes )--------------------> */
/* isvisited(row, col, wid, hei)
 * 	Test if the current postion is on the board. 
 * 
 * @PARAMETERS:
 * 	row, col: Coordinate of the square.
 * 	wid, hei: The board dimensions.
 * 
 * @RETURN:
 * 	1 if the square is ni the board.
 * 	0 if it's not.
 */
static inline bool ispossible(pair_t, board_t *);

/* ktour(board, steps, squr)
 *
 * @PARAMETERS:
 *	board: 	The chess board.
 *	steps: 	How much the Knight moves on the board.
 *	squr :	The next position on the board.
 * 
 * @RETURN:
 *	1 if the knight complete his tour on the board.
 *	0 if it is not possible to complete his tour.
 */
bool ktour(board_t *, pair_t, int);

/*initboard(square)
 *	this function initialize all the necessary variable
 *	to start the tour. 
 * 
 * @PARAMETERS: 
 *	 squrae: the starting square (row/column).
 *
 * @RETURN:
 *	a CHESS_BOARD structure.
 */
board_t *initboard(pair_t *);

/*dropboard(board)
 * free the board from memory 
 *
 * @PARAMETERS:
 *	board: the chess board structure.
 */
void dropboard(board_t *);

/* chechargs(argc, argv, square);
 *	handle arguments parsed to the program using libpopt
 *
 * @PARAMETERS:
 *	argc, argv: argument counter and vector.
 *	square:	    starting square .
 */
void checkargs(int, const char **, pair_t *);

void putboard(board_t *);

/* <--------------------( Global Variables )--------------------> */
/* Arguments of the program */
bool debug 	= false;	/* -d --debug */
bool recursive 	= false;	/* -R --recursive */
bool algn 	= false;	/* -A --algebric-notation */
int width;			/* -w --width */
int height;			/* -h --height */
  pair_t start = { -1, -1 };

/* <---------------------------------------------------------> 
 * <--------------------( Main Function )--------------------> 
 * <--------------------------------------------------------->
 */
int
main (int argc, const char **argv)
{
  srand(time(NULL));
  
  /* int i, j; */
  /* puts("@"); */
  checkargs(argc, argv, &start);
  board_t *chess = initboard(&start);  

  printf("\n");
  
  putboard(chess);  

  if(!ktour(chess, start, STEP_INIT)) puts("No Solution!");
  else {
    puts("Tour exist");
    putboard(chess);
  }
  dropboard(chess);
  
  return EXIT_SUCCESS;
  
}

void
checkargs(int c, const char **v, pair_t *s)
{
  /* int argrow 	= 0;     */
  /* int argcol 	= 0; */
  char rc;
  
  struct poptOption optionsTable[] = {
    {"debug", 'd', POPT_ARG_NONE, &debug, \
     'd', "show debuging messages", NULL },
    {"recursive", 'R', POPT_ARG_NONE, &recursive, \
     'R', "make a tour from every position on the board \
and show the log", NULL},
    {"algebric-notation", 'A', POPT_ARG_NONE, &algn, \
     'A', "show moves as chess algebric notation", NULL }, 
    {"row", 'r', POPT_ARG_INT, &start.row, \
     'r', "set spesifice row to start with", "6" }, 
    {"column", 'c', POPT_ARG_INT, &start.col,\
     'c', "set specifice column to start with", "2"},
    {"width", 'w', POPT_ARG_INT, &width, \
     'w', "set the width of the board", "16"},
    {"height", 'h', POPT_ARG_INT, &height, \
     'h', "set the height of the board", "16"},
    POPT_AUTOHELP
    POPT_TABLEEND
  };
  poptContext optCon = poptGetContext(NULL, c, v, optionsTable, 0);

  poptSetOtherOptionHelp(optCon, "[OPTIONS]");

  while((rc = poptGetNextOpt(optCon)) >= 0) 
    switch(rc) {
    case 'd':	debug = true;
      break;
    case 'R':	recursive = true;
      break;
    case 'A':	algn = true;
      break;
    case 'r':	/* s->row = argrow; */
      break;
    case 'c':	/* s->col = argcol; */
      break;
    case 'w':		/*  */
      break;
    case 'h':		/*  */
      break;
    default:
      poptPrintUsage(optCon, stderr, 0);
      exit(1);
    }
  
  /* if(debug) printf("[%d, %d]\n", width, height); */
  printf("#%d,%d\n",start.row,start.col);
  getchar();
  putchar('\n');
  if(recursive) puts("recursive enabled");
  if(algn) puts("algebric notation enabled");
  if(debug) puts("debug enabled");
  putchar('\n');
}

board_t *
initboard(pair_t *s)
{
  int i, j;
  int w = (w = width) <= 0 ? 8 : w, h = (h = height) <= 0 ? w : h;
  board_t *t = (board_t *) malloc(sizeof(board_t ));
  
  /* board dimensions */
  t->dim.col = h;
  t->dim.row = w;
  t->size = DIM(h, w);

  //puts("@");
  /* starting position */
    printf("#%d,%d\n",start.row,start.col);
    getchar();
  if((s->row >= 0 && s->row < w)) s->row = rand() % (w-1);
  /* if(s->row >= w) s->row = rand() %(w-1); */
  
  if((s->col >= 0 && s->col < h)) s->col = rand() % (h-1);
  /* if(s->col >= h) s->col = rand() % (h-1); */
  
      printf("#%d,%d\n",start.row,start.col);
  getchar();
  /* allocate memory for the board */
  t->board = (short **) malloc(w * sizeof(short *));
  for(i = 0; i < w; ++i) t->board[i] = (short *) malloc( h * sizeof(short ));

  if(debug) printf("(%d*%d)%d\t{%d,%d}\n", w, h, t->size, s->row, s->col);

  for(i = 0; i < t->dim.row; ++i)
    for(j = 0; j < t->dim.col; ++j)
      t->board[i][j] = (i == s->row && s->col == j) ? true : false;

  return t;
  /* return 0; */
}

void
dropboard(board_t *foo)
{
  int i;
  int n = foo->dim.row;
  
  for (i = 0; i < n; ++i)
    free(foo->board[i]);
  
  free(foo);
}

static inline bool
ispossible(pair_t square, board_t *foo)
{
  return (0 <= square.row && square.row < foo->dim.row) &&
    (0 <= square.col && square.col < foo->dim.col) &&
    (foo->board[square.row][square.col] == 0);
}

static inline bool
isfinished(board_t *foo)
{
  int i, j;

  for(i = 0; i < foo->dim.row; ++i)
    for(j = 0; j < foo->dim.col; ++j)
      if(foo->board[i][j] == NOTVISITED)
	return false;

  return true;
}

/* ktour */
bool
ktour (board_t *board, pair_t current, int steps)
{
  int i;
  if (steps == (board->size - 1)) return true;

  for(i = 0; i < 8; ++i) {
    pair_t next = {
      current.row + moveon.x[i],
      current.col + moveon.y[i]
    };

    if(ispossible(next, board)) {
      board->board[next.row][next.col] = steps + 1;
      if(ktour(board, next, steps + 1) == true) return true;
      else 
	board->board[next.row][next.col] = false;
    }
  }
  
  return false; 
}

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KNRM  "\x1B[0m"

void
putboard(board_t *c)
{
  int i, j;
  char *spc = "  ";
  
  printf("%s ", spc);
  
  /* A B C... */
  for(i = 'a'; i < 'a' + c->dim.col; ++i)
    printf("  %c ", i);
  printf("\n");

  for(i = 0; i < c->dim.row; ++i) {
    
    printf("%s ", spc);
    for(j = 0; j < c->dim.col; ++j)
      printf("+---");
    printf("+\n");

    /* 1 2 3... */
    printf("%-3d", i);
    for(j = 0; j < c->dim.col; ++j) {
      printf("|");
      if(c->board[i][j]) printf(KGRN);
      printf(" %d ", c->board[i][j]);
      printf(KNRM);
    }
    printf("|\n");
  }

  printf("%s ", spc);
  for(j = 0; j < c->dim.col; ++j)
    printf("+---");
  printf("+\n");
}
