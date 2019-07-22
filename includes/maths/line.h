/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:36:44 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/26 14:11:43 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_H
# define LINE_H
# include "doom.h"

typedef enum		e_line_type
{
	L_WALL,
	L_FLOOR
}					t_line_type;

typedef struct		s_line
{
	t_vec2			a;
	t_vec2			b;
	t_line_type		type;
}					t_line;

typedef struct		s_line_list
{
	t_line				line;
	SDL_Surface			*texture;
	struct s_line_list	*next;
}					t_line_list;

typedef enum		e_side
{
	S_COLINEAR = 0,
	S_FRONT = 1,
	S_BACK = 2,
	S_SPANNING = 3
}					t_side;

t_bool				intersect(t_line *line, t_line *seg, t_vec2 *intersect);
t_vec2				line_normal(t_line *line);
t_side				get_side_thin(t_line *partition, t_vec2 seg);
t_side				get_side_thick(t_line *partition, t_vec2 seg);
t_side				get_side(t_line *partition, t_line seg);

#endif
