/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 14:54:33 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/09 19:27:27 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include "libft.h"

typedef enum		e_node_type
{
	N_NODE,
	N_LEAF
}					t_node_type;

typedef enum		e_poly_type
{
	P_FLOOR,
	P_WALL
}					t_poly_type;

typedef struct		s_line
{
	t_vec2			a;
	t_vec2			b;
	t_vec2			n;
}					t_line;

typedef struct		s_poly
{
	t_poly_type		type;
	t_line			line;
}					t_poly;

typedef struct		s_rect
{
	t_vec2			pos;
	t_vec2			size;
}					t_rect;

typedef struct		s_node
{
	t_node_type		type;
	struct s_node	*front;
	struct s_node	*back;
	t_line			partition;
}					t_node;

typedef struct		s_leaf
{
	t_node			super;
	float			floor_h;
	float			ceil_h;
	t_rect			bounds;
	t_bool			is_back;
}					t_leaf;

t_line lines[] = {
	{ .a = { 0, 150 }, .b = { 0, 450 } },
	{ .a = { 0, 450 }, .b = { 800, 450 } },
	{ .a = { 800, 450 }, .b = { 800, 300 } },
	{ .a = { 800, 300 }, .b = { 500, 300 } },
	{ .a = { 500, 300 }, .b = { 500, 75 } },
	{ .a = { 500, 75 }, .b = { 0, 150 } }
};

int		main(void)
{
	return (0);
}
