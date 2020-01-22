/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wav_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/19 02:47:12 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 14:19:25 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "audio.h"

t_bool		wav_return_error(char *error)
{
	ft_putstr("WAV Loader error: ");
	ft_putendl(error);
	return (FALSE);
}
