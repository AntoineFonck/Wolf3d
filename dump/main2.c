#include "libft.h"

#include "wolf3d.h"

void	freeSDL(SDL_Window **win, SDL_Surface **surf)
{
	SDL_FreeSurface(*surf);
	*surf = NULL;
	SDL_DestroyWindow(*win);
	*win = NULL;
	SDL_Quit();
}

int	loadMedia(SDL_Surface **img, char *path)
{
	*img = SDL_LoadBMP(path);
	if (*img == NULL)
	{
		ft_dprintf(2, "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	initSDL(SDL_Window **win, SDL_Surface **surf)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		ft_dprintf(2, "SDL_Init Error: %{r}s\n", SDL_GetError());
		return (EXIT_FAILURE);
	}
	*win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, \
			SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	if (*win == NULL)
	{
		ft_dprintf(2, "SDL_CreateWindow Error: %{r}s\n", SDL_GetError());
		return (EXIT_FAILURE);
	}
	*surf = SDL_GetWindowSurface(*win);
	return (EXIT_SUCCESS);
}

void	initWolf(t_wolf *wolf)
{
	wolf->sdl.win = NULL;
	wolf->sdl.surf = NULL;
	wolf->sdl.img = NULL;
	wolf->quit = 0;
}

int main()
{
	t_wolf	wolf;
	char *path_bmp = "/Users/afonck/Desktop/Wolf3D/src/test.bmp";

	initWolf(&wolf);
	if (initSDL(&(wolf.sdl.win), &(wolf.sdl.surf)) != EXIT_SUCCESS)
	{
		freeSDL(&(wolf.sdl.win), &(wolf.sdl.surf));
		return (EXIT_FAILURE);
	}
	if (loadMedia(&(wolf.sdl.img), path_bmp) != EXIT_SUCCESS)
	{
		freeSDL(&(wolf.sdl.win), &(wolf.sdl.surf));
		return (EXIT_FAILURE);
	}
	while (!wolf.quit)
	{
		while (SDL_PollEvent(&(wolf.sdl.event)) != 0)
		{
			if (wolf.sdl.event.type == SDL_QUIT)
				wolf.quit = 1;
		}
		SDL_BlitSurface(wolf.sdl.img, NULL, wolf.sdl.surf, NULL );
		SDL_UpdateWindowSurface(wolf.sdl.win);
	}
	freeSDL(&(wolf.sdl.win), &(wolf.sdl.surf));
	return (EXIT_SUCCESS);
}
