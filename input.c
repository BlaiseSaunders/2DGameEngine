#include "main.h"


int get_input()
{
	/*
	 * Get Input
	 */
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				cleanup();
				return 1;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_e:
						keyboard.e = PRESSED;
						break;
					case SDLK_TAB:
						keyboard.tab = PRESSED;
						break;
					case SDLK_w:
						keyboard.w = PRESSED;
						break;
					case SDLK_a:
						keyboard.a = PRESSED;
						break;
					case SDLK_s:
						keyboard.s = PRESSED;
						break;
					case SDLK_d:
						keyboard.d = PRESSED;
						break;
					case SDLK_l:
						keyboard.l = PRESSED;
						break;
					case SDLK_UP:
						keyboard.up = PRESSED;
						break;
					case SDLK_DOWN:
						keyboard.down = PRESSED;
						break;
					case SDLK_LEFT:
						keyboard.left = PRESSED;
						break;
					case SDLK_RIGHT:
						keyboard.right = PRESSED;
						break;
					case SDLK_SPACE:
						keyboard.space = PRESSED;
						break;
					case SDLK_1:
						keyboard.one = PRESSED;
						break;
					case SDLK_2:
						keyboard.two = PRESSED;
						break;
					case SDLK_3:
						keyboard.three = PRESSED;
						break;
					case SDLK_4:
						keyboard.four = PRESSED;
						break;
					default:
						break;
				}
				break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
				case SDLK_e:
					keyboard.e = RELEASED;
					break;
				case SDLK_TAB:
					keyboard.tab = RELEASED;
					break;
				case SDLK_w:
					keyboard.w = RELEASED;
					break;
				case SDLK_a:
					keyboard.a = RELEASED;
					break;
				case SDLK_s:
					keyboard.s = RELEASED;
					break;
				case SDLK_d:
					keyboard.d = RELEASED;
					break;
				case SDLK_l:
					keyboard.l = RELEASED;
					break;
				case SDLK_UP:
					keyboard.up = RELEASED;
					break;
				case SDLK_DOWN:
					keyboard.down = RELEASED;
					break;
				case SDLK_LEFT:
					keyboard.left = RELEASED;
					break;
				case SDLK_RIGHT:
					keyboard.right = RELEASED;
					break;
				case SDLK_SPACE:
					keyboard.space = RELEASED;
					break;
				case SDLK_1:
					keyboard.one = RELEASED;
					break;
				case SDLK_2:
					keyboard.two = RELEASED;
					break;
				case SDLK_3:
					keyboard.three = RELEASED;
					break;
				case SDLK_4:
					keyboard.four = RELEASED;
					break;
				case SDLK_q:
					cleanup();
					return 1;
				default:
					break;
			}
			break;
			case SDL_CONTROLLERBUTTONDOWN:
				switch (event.cbutton.button)
				{
					case SDL_CONTROLLER_BUTTON_A:
						keyboard.down = PRESSED;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
						keyboard.d = PRESSED;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
						keyboard.a = PRESSED;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						keyboard.w = PRESSED;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						keyboard.s = PRESSED;
						break;
				}
				break;
			case SDL_CONTROLLERBUTTONUP:
				switch (event.cbutton.button)
				{
					case SDL_CONTROLLER_BUTTON_A:
						keyboard.down = RELEASED;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
						keyboard.d = RELEASED;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
						keyboard.a = RELEASED;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_UP:
						keyboard.w = RELEASED;
						break;
					case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						keyboard.s = RELEASED;
						break;
				}
				break;
			/*case SDL_JOYAXISMOTION:
				if ((event.jaxis.value < -3200) || (event.jaxis.value > 3200))
				{
					 x axis
					if (event.jaxis.axis == 0)
					{
						keyboard.w = PRESSED;
					}

					 y axis
					if (event.jaxis.axis == 1)
					{
						keyboard.d = PRESSED;
					}
				}
				break;
		*/}
	}
	/*fprintf(fp, "%d %d: %d\n", event.jaxis.axis, event.jaxis.which, event.jaxis.value);*/
	return 0;
}
