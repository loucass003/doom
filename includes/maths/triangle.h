/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:28:03 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 02:48:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRIANGLE_H
# define TRIANGLE_H

# include <libft.h>
# include "maths/vertex.h"
# include "doom.h"

typedef struct		s_triangle
{
	t_vertex		a;
	t_vertex		b;
	t_vertex		c;
}					t_triangle;

void				draw_triangle(t_doom *doom, t_triangle projected, t_mtl *mtl);
void				process_triangle(t_doom *doom, t_mtl *mtl, t_triangle triangle);
void				post_process_triangle(t_doom *doom, t_mtl *mtl, t_triangle triangle);
t_bool				inside_triangle(t_vec3 a, t_vec3 b, t_vec3 c, t_vec3 p);
t_vec3				get_triangle_normal(t_vec3 p0, t_vec3 p1, t_vec3 p2);

#endif