/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:36:44 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 02:07:22 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_H
# define LINE_H

typedef struct		s_line
{
	t_vec2	a;
	t_vec2	b;
	t_vec2	normal;
}					t_line;

float				line_get_distance(t_line l, t_vec2 p);
t_vec2				line_normal(t_line l);

#endif
