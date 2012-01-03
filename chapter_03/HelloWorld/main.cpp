/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* This example SDL code will load up a bitmap image of the SDL logo, and just display
* it in a window for you. It's very bare bones.
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: The goal of this small sample is to just act as our "Hello World". Basically display
* the SDL logo. W00t.
*/
#include <SDL.h>

int main(int argc, char* argv[])
{
	//initialize SDL and the video subsystem
	if(SDL_Init( SDL_INIT_VIDEO ) < 0)
		return -1;

	//create an SDL_Surface object which represents the
	//game window  
	SDL_Surface* screen = SDL_SetVideoMode(640, 480, 0, 0);

	//signal SDL to change the text of the main window
	//to "Hello World"
	SDL_WM_SetCaption("Hello World", "Hello World");


	//load the SDL logo bitmap to a temporary surface
	SDL_Surface* temp = SDL_LoadBMP("data\\textures\\sdl_logo.bmp");


	//create the working SDL_Surface which matches the 
	//display format of the temporary surface
	SDL_Surface* bg = SDL_DisplayFormat(temp);

	//free the memory allocated to the temporary SDL_Surface
	SDL_FreeSurface(temp);

	SDL_Event event;
	bool quit = false;

	//This is the main message loop of the game
	while(!quit)
	{
		//check the message queue for an event
		if (SDL_PollEvent(&event))
		{
			//if an event was found
			switch (event.type)
			{
				//check to see if the window was closed via the "x button"
			case SDL_QUIT:
				//set the quit flag to true
				quit = true;
				break;

				//check the keyboard to see if the ESC key was pressed
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					//set our quit flag to true
					quit = true;
					break;
				}
				break;
			}
		}

		//draw the background sprite
		SDL_BlitSurface(bg, NULL, screen, NULL);

		//update the current window
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

	//free the allocated memory for the background surface
	SDL_FreeSurface(bg);

	//quit SDL and allow it to clean up everything
	SDL_Quit();

	//return control to Windows with no errors
	return 0;
}
