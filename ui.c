#include "main.h"


#define BUF_SIZE 1024

void draw_chat_window(char *text)
{
	int i, w, h;
	SDL_Rect dest;
	SDL_Surface *text_surf;
	SDL_Texture *texture;
	SDL_Colour white;
	static int frame = 0;

	(void)frame;


	SDL_GetWindowSize(screen, &w, &h);

	/* Draw the horizontal bars */
	for (i = 0; i < (w/horiz_bar.off.w); i++)
	{
		dest.x = (i*horiz_bar.off.w)+w*.10;
		dest.y = h - (h*.30) - horiz_bar.off.h;
		dest.w = horiz_bar.off.w;
		dest.h = horiz_bar.off.h;

		/* Don't touch this */
		if (dest.x+dest.w > (w*.10)+(w - (2*(w*.10))))
			dest.w = (w*.10)+(w - (2*(w*.10)))-dest.x;

		SDL_RenderCopy(renderer, horiz_bar.texture,
	        	       &horiz_bar.off, &dest);
	}
	for (i = 0; i < (w/horiz_bar.off.w); i++)
	{
		dest.x = (i*horiz_bar.off.w)+w*.10;
		dest.y = h - (h*.30) + h*.25;
		dest.w = horiz_bar.off.w;
		dest.h = horiz_bar.off.h;

		/* Don't touch this */
		if (dest.x+dest.w > (w*.10)+(w - (2*(w*.10))))
			dest.w = (w*.10)+(w - (2*(w*.10)))-dest.x;

		SDL_RenderCopy(renderer, horiz_bar.texture,
	        	       &horiz_bar.off, &dest);
	}

	/* Draw the vertical bars */
	for (i = 0; i < h/vert_bar.off.h; i++)
	{
		dest.x = w*.10 - vert_bar.off.w;
		dest.y = (i*vert_bar.off.h) + h - (h*.30);
		dest.w = vert_bar.off.w;
		dest.h = vert_bar.off.h;

		if (dest.y+dest.h > h - (h*.30) + (h*.25))
			dest.h =  h - (h*.30) + (h*.25) - dest.y;


		SDL_RenderCopy(renderer, vert_bar.texture,
	        	       &vert_bar.off, &dest);
	}
	for (i = 0; i < h/vert_bar.off.h; i++)
	{
		dest.x = w*.10 + w - (2*(w*.10));
		dest.y = (i*vert_bar.off.h) + h - (h*.30);
		dest.w = vert_bar.off.w;
		dest.h = vert_bar.off.h;

		if (dest.y+dest.h > h - (h*.30) + (h*.25))
			dest.h =  h - (h*.30) + (h*.25) - dest.y;


		SDL_RenderCopy(renderer, vert_bar.texture,
	        	       &vert_bar.off, &dest);
	}


	dest.x = w*.10 - lu_corn.off.w +2;
	dest.y = h - (h*.30) - lu_corn.off.h + 2;
	dest.w = lu_corn.off.w;
	dest.h = lu_corn.off.h;
	SDL_RenderCopy(renderer, lu_corn.texture,
        	       &lu_corn.off, &dest);

	dest.x = w*.10 - lb_corn.off.w +2;
	dest.y = h - (h*.30) + h*.25 - 2;
	dest.w = lb_corn.off.w;
	dest.h = lb_corn.off.h;
	SDL_RenderCopy(renderer, lb_corn.texture,
        	       &lb_corn.off, &dest);

	dest.x = w*.10 + w - (2*(w*.10)) - 2;
	dest.y = h - (h*.30) - ru_corn.off.h + 2;
	dest.w = ru_corn.off.w;
	dest.h = ru_corn.off.h;
	SDL_RenderCopy(renderer, ru_corn.texture,
        	       &ru_corn.off, &dest);

	dest.x = w*.10 + w-(2*(w*.10)) - 2;
	dest.y = h - (h*.30) + h*.25 - 2;
	dest.w = rb_corn.off.w;
	dest.h = rb_corn.off.h;
	SDL_RenderCopy(renderer, rb_corn.texture,
        	       &rb_corn.off, &dest);



	/* Fill the box */
	dest.x = w*.10;
	dest.y = h - (h*.30);
	dest.w = w - (2*(w*.10));
	dest.h = h*.25;

	SDL_SetRenderDrawColor(renderer, 160, 157, 138,
                               180);/*off white*/

	SDL_RenderFillRect(renderer, &dest);



	white.r = 255;
	white.g = 255;
	white.b = 255;

	text_surf = TTF_RenderText_Solid(font, text, white);
	texture = SDL_CreateTextureFromSurface(renderer, text_surf);


	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	dest.x += 10;
	dest.y += 10;
	dest.w = strlen(text)*12;
	dest.h = 24;
	SDL_RenderCopy(renderer, texture, NULL, &dest);


}


void end_overlay()
{
	overlay_draw = NULL;
	overlay_logic = NULL;
}

int npc_chat_draw()
{
	char buf[BUF_SIZE];
	static char message[BUF_SIZE];
	static unsigned int frame = 0;
	static FILE *fp = NULL;



	if (!fp)
	{
		fp = fopen(active_dat, "r");
		fgets(message, BUF_SIZE, fp);
		message[strlen(message)-1] = '\0';
	}


	strncpy(buf, message, frame);
	buf[frame] = '\0';

	draw_chat_window(buf);

	if (frame < strlen(message)+8)
		frame += TEXT_SPEED;
	else if (next && frame)
	{
		next = false;
		frame = 0;
		if (fgets(message, BUF_SIZE, fp) == NULL)
		{
			fp = NULL;
			end_overlay();
		}
		message[strlen(message)-1] = '\0';
	}


	return 0;
}


int npc_chat_logic()
{
	if (keyboard.w)
		end_overlay();
	if (keyboard.s)
		next = true;


	return 0;
}
