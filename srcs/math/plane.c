/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 12:04:16 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/10 18:35:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_vec3	get_plane_normal(t_vec3 p0, t_vec3 p1, t_vec3 p2)
{
	const t_vec3 a = ft_vec3_sub(p1, p0);
	const t_vec3 b = ft_vec3_sub(p2, p0);
	
	return (ft_vec3_norm(ft_vec3_cross(a, b)));
}
