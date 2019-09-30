/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:27:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/30 13:53:46 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_H
# define SPRITE_H

# include <libft.h>

typedef struct	s_sprite
{
	t_bool		always_facing_player;
	t_bool		is_spritesheet;
	int			current_cell;
	t_vec2		cells_count;
	t_vec2		uv_min;
	t_vec2		uv_max;
}				t_sprite;

#endif
