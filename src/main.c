/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afonck <afonck@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 13:57:03 by sluetzen          #+#    #+#             */
/*   Updated: 2019/10/15 16:08:30 by afonck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <math.h>
#include "wolf3d.h"

char	check_line(char *line)
{
	if (*line == '\0')
		return ('\0');
	while (*line != '\0')
	{
		if (*line != '0' && *line != '1' && *line != 'X')
			return (*line);
		line++;
	}
	return ('0');
}

void	copy_line(char *charline, char *line, int width)
{
	int i;

	i = 0;
	while (i < width)
	{
		charline[i] = line[i];
		i++;
	}
}

void	fill_chartab(char chartab[MAX_MAP][MAX_MAP], char *title, int *map_width, int *map_height)
{
	char	*line;
	int	ret;
	int	fd;
	int	w;

	*map_height = 0;
	ret = 0;
	w = 0;
	if ((fd = open(title, O_RDONLY | O_NOFOLLOW)) == -1)
	{
		ft_dprintf(STDERR_FILENO, "%{r}s could not be opened\n", title);
		exit(EXIT_FAILURE);
	}
	while ((ret = get_next_line(fd, &line)) > 0 && *map_height < MAX_MAP)
	{
		if (check_line(line) != '0')
		{
			ft_dprintf(STDERR_FILENO, "invalid character %{r}c in map\n", check_line(line));
			ft_memdel((void **)&line);
			close(fd);
			exit(EXIT_FAILURE);
		}
		if (*map_height == 0)
			w = ft_strlen(line);
		*map_width = ft_strlen(line);
		if (*map_width != w)
		{
			ft_dprintf(STDERR_FILENO, "%{r}s is an uneven map, only rectangles or squares are allowed\n", title);
			ft_memdel((void **)&line);
			close(fd);
			exit(EXIT_FAILURE);
		}
		if (*map_width > MAX_MAP)
		{
			ft_dprintf(STDERR_FILENO, "%{r}s is too big, please modify MAP_MAX to %{b}d\n", title, *map_width);
			ft_memdel((void **)&line);
			close(fd);
			exit(EXIT_FAILURE);
		}
		copy_line(chartab[*map_height], line, *map_width);
		ft_memdel((void **)&line);
		(*map_height)++;
	}
	ft_memdel((void **)&line);
	close(fd);
	if (*map_height >= MAX_MAP)
	{
		ft_dprintf(STDERR_FILENO, "%{r}s is too big, please modify MAP_MAX to %{b}d\n", title, *map_height);
		exit(EXIT_FAILURE);
	}
}

void	fill_map(int (*map)[MAX_MAP][MAX_MAP], char *title, int *map_width, int *map_height)
{
	char	chartab[MAX_MAP][MAX_MAP];
	int		i;
	int		j;

	i = 0;
	fill_chartab(chartab, title, map_width, map_height);
	while (i < *map_height)
	{
		j = 0;
		while (j < *map_width)
		{
			if (chartab[i][j] == 'X')
				(*map)[i][j] = 'X';
			else
				(*map)[i][j] = chartab[i][j] - '0';
			j++;
		}
		i++;
	}
}

void	verify_bordermap(int const (*map)[MAX_MAP][MAX_MAP], char *title, int map_width, int map_height)
{
	int i;
	int j;
	int k;
	int l;
	
	i = 0;
	j = 0;
	k = 0;
	l = 0;
	while ((*map)[0][i] == 1 && i < map_width)
		i++;
	while ((*map)[map_height - 1][j] == 1 && j < map_width)
		j++;
	while ((*map)[k][0] == 1 && k < map_height)
		k++;
	while ((*map)[l][map_width - 1] == 1 && l < map_height)
		l++;
	if (i != map_width || j != map_width || k != map_height || l != map_height)
	{
		ft_dprintf(STDERR_FILENO, "map %{r}s is not surrounded by walls (1), exiting...\n", title);
		exit (EXIT_FAILURE);
	}
}

void	find_player_pos(t_player *player, int map[MAX_MAP][MAX_MAP], int map_width, int map_height)
{
	int	i;
	int	j;

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
				player->x = j;
				player->y = i;
				map[i][j] = 0;
				return ;
			}
			j++;
		}
		i++;
	}
	ft_dprintf(STDERR_FILENO, "no suitable starting position found for player, exiting...\n");
	exit (EXIT_FAILURE);
}

void	fill_pix(int *pixels, int x, int y, int color)
{
	if (x < WIN_WIDTH && y < WIN_HEIGHT && x > 0 && y > 0)
		pixels[x + y * WIN_WIDTH] = color;
}

void	draw_vertical(int *pixels, int x, int y1, int y2, int color)
{
	while (y1 < y2)
	{
		fill_pix(pixels, x, y1, color);
		y1++;
	}
}

void	print_map(int map[MAX_MAP][MAX_MAP], int width, int height)
{
	int i;
	int j;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			ft_printf("%d ", map[i][j]);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
	write(1, "\n", 1);
}

int	*create_pixel_tab(void)
{
	int *ptr;

	ptr = NULL;
	if ((ptr = (int *)malloc(sizeof(int) * WIN_WIDTH * WIN_HEIGHT)) == NULL)
		return (NULL);
	ft_memset(ptr, 255, WIN_WIDTH * WIN_HEIGHT * sizeof(int));
	return (ptr);
}

void	main_loop(t_wolf *wolf)
{
	int		start_clock;
	int		delta_clock;
	int		current_FPS;

	current_FPS = 0;
	fill_tex(wolf->data.texture);
	//int pixels[WIN_WIDTH * WIN_HEIGHT];
	//wolf->data.img_ptr = &pixels[0];
	if ((wolf->data.img_ptr = create_pixel_tab()) == NULL)
		return ;

	//int	leftMouseButtonDown = 0;

	const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
	SDL_WarpMouseInWindow(wolf->sdl.win, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	while (!wolf->data.quit)
	{
		//SDL_UpdateTexture(sdl->tex, NULL, data->pixels, WIN_WIDTH * sizeof(int));
		while (SDL_PollEvent(&(wolf->sdl.event)) != 0)
		{
			if (wolf->sdl.event.type == SDL_QUIT || (wolf->sdl.event.type == SDL_KEYDOWN && wolf->sdl.event.key.keysym.sym == SDLK_ESCAPE))
				wolf->data.quit = 1;
		}
			start_clock = SDL_GetTicks();
			wolf->ttf.fps = ft_itoa(current_FPS);
			
			if ((wolf->ttf.surf_message = TTF_RenderText_Solid(wolf->ttf.font, wolf->ttf.fps, wolf->ttf.color)) == NULL)
			{
				ft_dprintf(STDERR_FILENO, "TTF_RenderText_Solid error = %{r}s\n", TTF_GetError());
				ft_memdel((void **)&wolf->ttf.fps);
				return ;
			}
			ft_memdel((void **)&wolf->ttf.fps);
			if ((wolf->ttf.message = SDL_CreateTextureFromSurface(wolf->sdl.ren, wolf->ttf.surf_message)) == NULL)
			{
				ft_dprintf(STDERR_FILENO, "SDL_CreateTextureFromSurface error = %{r}s\n", SDL_GetError());
				return ;
			}
		SDL_FreeSurface(wolf->ttf.surf_message);
		wolf->ttf.surf_message = NULL;
		//ft_memset(pixels, 255, WIN_WIDTH * WIN_HEIGHT * sizeof(int));
		ft_memset(wolf->data.img_ptr, 255, WIN_WIDTH * WIN_HEIGHT * sizeof(int));
		movement(&(wolf->player), &(wolf->data), keyboard_state_array);
		multithread(wolf);
		//const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
		//movement(&(wolf->player), &(wolf->sdl), &(wolf->data), keyboard_state_array);
		//SDL_SetRenderDrawColor(sdl->ren, 255, 255, 255, 255);
		SDL_UpdateTexture(wolf->sdl.tex, NULL, wolf->data.img_ptr, WIN_WIDTH * sizeof(int));
		SDL_RenderClear(wolf->sdl.ren);
		SDL_RenderCopy(wolf->sdl.ren, wolf->sdl.tex, NULL, NULL);
		SDL_RenderCopy(wolf->sdl.ren, wolf->ttf.message, NULL, &wolf->ttf.rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
		SDL_RenderPresent(wolf->sdl.ren);
		delta_clock = SDL_GetTicks() - start_clock;
		if (delta_clock != 0)
			current_FPS = 1000 / delta_clock;
		SDL_DestroyTexture(wolf->ttf.message);
		//printf("%s\n", SDL_GetError());
		wolf->ttf.message = NULL;
	}
}

int	main(int argc, char *argv[])
{
	t_wolf	wolf;
	int		map[MAX_MAP][MAX_MAP];

	wolf.data.map_ptr = &map;
	if (argc != 2)
	{
		ft_dprintf(STDERR_FILENO, "usage: ./wolf3d %{g}s\n", "[valid .w3d map]");
		return (EXIT_FAILURE);
	}
	if (MAX_MAP > 100 || WIN_WIDTH > 1920 || WIN_HEIGHT > 1080 || MAX_MAP < 10 || WIN_WIDTH < 100 || WIN_HEIGHT < 100)
	{
		ft_dprintf(STDERR_FILENO, "max/min size of map = %{b}s, you chose %{r}d / max/min screen size = %{b}s, you chose %{r}d x %{r}d\n", "100/10", MAX_MAP, "1920 x 1080/100 x 100", WIN_WIDTH, WIN_HEIGHT);
		return (EXIT_FAILURE);
	}
	if (NB_THREADS > 50 || NB_THREADS < 1)
	{
		ft_printf("max nb of threads = %{r}s, you chose %{r}d\n", "50", NB_THREADS);
		return (EXIT_FAILURE);
	}
	init_wolf(&wolf, argv[1]);
	if (init_SDL(&(wolf.sdl.win), &(wolf.sdl.ren), &(wolf.sdl.tex)) != EXIT_SUCCESS)
	{
		free_SDL(&(wolf.sdl.win), &(wolf.sdl.ren), &(wolf.sdl.tex));
		return (EXIT_FAILURE);
	}
	if (init_TTF(&(wolf.ttf)) != EXIT_SUCCESS)
	{
		free_SDL(&(wolf.sdl.win), &(wolf.sdl.ren), &(wolf.sdl.tex));
		free_TTF(&(wolf.ttf));
		return (EXIT_FAILURE);
	}
	main_loop(&wolf);
	ft_memdel((void *)&wolf.data.img_ptr);
	free_SDL(&(wolf.sdl.win), &(wolf.sdl.ren), &(wolf.sdl.tex));
	free_TTF(&(wolf.ttf));
	return (EXIT_SUCCESS);
}
