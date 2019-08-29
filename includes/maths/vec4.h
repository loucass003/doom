/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:15:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 01:27:43 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC4_H
# define VEC4_H

typedef struct		s_vec4
{
	float			x;
	float			y;
	float			z;
	float			w;
}					t_vec4;

typedef	union		u_vec4_u
{
	float			a[4];
	t_vec4			v;
}					t_vec4_u;

t_vec4				vec3_to_4(t_vec3 v);
t_vec3				vec4_to_3(t_vec4 v);
t_vec3				vec4_to_3w(t_vec4 v);
t_vec4				mat4_mulv4(t_mat4 m, t_vec4 p);
t_vec4				ft_vec4_mul_s(t_vec4 a, float v);
t_vec4				ft_vec4_mul(t_vec4 a, t_vec4 b);
t_vec4				ft_vec4_sub_s(t_vec4 a, float v);
t_vec4				ft_vec4_div_s(t_vec4 a, float v);
t_vec4				ft_vec4_sub(t_vec4 a, t_vec4 b);
t_vec4				ft_vec4_add(t_vec4 a, t_vec4 b);
t_vec4				ft_vec4_interpolate(t_vec4 a, t_vec4 b, float alpha);
t_vec3				vec3_clamp_s(t_vec3 v, float min, float max);

#endif