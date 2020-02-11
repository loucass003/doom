/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_datapack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 02:52:17 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/11 04:20:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <libft.h>
#include <unistd.h>
#include <sys/stat.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"

t_bool		read_header(t_ressource_manager *rm)
{
	ssize_t	len;

	if ((len = get_file_size(rm->reader.fd)) == -1)
		return (FALSE);
	if (!io_memcpy(&rm->reader, &rm->dp_header, sizeof(t_wr_header)))
		return (FALSE);
	if (ft_memcmp(rm->dp_header.magic, "DATAPACK", 8) != 0)
		return (FALSE);
	if (len != rm->dp_header.file_size)
		return (FALSE);
	return (TRUE);
}

t_bool		load_datapack(t_doom *doom, char *path)
{
	struct stat	stats;

	doom->res_manager.doom = doom;
	doom->res_manager.reader = (t_reader) { .pos = 0 };
	doom->res_manager.path = path;
	if (stat(path, &stats) == -1)
		return (TRUE);
	if ((doom->res_manager.reader.fd = open(doom->res_manager.path, O_RDONLY, 0666)) == -1)
		return (FALSE);
	if (!read_header(&doom->res_manager))
		return (FALSE);
	if (!read_ressources(&doom->res_manager))
		return (FALSE);
	if (!read_map(&doom->res_manager))
		return (FALSE);
	close_datapack(doom);
	return (TRUE);
}

t_bool		close_datapack(t_doom *doom)
{
	if (doom->res_manager.reader.fd == -1)
		return (FALSE);
	return (close(doom->res_manager.reader.fd) != -1);
}
