/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wav_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 02:47:12 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/19 03:20:02 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "audio.h"

t_bool		wav_return_error(char *error)
{
	ft_putstr("WAV Loader error: ");
	ft_putendl(error);
	return (FALSE);
}