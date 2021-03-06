/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_datapack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 22:14:40 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/02 16:15:44 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <libft.h>
#include "ressource.h"
#include "doom.h"
#include "write_structs.h"

void		dp_write(t_ressource_manager *rm, const void *buf, size_t n)
{
	if (!buf)
		return ;
	write(rm->reader.fd, buf, n);
}

t_bool		write_header(t_ressource_manager *rm)
{
	if ((rm->dp_header.file_size = get_file_size(rm->reader.fd)) == -1)
		return (FALSE);
	if (lseek(rm->reader.fd, 0, SEEK_SET) == -1)
		return (FALSE);
	ft_memcpy(rm->dp_header.magic, "DATAPACK", 8);
	dp_write(rm, &rm->dp_header, sizeof(t_wr_header));
	return (TRUE);
}

t_bool		save_datapack(t_doom *doom)
{
	if ((doom->res_manager.reader.fd = open(doom->res_manager.path,
		O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
		return (FALSE);
	if (lseek(doom->res_manager.reader.fd, sizeof(t_wr_header), SEEK_SET) == -1)
		return (FALSE);
	write_ressources(&doom->res_manager);
	write_map(&doom->res_manager);
	write_header(&doom->res_manager);
	close_datapack(doom);
	return (TRUE);
}
