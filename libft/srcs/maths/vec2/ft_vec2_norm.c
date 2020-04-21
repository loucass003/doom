/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec2_norm.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 18:09:15 by llelievr          #+#    #+#             */
/*   Updated: 2018/12/12 15:26:25 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_vec2		ft_vec2_norm(t_vec2 v)
{
	const float	len = ft_vec2_len(v);

	return (ft_vec2_div(v, (t_vec2) { len, len }));
}
