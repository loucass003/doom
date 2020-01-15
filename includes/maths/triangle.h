/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:28:03 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/15 03:45:07 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRIANGLE_H
# define TRIANGLE_H

# include <libft.h>
# include "maths/vertex.h"



t_bool				inside_triangle(t_vec4 a, t_vec4 b, t_vec4 c, t_vec4 p);
t_vec3				get_triangle_normal(t_vec3 p0, t_vec3 p1, t_vec3 p2);

#endif
