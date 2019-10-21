/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_progress.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 20:55:53 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/21 20:09:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "gui.h"
#include "doom.h"

void		c_progress_render(t_doom *doom, t_component *self, t_img *img)
{
	t_progress	*progress;
	int			x;
	int			y;
	int			color;

	if (self->type != C_PROGRESS)
		return ;
	progress = (t_progress *)self;
	x = -1;
	while(++x < self->bounds.w && !!(y = -1))
	{
		while(++y < self->bounds.h)
		{
			color = self->bounds.w / 100. * progress->value > x
				? progress->fg_color
				: progress->bg_color;
			doom->screen.pixels[(y + self->bounds.y) *
				doom->screen.width + x + self->bounds.x] = color;
		}
	}
}

t_component	 *create_progress(SDL_Rect bounds)
{
	t_progress *progress;

	if (!(progress = (t_progress *)malloc(sizeof(t_progress))))
		return (NULL);
	ft_bzero(progress, sizeof(t_progress));
	progress->super = (t_component) { .enabled = TRUE, .visible = TRUE, 
		.bounds = bounds, .type = C_PROGRESS, 
		.render = c_progress_render };
	return ((t_component *)progress);
}
