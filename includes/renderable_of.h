/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderable_of.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/05 17:25:21 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/05 17:25:52 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERABLE_OF_H
# define RENDERABLE_OF_H

typedef enum		e_renderable_type
{
	RENDERABLE_UNKNOWN,
	RENDERABLE_OBJ,
	RENDERABLE_POLYGON,
	RENDERABLE_SPRITE
}					t_renderable_type;

typedef union		u_renderable_of_data
{
	struct s_polygon		*polygon;
	struct s_obj			*obj;
	struct s_sprite			*sprite;
}					t_renderable_of_data;

typedef struct		s_renderable_of
{
	t_renderable_type		type;
	t_renderable_of_data	data;
}					t_renderable_of;

#endif