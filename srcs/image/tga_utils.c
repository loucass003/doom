/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 03:04:16 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 03:04:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tga.h"

t_bool		tga_return_error(char *error)
{
	ft_putstr("TGA Loader error: ");
	ft_putendl(error);
	return (FALSE);
}