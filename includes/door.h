/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 15:38:53 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/31 19:42:24 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOR_H
# define DOOR_H

#include "render.h"

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

t_bool				create_door(struct s_doom *doom, int indexes[3], t_renderable *r);

#endif
