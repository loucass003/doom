/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 03:03:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/28 16:31:12 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATHS_H
# define MATHS_H
# include "libft.h"
# include "json.h"

typedef struct		s_vec3
{
	float			x;
	float			y;
	float			z;
}					t_vec3;

typedef	union		u_vec3_u
{
	float			a[3];
	t_vec3			v;
}					t_vec3_u;

typedef struct		s_vec2
{
	float			x;
	float			y;
}					t_vec2;

typedef	union		u_vec2_u
{
	float			a[2];
	t_vec2			v;
}					t_vec2_u;

typedef struct		s_mat2_data
{
	float			m00;
	float			m01;
	float			m10;
	float			m11;
}					t_mat2_data;

typedef struct		s_mat4_data
{
	float			m00;
	float			m01;
	float			m02;
	float			m03;
	float			m10;
	float			m11;
	float			m12;
	float			m13;
	float			m20;
	float			m21;
	float			m22;
	float			m23;
	float			m30;
	float			m31;
	float			m32;
	float			m33;
}					t_mat4_data;

typedef	union		u_mat4
{
	float			a[4][4];
	float			b[16];
	t_mat4_data		d;
}					t_mat4;

typedef union		u_mat2
{
	float			a[2][2];
	t_mat2_data		d;
}					t_mat2;

int					ft_abs(int n);
float				ft_absf(float n);

float				ft_sqrt(float f);
int					ft_pow(int n, int pow);

t_bool				ft_json_vec3(t_json_value *val, t_vec3 *vec);
t_bool				ft_json_vec2(t_json_value *val, t_vec2 *vec);

t_vec3				ft_vec3_add(t_vec3 a, t_vec3 b);
t_vec3				ft_vec3_sub(t_vec3 a, t_vec3 b);
t_vec3				ft_vec3_div(t_vec3 a, t_vec3 b);
t_vec3				ft_vec3_div_s(t_vec3 a, float v);
t_vec3				ft_vec3_mul(t_vec3 a, t_vec3 b);
t_vec3				ft_vec3_mul_s(t_vec3 a, float v);
float				ft_vec3_dot(t_vec3 a, t_vec3 b);
float				ft_vec3_len(t_vec3 v);
t_vec3				ft_vec3_norm(t_vec3 v);
t_vec3				ft_vec3_cross(t_vec3 a, t_vec3 b);
t_vec3				ft_vec3_inv(t_vec3 v);

t_vec2				ft_vec2_add(t_vec2 a, t_vec2 b);
t_vec2				ft_vec2_sub(t_vec2 a, t_vec2 b);
t_vec2				ft_vec2_div(t_vec2 a, t_vec2 b);
t_vec2				ft_vec2_div_s(t_vec2 a, float s);
t_vec2				ft_vec2_mul(t_vec2 a, t_vec2 b);
t_vec2				ft_vec2_mul_s(t_vec2 a, float s);
float				ft_vec2_cross(t_vec2 a, t_vec2 b);
float				ft_vec2_dot(t_vec2 a, t_vec2 b);
float				ft_vec2_cross(t_vec2 a, t_vec2 b);
float				ft_vec2_len(t_vec2 v);
t_vec2				ft_vec2_norm(t_vec2 v);
t_vec2				ft_vec2_inv(t_vec2 v);

t_mat4				ft_mat4_identity(void);
t_mat4				ft_mat4_translation(t_vec3 t);
t_mat4				ft_mat4_rotation(t_vec3 angles);
t_mat4				ft_mat4_scale(t_vec3 s);
t_mat4				ft_mat4_mul(t_mat4 a, t_mat4 b);
t_vec3				ft_mat4_mulv(t_mat4 m, t_vec3 v);

t_mat2				ft_mat2_identity(void);
t_mat2				ft_mat2_rotation(float theta);
t_mat2				ft_mat2_scale(t_vec2 s);
t_mat2				ft_mat2_mul(t_mat2 a, t_mat2 b);
t_vec2				ft_mat2_mulv(t_mat2 m, t_vec2 v);

int					ft_min(int a, int b);
int					ft_max(int a, int b);

#endif
