/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 15:38:53 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/16 14:29:17 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOR_H
# define DOOR_H

# include "render.h"

typedef struct	s_door
{
	int			indexes[3];
	float		open_value;
	int			door_1;
	int			door_2;
	t_ints		*door_1_indexes;
	t_ints		*door_2_indexes;
	t_bool		have_groups;
}				t_door;

t_bool			create_door(struct s_doom *doom, int indexes[3],
					t_renderable *r);
t_bool			update_renderable_door(t_render_context *ctx, t_renderable *r);
void			transform_renderable_door(t_renderable *r);
void			free_door(t_door **d_addr);

#endif
