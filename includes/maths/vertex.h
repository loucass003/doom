/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 16:55:01 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/16 16:56:17 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VERTEX_H
# define VERTEX_H

# include <libft.h>
# include "maths/vec4.h"

typedef struct		s_vertex
{
	t_vec4			pos;
	t_vec3			normal;
	float			light_color;
	t_vec2			tex;
}					t_vertex;

t_vertex			vertex_add(t_vertex a, t_vertex b);
t_vertex			vertex_sub(t_vertex a, t_vertex b);
t_vertex			vertex_div_s(t_vertex a, float v);
t_vertex			vertex_mul_s(t_vertex a, float v);
t_vertex			vertex_interpolate(t_vertex a, t_vertex b, float v);

#endif
