/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sluetzen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 16:53:33 by sluetzen          #+#    #+#             */
/*   Updated: 2019/10/14 16:53:56 by sluetzen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "wolf3d.h"

int	init_SDL(SDL_Window **win, SDL_Renderer **ren, SDL_Texture **tex)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		ft_dprintf(STDERR_FILENO, "SDL_Init Error: %{r}s\n", SDL_GetError());
		return (EXIT_FAILURE);
	}
	if ((*win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, \
			SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN)) == NULL)
	{
		ft_dprintf(STDERR_FILENO, "SDL_CreateWindow Error: %{r}s\n", SDL_GetError());
		return (EXIT_FAILURE);
	}
	if ((*ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_SOFTWARE)) == NULL)
	{
		ft_dprintf(STDERR_FILENO, "SDL_CreateRenderer Error: %{r}s\n", SDL_GetError());
		return (EXIT_FAILURE);
	}
	if ((*tex = SDL_CreateTexture(*ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING/*STATIC*/,
			WIN_WIDTH, WIN_HEIGHT)) == NULL)
	{
		ft_dprintf(STDERR_FILENO, "SDL_CreateTexture Error: %{r}s\n", SDL_GetError());
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	init_TTF(t_ttf *ttf)
{
	if (TTF_Init() != 0)
	{
		ft_dprintf(STDERR_FILENO, "TTF_Init Error: %{r}s\n", TTF_GetError());
		return (EXIT_FAILURE);
	}
	if ((ttf->font = TTF_OpenFont("/Library/Fonts/Arial.ttf", 24)) == NULL)
	{
		ft_dprintf(STDERR_FILENO, "TTF_OpenFont Error: %{r}s\n", TTF_GetError());
		return (EXIT_FAILURE);
	}
	ttf->color.r = 0;
	ttf->color.g = 0;
	ttf->color.b = 0;
	ttf->color.a = 100;
	ttf->rect.x = 0;
	ttf->rect.y = 0;
	ttf->rect.w = WIN_WIDTH / 8;
	ttf->rect.h = WIN_HEIGHT / 8;
	return (EXIT_SUCCESS);
}

void    init_wolf(t_wolf *wolf, char *title)
{
        init_sdl_struct(&(wolf->sdl));
        init_ttf_struct(&(wolf->ttf));
        init_data_struct(&(wolf->data), title);
        init_player_struct(&(wolf->player), *(wolf->data.map_ptr), wolf->data.map_width, wolf->data.map_height);
        init_raycast_struct(&(wolf->raycast), wolf->player.x, wolf->player.y);
}