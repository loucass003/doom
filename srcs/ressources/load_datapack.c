/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_datapack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 02:52:17 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/25 06:30:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <libft.h>
#include <unistd.h>
#include "doom.h"
#include "ressource.h"

t_bool		close_datapack(t_doom *doom)
{
	if (doom->res_manager.fd == -1)
		return (FALSE);
	return (close(doom->res_manager.fd) != -1);
}

t_bool		load_datapack(t_doom *doom, char *path)
{
	if ((doom->res_manager.fd = open(path, O_RDWR | O_CREAT, 0666)) == -1)
		return (FALSE);
	//write(doom->res_manager.fd, "TOTO", 4);
	printf("datapack OPEN\n");
	return (TRUE);
}