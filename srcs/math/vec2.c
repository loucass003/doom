/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 01:54:26 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/20 17:02:15 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

t_vec2		vec2_add_s(t_vec2 v, float s)
{
	v.x += s;
	v.y += s;
	return (v);
}

t_vec2		vec2_sub_s(t_vec2 v, float s)
{
	v.x -= s;
	v.y -= s;
	return (v);
}
