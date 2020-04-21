/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 03:08:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/08 14:52:21 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H
# include "libft.h"
# include "json.h"

typedef struct		s_size
{
	size_t			width;
	size_t			height;
}					t_size;

typedef	union		u_size
{
	size_t			a[2];
	t_size			s;
}					t_size_u;

typedef struct		s_pixel
{
	int				x;
	int				y;
	int				color;
}					t_pixel;

typedef struct		s_color
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	uint8_t		a;
}					t_color;

typedef	union		u_color
{
	uint8_t		a[4];
	t_color		c;
}					t_color_u;

t_color				ft_color(uint8_t r, uint8_t g, uint8_t b);
t_color				ft_i_color(int c);
int					ft_color_i(t_color c);
t_color				ft_color_gradient(t_color a, t_color b, float percent);
t_bool				ft_json_color(t_json_value *val, t_color *color);
t_color				ft_alpha_blend(t_color c1, t_color c2);

t_pixel				ft_pixel_add(t_pixel a, t_pixel b);
t_pixel				ft_pixel_sub(t_pixel a, t_pixel b);
t_pixel				ft_pixel_div(t_pixel a, t_pixel b);
t_pixel				ft_pixel_mul(t_pixel a, t_pixel b);

t_bool				ft_json_size(t_json_value *val, t_size *size);

#endif
