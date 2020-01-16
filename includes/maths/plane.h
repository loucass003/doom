/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 22:08:31 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/16 15:44:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLANE_H
# define PLANE_H

#include <libft.h>

typedef struct	s_plane 
{
	t_vec3	origin;
	t_vec3	normal;
	t_vec3	point;
	float	f;
}				t_plane;

t_plane			triangle_to_plane(t_vec3 a, t_vec3 b, t_vec3 c);
t_plane			plane_new(t_vec3 origin, t_vec3 normal);
float			distance_to_plane( t_plane plane, t_vec3 point);
t_bool			is_front_facing(t_plane plane, t_vec3 direction);

#endif
