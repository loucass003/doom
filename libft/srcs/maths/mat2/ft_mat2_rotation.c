/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mat2_rotation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 19:06:00 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/23 12:58:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

t_mat2			ft_mat2_rotation(float theta)
{
	const float	c = cosf(theta);
	const float s = sinf(theta);

	return ((t_mat2)((t_mat2_data){
		c, -s,
		s, c
	}));
}
