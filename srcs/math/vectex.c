/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/20 14:47:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/20 23:05:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_vertex		vertex_mul_s(t_vertex a, float v)
{
	a.pos = ft_vec3_mul_s(a.pos, v);
	a.tex = ft_vec2_mul_s(a.tex, v);
	return (a);
}

t_vertex		vertex_div_s(t_vertex a, float v)
{
	a.pos = ft_vec3_div_s(a.pos, v);
	a.tex = ft_vec2_div_s(a.tex, v);
	return (a);
}

t_vertex		vertex_sub(t_vertex a, t_vertex b)
{
	a.pos = ft_vec3_sub(a.pos, b.pos);
	a.tex = ft_vec2_sub(a.tex, b.tex);
	return (a);
}

t_vertex		vertex_add(t_vertex a, t_vertex b)
{
	a.pos = ft_vec3_add(a.pos, b.pos);
	a.tex = ft_vec2_add(a.tex, b.tex);
	return (a);
}

t_vertex		vertex_interpolate(t_vertex a, t_vertex b, float alpha)
{
	return (vertex_add(a, vertex_mul_s(vertex_sub(b, a), alpha)));
}
