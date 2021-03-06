/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:27:28 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/16 16:47:44 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_H
# define SPRITE_H

# include <libft.h>
# include "render.h"
# include "ressource.h"

typedef struct	s_sprite
{
	t_bool		always_facing_player;
	int			current_cell;
	t_vec2		cells_count;
	t_vec2		uv_min;
	t_vec2		uv_max;
	t_vec3		hitbox_radius;
	t_ressource	*texture;
}				t_sprite;

void			set_current_cell(t_renderable *r, int x, int y);
t_bool			create_sprite_renderable(t_renderable *r, t_sprite *sprite);
t_sprite		*create_sprite(t_vec2 cells_count, t_ressource *texture);
t_bool			write_sprite(t_ressource_manager *r, t_sprite *sprite);
t_bool			read_sprite(t_ressource_manager *r, t_sprite **sprite);
void			compute_sprite_hitbox(t_renderable *r);

#endif
