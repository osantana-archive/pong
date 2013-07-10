/*
 * 
 * PONG.C - Main file
 * 
 * Created by Osvaldo Santana Neto <acidbase@bigfoot.com>
 * 
 */

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <allegro.h>
#include <jgmod.h>
#include <unistd.h>
#include <string.h>

#define starting_speed  100		// Music velocity
#define starting_pitch  100

#define f_back		"back.pcx"	// Image file names
#define	f_raq		"raq.pcx"
#define f_ball		"ball.pcx"
#define f_score		"score.pcx"
#define f_open		"open.pcx"

#define raq1 		0		// raq aliases
#define raq2 		1
#define raq_WIDTH	12		// raq dimensions
#define raq_HEIGHT	61
#define raq_MAX_Y	138		// raq MAX and MIN y
#define raq_MIN_Y 	1

#define ball_WIDTH 	8		// Ball dimensions
#define ball_HEIGHT 8

#define DRAW 		1		// Macro to Draw functions
#define REST 		0

struct RAQ {
	int x;
	int y;
	int p;
	BITMAP *b;
};

/*
 * Define variables
 */
struct RAQ raqs[2];
BITMAP *raq;		// Raq image
BITMAP *back;		// Background image
BITMAP *ball;		// Ball image
BITMAP *b_ball;		// Ball buffer
PALETTE palette;	// Game Palette
JGMOD *music;		// Music
int chn_reserved; 	// channel to be reserved

/*
 * Function prototypes
 */

void err(char *errmsg);
void raq_draw(unsigned int rn, char dr, BITMAP *r);
void ball_newpos(int *x, int *y, int *ix, int *iy);
void ball_draw(int x, int y, char dr, BITMAP *b, BITMAP *ba);
void score(void);
void pong_exit(void);
void pong_setup(void);
void pong_opening(void);
void pong_about();

/*
 * Main Function
 */

int main(int argc, char *argv[])
{
	/* Define & Initialize ball variables */
	int b_x = (320 - ball_WIDTH) / 2;
	int b_y = (200 - ball_WIDTH) / 2;
	int b_ix = 1;
	int b_iy = 1;

	allegro_init();		// Initialize Allegro
	install_keyboard();	// Initialize Keyboard functions
	install_timer();	// Initialize timer functions
	install_mouse();	// Initialize Mouse functions

	/* Start Graph mode */
	if(set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0))
		err(allegro_error);

	/* Initialize Music */
	set_volume (255, -1);
	enable_lasttrk_loop = TRUE;
	if ((music = load_mod ("music.xm")) == null)
		err("Error loading music file\n");
	chn_reserved = (music->no_chn > MAX_ALLEG_VOICE) ? 
			        MAX_ALLEG_VOICE : 
					music->no_chn;
	reserve_voices (chn_reserved, -1);
	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, null) < 0)
		err("Error initializing sound card\n");       
	if (install_mod(chn_reserved) < 0)
		err("Error allocating voices\n");
	set_mod_speed (starting_speed);
	set_mod_pitch (starting_pitch);
	play_mod (music, TRUE);
	
	/* Load & Put Background Image */
	if(!(back = load_pcx(f_back, palette)))
		err("Error reading background");
	
	/* Start opening */
	pong_opening();

	/* Create Ball Buffer */
	b_ball = create_bitmap(ball_WIDTH, ball_HEIGHT);

	/* Initializing Raq's variables */
	raqs[raq1].x = 5;
	raqs[raq2].x = 303;
	raqs[raq1].y = raq_MIN_Y;
	raqs[raq2].y = raq_MAX_Y;
	raqs[raq1].b = create_bitmap(raq_WIDTH, 200);
	raqs[raq2].b = create_bitmap(raq_WIDTH, 200);
	raqs[raq1].p = 0;
	raqs[raq2].p = 0;
	blit(back, raqs[raq1].b, raqs[raq1].x, 0, 0, 0, raq_WIDTH, 200);
	blit(back, raqs[raq2].b, raqs[raq2].x, 0, 0, 0, raq_WIDTH, 200);

	/* Load Raq image */	
	if (!(raq = load_pcx(f_raq, palette)))
		err("Error reading raq");

	/* Load Ball image */
	if(!(ball = load_pcx(f_ball, palette)))
		err("Error reading ball");
	
	/* Main Loop */
	while(TRUE) {

		/* calculate next ball position */
		ball_newpos(&b_x, &b_y, &b_ix, &b_iy);
		ball_draw(b_x, b_y, DRAW, b_ball, ball);
		
		
		/* Read keyboard */
		if(key[KEY_ESC]) pong_setup();
		if(key[KEY_Q]) raqs[raq1].y -= (raqs[raq1].y >= raq_MIN_Y) ? 1 : 0;
		if(key[KEY_A]) raqs[raq1].y += (raqs[raq1].y <= raq_MAX_Y) ? 1 : 0;
		if(key[KEY_UP]) raqs[raq2].y -= (raqs[raq2].y >= raq_MIN_Y) ? 1 : 0;
		if(key[KEY_DOWN]) raqs[raq2].y += (raqs[raq2].y <= raq_MAX_Y) ? 1 : 0;

		/* Draw raq's */
		raq_draw(raq1, DRAW, raq);
		raq_draw(raq2, DRAW, raq);
		
		/* wait */
		usleep(7000);
		
		/* Restore raq's buffer */
		raq_draw(raq1, REST, raq);
		raq_draw(raq2, REST, raq);

		/* Restore ball's buffer */
		ball_draw(b_x, b_y, REST, b_ball, ball);

	}
	
	/* Exiting */
	pong_exit();
	
} END_OF_MAIN();
/* main() END */

/* Error */
void err(char *errmsg)
{
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	allegro_message("%s", errmsg);
	exit(1);
}

void pong_exit(void) {
	/* Free images memory */
	release_bitmap(raq);
	release_bitmap(back);
	release_bitmap(ball);
	release_bitmap(b_ball);
	release_bitmap(raqs[raq1].b);
	release_bitmap(raqs[raq2].b);

	/* Stop music & release module memory */
	stop_mod();
	destroy_mod(music);

	/* Terminating... */
	remove_keyboard();
	remove_mouse();
	remove_timer();
	allegro_exit();
	printf("Created by Osvaldo Santana Neto\n\n");
	printf("Borolandia Studios\n");
	printf("http://www.borolandia.org\n\n");
	exit(0);
}

/* Calculate next ball position */
void ball_newpos(int *x, int *y, int *ix, int *iy)
{
	(*x) += (*ix);
	(*y) += (*iy);
	if (((*y) > 192) || ((*y) < 1))
		(*iy) *= -1;

	/* Detect collision */
	if (*x <= raqs[raq1].x+raq_WIDTH) {
		if ( ((*y) + ball_HEIGHT > raqs[raq1].y) &&
		     ((*y) < raqs[raq1].y + raq_HEIGHT)
		   ) {
			(*ix) *= -1;
		}
	}
	if (*x+ball_WIDTH >= raqs[raq2].x) {
		if ( ((*y) + ball_HEIGHT > raqs[raq2].y) &&
		     ((*y) < raqs[raq2].y + raq_HEIGHT)
		   ) {
			(*ix) *= -1;
		}
	}
	if (*x > 312) {
		/* Raq1 Point! */
		*x = (320 - ball_WIDTH) / 2;
		*y = (200 - ball_WIDTH) / 2;
		*ix = -1;
		raqs[raq1].p++;
		score();
	} 
	if (*x < 1) {
		/* Raq2 Point! */
		*x = (320 - ball_WIDTH) / 2;
		*y = (200 - ball_WIDTH) / 2;
		*ix = 1;
		raqs[raq2].p++;
		score();
	}
}

/* Draw Ball */
void ball_draw(int x, int y, char dr, BITMAP *b, BITMAP *ba)
{
	if(dr == REST) {
		blit(b, screen, 0, 0, x, y, b->w, b->h);
		return;
	}
	blit(screen, b, x, y, 0, 0, ba->w, ba->h);
	draw_sprite(screen, ba, x, y);
}

/* Draw raq */
void raq_draw(unsigned int rn, char dr, BITMAP *r)
{
	if (dr == REST) {
		blit(raqs[rn].b, screen,
			0, raqs[rn].y-1,
			raqs[rn].x, raqs[rn].y-1,
		       	raqs[rn].b->w, 1);
		blit(raqs[rn].b, screen,
			0, raqs[rn].y+raq_HEIGHT,
			raqs[rn].x, raqs[rn].y+raq_HEIGHT,
			raqs[rn].b->w, 1);
		return;
	}
	blit(r, screen, 0, 0, raqs[rn].x, raqs[rn].y, r->w, r->h);
}

void score(void)
{
	BITMAP *bsave;
	BITMAP *score;
	PALETTE tmp;

	score = load_pcx(f_score, tmp);
	bsave = create_bitmap(score->w, score->h);
	if (!score) err("Error reading score");
	blit(screen, bsave, 70, 50, 0, 0, bsave->w, bsave->h);
	blit(score, screen, 0, 0, 70, 50, score->w, score->h);
	textprintf_ex(screen,font,177,128,makecol(255,255,255), -1, "%1d x %1d", raqs[raq1].p, raqs[raq2].p);
	clear_keybuf();
	while(!(key[KEY_SPACE] || (mouse_b & 1) || key[KEY_ESC]));
	blit(bsave, screen, 0, 0, 70, 50, score->w, score->h);
}

void pong_setup(void) {
	int ret;
	BITMAP *bsave;
	
	DIALOG the_dialog[] = {
	  /*dialog proc  x  y   w   h  fg  bg key flags d1 d2   dp  dp2  dp3 */
	   {d_shadow_box_proc,105,25,110,150,255,0,0,0,0,0,NULL,NULL,NULL},
	   {d_button_proc,115,35,90,30,255,0,0,D_EXIT,0,0,"Continuar",NULL,NULL},
	   {d_button_proc,115,68,90,30,255,0,0,D_EXIT,0,0,"Placar",NULL,NULL},
	   {d_button_proc,115,101,90,30,255,0,0,D_EXIT,0,0,"Sobre",NULL,NULL},
	   {d_button_proc,115,134,90,30,255,0,0,D_EXIT,0,0,"Sair",NULL,NULL},
	   {NULL,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL}
	};

	clear_keybuf();
	
	bsave = create_bitmap(screen->w,screen->h);
	blit(screen,bsave, 0,0,0,0,screen->w,screen->h);
	show_mouse(screen);
	
	/* do the dialog */
	ret = do_dialog(the_dialog, -1);
	switch(ret) {
		case 1:
			break;
		case 2:
			show_mouse(NULL);
			score();
			show_mouse(screen);
			break;
		case 3:
			pong_about();
			break;
		case 4:
		       	pong_exit();
			break;
	}

	show_mouse(NULL);	
	set_palette(palette);
	blit(bsave,screen, 0,0,0,0,screen->w,screen->h);
}

void pong_opening(void) {

	/* RGB -> color mapping table. Not needed, but speeds things up */
	RGB_MAP rgb_table;

	/* lighting color mapping table */
	COLOR_MAP light_table;
   	PALETTE pal,white_palette;
	BITMAP *s;
	BITMAP *spotlight;
	BITMAP *background;
	int i, x, y=36;
   
	/* initialize white_palette */
	for(i=0; i<256; i++) {
		white_palette[i].r = 64;
		white_palette[i].g = 64;
		white_palette[i].b = 64;
	}	
		
	/* load the main screen image */
	background = load_bitmap(f_open, pal);

	/* this isn't needed, but it speeds up the color table calculations */
	create_rgb_table(&rgb_table, pal, NULL);
	rgb_map = &rgb_table;

	/* build a color lookup table for lighting effects */
	create_light_table(&light_table, pal, 0, 0, 0, NULL);

	set_palette(pal);
	s = create_bitmap(320, 200);
	spotlight = create_bitmap(128, 128);

	/* generate a spotlight image */
	clear(spotlight);
	for(i=0; i<256; i++)
	circlefill(spotlight, 64, 64, 64-i/4, i);

	/* select the lighting table */
	color_map = &light_table;

	/* display a spotlight effect */
	clear(s);
	for (x = -128; x<384; x++) {
		blit(background, s, x, y, x, y, 128, 128);
		draw_trans_sprite(s, spotlight, x, y);
		blit(s, screen, 0, 0, 0, 0, 320, 200);
		usleep(7000);
	}
	destroy_bitmap(s);
	destroy_bitmap(spotlight);
	destroy_bitmap(background);

	
	fade_from(black_palette,white_palette,1);
	blit(back, screen, 0, 0, 0, 0, back->w, back->h);
	fade_from(white_palette,palette,1);
	set_palette(palette);
}

void pong_about(void)
{
	DIALOG the_dialog[] = {
	  /*dialog proc  x  y   w   h  fg  bg key flags d1 d2   dp  dp2  dp3 */
	   {d_shadow_box_proc,20,25,280,150,255,0,0,0,0,0,NULL,NULL,NULL},
	   {d_ctext_proc,160,35,90,30,255,0,0,0,0,0,"Pong!",NULL,NULL},
	   {d_ctext_proc,160,45,90,30,255,0,0,0,0,0,"version 0.1",NULL,NULL},
	   {d_ctext_proc,160,55,90,30,255,0,0,0,0,0,"Created by Osvaldo Santana Neto",NULL,NULL},
	   {d_ctext_proc,160,65,90,30,255,0,0,0,0,0,"Borolandia Studio",NULL,NULL},
	   {d_ctext_proc,160,75,90,30,255,0,0,0,0,0,"http://www.borolandia.org",NULL,NULL},
	   {d_button_proc,115,134,90,30,255,0,0,D_EXIT,0,0,"OK",NULL,NULL},
	   {NULL,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL}
	};
	do_dialog(the_dialog,-1);
	
}

