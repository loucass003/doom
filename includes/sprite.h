/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:27:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/05 19:10:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_H
# define SPRITE_H

# include <libft.h>
# include "render.h"

typedef struct	s_sprite
{
	t_bool		always_facing_player;
	t_bool		is_spritesheet;
	int			current_cell;
	t_vec2		cells_count;
	t_vec2		uv_min;
	t_vec2		uv_max;
}				t_sprite;

void	set_current_cell(t_renderable *r, int x, int y);
t_bool	create_sprite(t_renderable *r, t_mtl mtl, t_vec2 cells_count);

#endif
