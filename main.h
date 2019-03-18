#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <fcntl.h>

#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>
#include <errno.h>



#ifndef MAIN_HEAD
#define MAIN_HEAD


#define _XOPEN_SOURCE 600

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define FPS 64
#define TEXT_SPEED 2


#define MAXX 128
#define MAXY 128


#define TEXTURE_DIR "textures/"
#define DATA_DIR "data/"

#define TILE_SIZE 32

#define TEXTURE_COUNT 64

#define WALK_FRAMES 3


typedef enum
{
	false,
	true
} bool;


enum tile_type
{
	GENERIC,
	EVENT
};

enum direction
{
	DOWN,
	LEFT,
	RIGHT,
	UP
};


typedef struct player
{
	int x;
	int y;
	int xoff;
	int yoff;
	enum direction dir;
	bool walking;
	int walk_frame;
	SDL_Texture *texture;
	SDL_Rect offset;
	SDL_Rect base_offset;

	char dat_file[256];
	void (*activate)(char *);
	void (*update)(struct player *);

	struct player *next;

} player;


typedef struct
{

	bool walkable;
	enum tile_type type;
	char *params;
	SDL_Texture *texture;
	SDL_Rect offset;
	void (*activate)(char *);
	player *stander;

} tile;


typedef struct ui_element
{
	SDL_Texture *texture;
	SDL_Rect off;

} ui_element;


typedef struct
{
	int x;
	int y;
	int w;
	int h;
} camera;

typedef enum
{
	RELEASED,
	PRESSED
} key_state;

struct keyboard_s
{
	key_state w, a, s, d, space,
		left, right, up, down,
		one, two, three, four,
		l, m, tab, e;
};


struct decision
{
	int count;

	char dec[8][128];
};




int main_menu(player **players);
int load_area(char *file, player **players);
void cleanup();
int get_input();
void load_textures();
void gen_update(player *);
void dumb_update(player *);
void dummy_activate(char *);
void side_activate(char *);
int npc_chat_draw();
int npc_chat_logic();



void (*updates[128])(player*);
void (*activates[128])(char*);

struct keyboard_s keyboard;
camera cam;
tile map[MAXX][MAXY];
SDL_Window *screen;
SDL_Renderer *renderer;
TTF_Font *font;
SDL_Event event;


bool next;


int (*overlay_logic)();
int (*overlay_draw)();


char *active_dat;

ui_element horiz_bar;
ui_element vert_bar;
ui_element lu_corn;
ui_element lb_corn;
ui_element ru_corn;
ui_element rb_corn;


SDL_Texture *textures[TEXTURE_COUNT];




#endif
