/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 23:32:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/08 14:45:53 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <SDL.h>
#include <math.h>

#define UNIT 20

t_vec2	walls[][2] = {
	{ {70, 20}, {70, 70} },
	{ {20, 20}, {70, 20} },
	{ {20, 20}, {20, 70} },
	{ {20, 70}, {70, 70} }
};

typedef struct	s_player
{
	t_vec2		pos;
	float		rot;
}				t_player;


void	draw_player(t_vec2 pos, float rot, SDL_Renderer *r)
{
	SDL_SetRenderDrawColor(r, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(r, pos.x, pos.y, pos.x + cosf(rot) * UNIT / 2, pos.y + sinf(rot) * UNIT / 2);
	SDL_SetRenderDrawColor(r, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(r, &(SDL_Rect){ pos.x - 1, pos.y - 1, 3, 3});
	SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);
}

t_vec2	intersect(t_vec2 a, t_vec2 b, t_vec2 c, t_vec2 d)
{
	float x = ft_vec2_cross(a, b);
	float y = ft_vec2_cross(c, d);
	float det = ft_vec2_cross(ft_vec2_sub(a, b), ft_vec2_sub(c, d));
	x = ft_vec2_cross((t_vec2){ x, a.x - b.x}, (t_vec2){y, c.x - d.x}) / det;
	y = ft_vec2_cross((t_vec2){ x, a.y - b.y}, (t_vec2){y, c.y - d.y}) / det;
	return (t_vec2){ x, y };
}

int		main(void)
{
	t_player p = {
		.pos = { 50, 50 },
		.rot = 0
	};

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		SDL_Window* window = SDL_CreateWindow("|| Wolf 3D ||",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 111 * 7, 157 * 3,
			SDL_WINDOW_SHOWN);
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		SDL_RenderSetLogicalSize(renderer, 111 * 3, 157);
		
		SDL_bool done = SDL_FALSE;

		while (!done) {
			SDL_Event event;

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

			int count_lines = sizeof(walls) / sizeof(t_vec2) / 2;

			for (int i = 0; i < count_lines; i++)
			{			
				t_vec2 a = walls[i][0];
				t_vec2 b = walls[i][1];
				SDL_RenderSetViewport(renderer, &(SDL_Rect){4, 4, 104, 150});
				SDL_RenderDrawRect(renderer, &(SDL_Rect){ 0, 0, 103, 149 });
				SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
				SDL_RenderDrawLine(renderer, p.pos.x, p.pos.y, p.pos.x + cosf(p.rot) * 5, p.pos.y + sinf(p.rot)  * 5);
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawPoint(renderer, p.pos.x, p.pos.y);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				
				t_mat2 m = ft_mat2_rotation(p.rot);
				t_vec2 ta = ft_mat2_mulv(m, ft_vec2_sub(a, p.pos));
				t_vec2 tb = ft_mat2_mulv(m, ft_vec2_sub(b, p.pos));

				SDL_RenderSetViewport(renderer, &(SDL_Rect){103 + 8, 4, 104, 150});
				SDL_RenderDrawRect(renderer, &(SDL_Rect){ 0, 0, 103, 149 });
				SDL_RenderDrawLine(renderer, 50 - ta.x, 50 - ta.y, 50 - tb.x, 50 - tb.y);
				SDL_RenderDrawLine(renderer, 50, 50, 50, 45);

				SDL_RenderSetViewport(renderer, &(SDL_Rect){103 * 2 + 4 * 3, 4, 104, 150});
				SDL_RenderDrawRect(renderer, &(SDL_Rect){ 0, 0, 103, 149 });
				if (ta.y > 0 || tb.y > 0)
				{
					t_vec2 i1 = intersect(ta, tb, (t_vec2){-0.0001, 0.0001}, (t_vec2){-20, 5});
					t_vec2 i2 = intersect(ta, tb, (t_vec2){0.0001, 0.0001}, (t_vec2){20, 5});
					if (ta.y <= 0)
						ta = i1.y > 0 ? i1 : i2;
					if (tb.y <= 0)
						tb = i1.y > 0 ? i1 : i2;
					float x1 = -ta.x * 16. / ta.y;
					float x2 = -tb.x * 16. / tb.y;
					float y1a = -50. / ta.y;
					float y1b = 50. / ta.y;
					float y2a = -50. / tb.y;
					float y2b = 50. / tb.y;
					SDL_RenderDrawLine(renderer, 50 + x1, 50 + y1a, 50 + x2, 50 + y2a);
					SDL_RenderDrawLine(renderer, 50 + x1, 50 + y1b, 50 + x2, 50 + y2b);
					SDL_RenderDrawLine(renderer, 50 + x1, 50 + y1a, 50 + x1, 50 + y1b);
					SDL_RenderDrawLine(renderer, 50 + x2, 50 + y2a, 50 + x2, 50 + y2b);
				}
			}

			SDL_RenderPresent(renderer);
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
					done = SDL_TRUE;
				if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.scancode == SDL_SCANCODE_W || event.key.keysym.scancode == SDL_SCANCODE_S)
					{
						p.pos.y += sinf(p.rot) * (event.key.keysym.scancode == SDL_SCANCODE_W ? 1 : -1) * 3;
						p.pos.x += cosf(p.rot) * (event.key.keysym.scancode == SDL_SCANCODE_W ? 1 : -1) * 3;
					}
					if (event.key.keysym.scancode == SDL_SCANCODE_A || event.key.keysym.scancode == SDL_SCANCODE_D)
					{
						p.pos.y += -cosf(p.rot) * (event.key.keysym.scancode == SDL_SCANCODE_A ? 1 : -1) * 3;
						p.pos.x += sinf(p.rot) * (event.key.keysym.scancode == SDL_SCANCODE_A ? 1 : -1) * 3;
					}
					if (event.key.keysym.scancode == SDL_SCANCODE_J || event.key.keysym.scancode == SDL_SCANCODE_L)
						p.rot += (event.key.keysym.scancode == SDL_SCANCODE_L ? 1 : -1) * 0.1 ;
				}
			}
		}
	

		if (renderer) {
			SDL_DestroyRenderer(renderer);
		}
		if (window) {
			SDL_DestroyWindow(window);
		}
	}
	SDL_Quit();
	return (0);
}