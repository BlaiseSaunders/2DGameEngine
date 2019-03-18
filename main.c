#include "main.h"


int main(int argc, char **argv)
{

	/*
	* Declarations
	*/
	int i, j, k, l;
	int w, h;
	unsigned int frame_limit, ticks;
	player *players, *cur;


	SDL_Rect dest;

	if (argc > 1)
		printf("Usage: %s\n", argv[0]);

	/*
	 * Initialisation
	 */


	/* Setup SDL */
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER|
	             SDL_INIT_AUDIO) < 0)
	{
		printf("Failed to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	if (TTF_Init() < 0)
		printf("TTF Fudged up\n");

	font = TTF_OpenFont("font/DroidSansMono.ttf", 48);
	if (font == NULL)
		printf("Managed to screw up opening the font: %s\n", TTF_GetError());
	TTF_SetFontHinting(font, TTF_HINTING_MONO);
	TTF_SetFontKerning(font, 0);

	if ((screen = SDL_CreateWindow("Game",
		                       30,
		                       50,
		                       SCREEN_WIDTH, SCREEN_HEIGHT,
		                       SDL_WINDOW_OPENGL/*|SDL_WINDOW_FULLSCREEN_DESKTOP*/)) == NULL)
	{
		printf("Failed to create SDL window: %s\n", SDL_GetError());
		return 1;
	}
	if ((renderer = SDL_CreateRenderer(screen, -1, 0)) == NULL)
	{
		printf("Failed to create SDL screen: %s\n", SDL_GetError());
		return 1;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	atexit(cleanup);




	/* Init Misc */
	srand(time(NULL));


	/* Setup update pointer array */
	updates[0] = gen_update;
	updates[1] = dumb_update;

	/* Setup activate pointer array */
	activates[0] = dummy_activate;
	activates[1] = side_activate;


	/* Load textures */
	printf("Loading textures:\n");
	load_textures();
	printf("Succesfully loaded textures.\n");


	/* Setup some UI elements */
	horiz_bar.texture = textures[63];
	horiz_bar.off.x = 80;
	horiz_bar.off.y = 0;
	horiz_bar.off.w = 8;
	horiz_bar.off.h = 6;

	vert_bar.texture = textures[63];
	vert_bar.off.x = 64;
	vert_bar.off.y = 14;
	vert_bar.off.w = 6;
	vert_bar.off.h = 8;


	lu_corn.texture = textures[63];
	lu_corn.off.x = 64;
	lu_corn.off.y = 0;
	lu_corn.off.w = 8;
	lu_corn.off.h = 8;
	lb_corn.texture = textures[63];
	lb_corn.off.x = 64;
	lb_corn.off.y = 64-8;
	lb_corn.off.w = 8;
	lb_corn.off.h = 8;
	rb_corn.texture = textures[63];
	rb_corn.off.x = 128-8;
	rb_corn.off.y = 64-8;
	rb_corn.off.w = 8;
	rb_corn.off.h = 8;
	ru_corn.texture = textures[63];
	ru_corn.off.x = 128-8;
	ru_corn.off.y = 0;
	ru_corn.off.w = 8;
	ru_corn.off.h = 8;




	for (i = 0; i < MAXX; i++)
		for (j = 0; j < MAXY; j++)
			map[i][j].stander = NULL;
	if (main_menu(&players))
		return EXIT_FAILURE;

	printf("Starting up.\n");
	fflush(stdout);



	frame_limit = SDL_GetTicks() + 16;
	while (1)
	{
		/*
		* Check for a change in screen size
		*/
		SDL_GetWindowSize(screen, &w, &h);


		/*
		 * Get Input
		 */
		if (get_input())
			return EXIT_SUCCESS;



		/*
		 * Run game logic
		 */
		if (!overlay_logic)
		{
			/*
			 * Handle Input
			 */


			if (!players->walking)
			{
				/* Handle arrow keys first */
				if (keyboard.up)
				{
					players->dir = UP;
					players->walking = true;
				}
				if (keyboard.down)
				{
					players->dir = DOWN;
					players->walking = true;
				}
				if (keyboard.left)
				{
					players->dir = LEFT;
					players->walking = true;
				}
				if (keyboard.right)
				{
					players->dir = RIGHT;
					players->walking = true;
				}
				if (keyboard.space)
				{
					/*map[players->x][players->y].offset.x += 64;*/

					switch (players->dir)
					{
						case UP:
							if (map[players->x][players->y-1].stander != NULL)
								map[players->x][players->y-1]
								.stander->activate(map[players->x][players->y-1].stander->dat_file);
							break;
						case DOWN:
							if (map[players->x][players->y+1].stander != NULL)
								map[players->x][players->y+1]
								.stander->activate(map[players->x][players->y+1].stander->dat_file);
							break;
						case LEFT:
							if (map[players->x-1][players->y].stander != NULL)
								map[players->x-1][players->y]
								.stander->activate(map[players->x-1][players->y].stander->dat_file);
							break;
						case RIGHT:
							if (map[players->x+1][players->y].stander != NULL)
								map[players->x+1][players->y]
								.stander->activate(map[players->x+1][players->y].stander->dat_file);
							break;
					}

				}
			}





			/*
			 * Update erryone
			 */
			for (cur = players; cur->next != NULL; cur = cur->next)
				cur->update(cur);

		}
		/* Run overlay logic */
		else
			overlay_logic();




		/*
		 * Draw
		 */
		SDL_SetRenderDrawColor(renderer, 23, 24, 26, 255);
		SDL_RenderClear(renderer);


		/* Draw everything before we draw the 'over'lay */


		/* Setup the camera */
		cam.x = (players->x-((w/TILE_SIZE)/2));
		cam.y = (players->y-((h/TILE_SIZE)/2));
		cam.h = (h/TILE_SIZE)+3;
		cam.w = (w/TILE_SIZE)+3;


		if (cam.x < 0)
			cam.x = 0;
		else if (cam.x > 0)
			cam.x--;
		if (cam.y < 0)
			cam.y = 0;
		else if (cam.y > 0)
			cam.y--;

		/*printf("cam.x: %d\n", cam.x);
		printf("cam.y: %d\n", cam.y);
		printf("player.x: %d\n", players->x);
		printf("player.y: %d\n", players->y);*/


		/* Draw tiles */
		dest.h = TILE_SIZE;
		dest.w = TILE_SIZE;
		for (i = cam.x, k = ((cam.x > 0)? -1:0); i < cam.x+cam.w; i++, k++)
			for (j = cam.y, l = ((cam.y > 0)? -1:0); j < cam.y+cam.h; j++, l++)
			{
				dest.x = (k*TILE_SIZE);
				dest.y = (l*TILE_SIZE);

				if (cam.x)
					dest.x -= players->xoff;
				if (cam.y)
					dest.y -= players->yoff;


				SDL_RenderCopy(renderer, map[i][j].texture,
				               &map[i][j].offset, &dest);

			}



		for (i = cam.x, k = 0; i < cam.x+cam.w; i++, k++)
			for (j = cam.y, l = 0; j < cam.y+cam.h; j++, l++)
				if (map[i][j].stander)
				{
					dest.x = ((k-1)*TILE_SIZE);
					dest.y = ((l-1)*TILE_SIZE);

					if (map[i][j].stander != players)
					{
						dest.x += map[i][j].stander->xoff;
						dest.y += map[i][j].stander->yoff;

						/* Make them move with the camera */
						dest.x -= players->xoff;
						dest.y -= players->yoff;
					}


					SDL_RenderCopy(renderer, map[i][j].stander->texture,
				        	       &map[i][j].stander->offset, &dest);
				}



		/* Draw the current overlay */
		if (overlay_draw)
			overlay_draw();



		SDL_RenderPresent(renderer);



		/*
		* Sleep
		*/
		if (frame_limit > (ticks = SDL_GetTicks()) + FPS)
			SDL_Delay(FPS);
		else if (frame_limit > ticks)
			SDL_Delay(frame_limit - ticks);
		frame_limit = SDL_GetTicks() + FPS;



	}

	return EXIT_SUCCESS;
}

void cleanup()
{
	SDL_Quit();
}

void set_colour(int colour)
{
	switch (colour)
	{
		case -5:
			SDL_SetRenderDrawColor(renderer, 146, 172, 88,
				255); /*blue*/
			break;
		case 0:
			SDL_SetRenderDrawColor(renderer, 207, 153, 17,
				255);/*yellow*/
			break;
		case 1:
			SDL_SetRenderDrawColor(renderer, 187, 119, 164,
				255);/*magenta*/
			break;
		case 2:
		case -2:
			SDL_SetRenderDrawColor(renderer, 226, 31, 62,
				255);/*red*/
			break;
		case 3:
		case -3:
			SDL_SetRenderDrawColor(renderer, 61, 160, 129,
				255);/*blue*/
			break;
		default:
			SDL_SetRenderDrawColor(renderer, 160, 157, 138,
				255);/*off white*/
			break;
	}
}


void load_textures()
{
	int i;
	char buf[128];


	for (i = 0; i < TEXTURE_COUNT; i++)
	{
		sprintf(buf, "%s%d.png", TEXTURE_DIR, i);
		if (access(buf, F_OK) != -1)
		{
			textures[i] = IMG_LoadTexture(renderer, buf);

			if (textures[i] == NULL)
				printf("Failed to load texutre %s: %s\n", buf, SDL_GetError());
			else
				printf("Succsesfully loaded %s to texture slot %d\n", buf, i);


		}
		else
			textures[i] = NULL;
	}

}


void load_old()
{
	int i;
	char buf[128];
	SDL_Surface *tmp;


	for (i = 0; i < TEXTURE_COUNT; i++)
	{
		sprintf(buf, "%s%d.bmp", TEXTURE_DIR, i);
		if (access(buf, F_OK) != -1)
		{
			tmp = SDL_LoadBMP(buf);
			textures[i] = SDL_CreateTextureFromSurface(renderer, tmp);
			SDL_FreeSurface(tmp);

			if (textures[i] == NULL)
				printf("Failed to load texutre %s: %s\n", buf, SDL_GetError());
			else
				printf("Succsesfully loaded %s to texture slot %d\n", buf, i);


		}
		else
			textures[i] = NULL;
	}

}
