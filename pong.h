
/*
 * 
 * PONG.H - Header
 * 
 * I'm use Allegro Library to develop this game.
 * 
 * Created by Osvaldo Santana Neto <acidbase@bigfoot.com>
 * 
 */

#include <stdio.h>
#include <allegro.h>
#include <jgmod.h>
#include <unistd.h>
#include <string.h>

#define SW		320
#define SH		200

#define STARTING_SPEED  100		// Music definitions
#define STARTING_PITCH  100
#define SND_VOLUME	255

#define F_BACK		"back.pcx"	// Image file names
#define	F_RAQ		"raq.pcx"
#define F_BALL		"ball.pcx"
#define F_SCORE		"score.pcx"
#define F_OPEN		"open.pcx"

#define RAQ1 		0		// raq aliases
#define RAQ2 		1
#define RAQ_WIDTH	12		// raq dimensions
#define RAQ_HEIGHT	61
#define RAQ_MAX_Y	138		// raq MAX and MIN y
#define RAQ_MIN_Y 	1

#define BALL_WIDTH 	8		// Ball dimensions
#define BALL_HEIGHT 	8

#define DRAW 		1		// Macro to Draw functions
#define REST 		0

struct RAQ {
	int x;
	int y;
	int p;
	BITMAP *b;
};

/*
 * Function prototypes
 */

void err(char *errmsg);
void raq_draw(char rn, char dr);
void ball_newpos(void);
void ball_draw(char dr);
void score(void);
void pong_exit(void);
void pong_setup(void);
void pong_opening(void);
void pong_about(void);

