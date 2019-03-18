#include "main.h"


#define BUF_SIZE 8192

int main_menu(player **players)
{

	if (load_area("map.dat", players))
		return 1;


	printf("Done with main menu.\n");
	fflush(stdout);

	return 0;
}

void dummy_activate(char *not_used)
{
	(void)not_used;

	printf("Ayla Mao\n");

	return;
}

void side_activate(char *dat_file)
{

	active_dat = dat_file;

	overlay_draw = npc_chat_draw;
	overlay_logic = npc_chat_logic;

	printf("Entering overlay\n");

	return;
}

void dumb_update(player *cur)
{
	if (!(rand()%50))
		cur->dir = (rand()%4);
	if (!(rand()%25))
		cur->walking = true;
	gen_update(cur);
}


void gen_update(player *cur)
{

	/* Update their sprite */
	cur->offset.x = cur->base_offset.x+(TILE_SIZE*cur->walk_frame);
	cur->offset.y = cur->base_offset.y+(TILE_SIZE*cur->dir);

	/* If they're starting to walk, check if they can */
	if (cur->walking && !cur->walk_frame)
		switch (cur->dir)
		{
			case UP:
				if (map[cur->x][cur->y-1].stander != NULL || !map[cur->x][cur->y-1].walkable)
					cur->walking = false;
				break;
			case DOWN:
				if (map[cur->x][cur->y+1].stander != NULL || !map[cur->x][cur->y+1].walkable)
					cur->walking = false;
				break;
			case LEFT:
				if (map[cur->x-1][cur->y].stander != NULL || !map[cur->x-1][cur->y].walkable)
					cur->walking = false;
				break;
			case RIGHT:
				if (map[cur->x+1][cur->y].stander != NULL || !map[cur->x+1][cur->y].walkable)
					cur->walking = false;
				break;
		}



	if (cur->walking)
	{

		switch (cur->dir)
		{
			case UP:
				cur->yoff -= (TILE_SIZE/WALK_FRAMES)/1;
				break;
			case DOWN:
				cur->yoff += (TILE_SIZE/WALK_FRAMES)/1;
				break;
			case LEFT:
				cur->xoff -= (TILE_SIZE/WALK_FRAMES)/1;
				break;
			case RIGHT:
				cur->xoff += (TILE_SIZE/WALK_FRAMES)/1;
				break;

		}
		cur->walk_frame++;
		if (cur->walk_frame > WALK_FRAMES-1)
		{
			cur->walk_frame = 0;
			cur->xoff = 0;
			cur->yoff = 0;
			cur->walking = false;


			map[cur->x][cur->y].stander = NULL;

			switch (cur->dir)
			{
				case UP:
					cur->y--;
					break;
				case DOWN:
					cur->y++;
					break;
				case LEFT:
					cur->x--;
					break;
				case RIGHT:
					cur->x++;
					break;

			}

			map[cur->x][cur->y].stander = cur;
		}
	}
}

player* init_player()
{
	player *cur;

	cur = malloc(sizeof (player));
	cur->xoff = 0;
	cur->yoff = 0;
	cur->dir = 0;
	cur->walking = false;
	cur->walk_frame = 0;
	cur->next = NULL;


	return cur;
}

int load_area(char *file, player **players)
{
	int x, y;
	char buf[BUF_SIZE], *buf2, *saveptr1, *saveptr2, *propbuf;
	player *cur;
	FILE *fp;


	if ((fp = fopen(file, "r")) == NULL)
	{
		printf("Failed to load map \"%s\"\n", file);
		return 1;
	}


	/* Get 'player' data */
	fgets(buf, BUF_SIZE, fp);

	*players = cur = init_player();
	buf2 = strtok_r(buf, "|", &saveptr1);
	do
	{
		if (strlen(buf2) < 5)
			continue;
		/*printf("Working through %s\n", buf2);*/
		propbuf = strtok_r(buf2, ",", &saveptr2);
		cur->x = atoi(propbuf);
		propbuf = strtok_r(NULL, ",", &saveptr2);
		cur->y = atoi(propbuf);
		propbuf = strtok_r(NULL, ",", &saveptr2);
		cur->texture = textures[atoi(propbuf)];

		propbuf = strtok_r(NULL, ",", &saveptr2);
		cur->base_offset.x = atoi(propbuf);
		propbuf = strtok_r(NULL, ",", &saveptr2);
		cur->base_offset.y = atoi(propbuf);

		cur->offset.w = TILE_SIZE;
		cur->offset.h = TILE_SIZE;



		propbuf = strtok_r(NULL, ",", &saveptr2);
		cur->activate = (*activates[atoi(propbuf)]);


		propbuf = strtok_r(NULL, ",", &saveptr2);
		cur->update = (*updates[atoi(propbuf)]);



		propbuf = strtok_r(NULL, ",", &saveptr2);
		sprintf(cur->dat_file, "%s%s", DATA_DIR, propbuf);

		cur->next = init_player();


		map[cur->x][cur->y].stander = cur;


		cur = cur->next;


	} while ((buf2 = strtok_r(NULL, "|", &saveptr1)) != NULL);


	for (y = 0; fgets(buf, BUF_SIZE, fp) != NULL; y++)
	{
		x = 0;
		buf2 = strtok_r(buf, "|", &saveptr1);
		do
		{
			if (strlen(buf2) < 5)
				continue;
			/*printf("Working through tile: %s\n", buf2);*/
			propbuf = strtok_r(buf2, ",", &saveptr2);
			map[x][y].walkable = atoi(buf2);
			propbuf = strtok_r(NULL, ",", &saveptr2);
			map[x][y].type = atoi(buf2);
			propbuf = strtok_r(NULL, ",", &saveptr2);
			map[x][y].texture = textures[atoi(buf2)];


			propbuf = strtok_r(NULL, ",", &saveptr2);
			map[x][y].offset.x = atoi(propbuf);
			propbuf = strtok_r(NULL, ",", &saveptr2);
			map[x][y].offset.y = atoi(propbuf);

			map[x][y].offset.w = TILE_SIZE;
			map[x][y].offset.h = TILE_SIZE;
			/*printf("Done with %d, %d\n", x, y);*/

			map[x][y].activate = dummy_activate;

			x++;

		} while ((buf2 = strtok_r(NULL, "|", &saveptr1)) != NULL);

	}

	fclose(fp);
	return 0;

}
