/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/20 14:47:41 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/10 16:01:02 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "maths/vertex.h"

t_vertex		vertex_mul_s(t_vertex a, float v)
{
	a.pos = ft_vec4_mul_s(a.pos, v);
	a.posw = ft_vec4_mul_s(a.posw, v);
	a.tex = ft_vec2_mul_s(a.tex, v);
	a.normal = ft_vec3_mul_s(a.normal, v);
	a.light_color = a.light_color * v;
	return (a);
}

t_vertex		vertex_div_s(t_vertex a, float v)
{
	a.pos = ft_vec4_div_s(a.pos, v);
	a.posw = ft_vec4_div_s(a.posw, v);
	a.tex = ft_vec2_div_s(a.tex, v);
	a.normal = ft_vec3_div_s(a.normal, v);
	a.light_color = a.light_color / v;
	return (a);
}

t_vertex		vertex_sub(t_vertex a, t_vertex b)
{
	a.pos = ft_vec4_sub(a.pos, b.pos);
	a.posw = ft_vec4_sub(a.posw, b.pos);
	a.tex = ft_vec2_sub(a.tex, b.tex);
	a.normal = ft_vec3_sub(a.normal, b.normal);
	a.light_color = a.light_color - b.light_color;
	return (a);
}

t_vertex		vertex_add(t_vertex a, t_vertex b)
{
	a.pos = ft_vec4_add(a.pos, b.pos);
	a.posw = ft_vec4_add(a.posw, b.pos);
	a.tex = ft_vec2_add(a.tex, b.tex);
	a.normal = ft_vec3_add(a.normal, b.normal);
	a.light_color = a.light_color + b.light_color;
	return (a);
}

t_vertex		vertex_interpolate(t_vertex a, t_vertex b, float alpha)
{
	return (vertex_add(a, vertex_mul_s(vertex_sub(b, a), alpha)));
}
