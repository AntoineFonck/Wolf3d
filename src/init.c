/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 16:53:33 by sluetzen          #+#    #+#             */
/*   Updated: 2019/10/18 12:46:10 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "wolf3d.h"

void	find_player_pos(t_player *player, int map[MAX_MAP][MAX_MAP],
			int map_width, int map_height)
{
	int i;
	int j;

	i = 0;
	player->x = 0;
	player->y = 0;
	while (i < map_height)
	{
		j = 0;
		while (j < map_width)
		{
			if (map[i][j] == 'X')
			{
				player->x = j + 0.000001;
				player->y = i + 0.000001;
				map[i][j] = 0;
				return ;
			}
			j++;
		}
		i++;
	}
	ft_dprintf(STDERR_FILENO, "no suitable starting position found for player, \
		exiting...\n");
	exit(EXIT_FAILURE);
}

int		init_sdl(SDL_Window **win, SDL_Surface **surf)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		ft_dprintf(STDERR_FILENO, "SDL_Init Error: %{r}s\n", SDL_GetError());
		return (EXIT_FAILURE);
	}
	if ((*win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, \
		SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN)) \
		== NULL)
	{
		ft_dprintf(STDERR_FILENO, "SDL_CreateWindow Error: %{r}s\n", \
			SDL_GetError());
		return (EXIT_FAILURE);
	}
	if ((*surf = SDL_GetWindowSurface(*win)) == NULL)
	{
		ft_dprintf(STDERR_FILENO, "SDL_GetWindowSurface Error: %{r}s\n", \
			SDL_GetError());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int		init_ttf(t_ttf *ttf)
{
	if (TTF_Init() != 0)
	{
		ft_dprintf(STDERR_FILENO, "TTF_Init Error: %{r}s\n", TTF_GetError());
		return (EXIT_FAILURE);
	}
	if ((ttf->font = TTF_OpenFont("/Library/Fonts/Arial.ttf", 150)) == NULL)
	{
		ft_dprintf(STDERR_FILENO, "TTF_OpenFont Error: %{r}s\n",
			TTF_GetError());
		return (EXIT_FAILURE);
	}
	ttf->color.r = 0;
	ttf->color.g = 0;
	ttf->color.b = 0;
	ttf->color.a = 100;
	ttf->rect.x = WIN_WIDTH / 200;
	ttf->rect.y = WIN_HEIGHT / 200;
	ttf->rect.w = WIN_WIDTH / 12;
	ttf->rect.h = WIN_HEIGHT / 12;
	return (EXIT_SUCCESS);
}

void	init_wolf(t_wolf *wolf, char *title)
{
	init_sdl_struct(&(wolf->sdl));
	init_ttf_struct(&(wolf->ttf));
	init_data_struct(&(wolf->data), title);
	init_player_struct(&(wolf->player), *(wolf->data.map_ptr),
		wolf->data.map_width, wolf->data.map_height);
	init_raycast_struct(&(wolf->raycast), wolf->player.x, wolf->player.y);
}
