/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:28:03 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 19:10:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRIANGLE_H
# define TRIANGLE_H

# include <libft.h>
# include "maths/vertex.h"
# include "doom.h"


t_bool				inside_triangle(t_vec3 a, t_vec3 b, t_vec3 c, t_vec3 p);
t_vec3				get_triangle_normal(t_vec3 p0, t_vec3 p1, t_vec3 p2);

#endif
