/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_datapack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 22:14:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/12 17:28:40 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <libft.h>
#include "ressource.h"
#include "datapack.h"
#include "doom.h"
#include "write_structs.h"

void		dp_write(t_ressource_manager *rm, const void *buf, size_t n)
{
	if (!buf)
		return ;
	write(rm->reader.fd, buf, n);
	rm->dp_header.file_size += n;
}

t_bool		write_header(t_ressource_manager *rm)
{
	if (lseek(rm->reader.fd, 0, SEEK_SET) == -1)
		return (FALSE);
	dp_write(rm, &rm->dp_header, sizeof(t_wr_header));
	return (TRUE);
}



t_bool		write_ressource(t_ressource_manager *rm, t_ressource *r)
{
	const t_wr_ressource	wr_res = (t_wr_ressource){ 
		.type = r->type,
		.fixed = r->fixed,
		.loaded = r->loaded,
		.name_len = ft_strlen(r->display_name)
	};
	dp_write(rm, &wr_res, sizeof(t_wr_ressource));
	dp_write(rm, r->display_name, wr_res.name_len);
	if (!wr_res.loaded)
		return (TRUE);
	if (r->type == RESSOURCE_TEXTURE)
		return (write_texture(rm, r->data.texture));
	return (TRUE);
}

t_bool		write_ressources(t_ressource_manager *rm)
{
	int				i;
	t_ressource		*r;

	dp_write(rm, &rm->ressources->len, sizeof(int));
	i = -1;
	while (++i < rm->ressources->len)
	{
		r = rm->ressources->values[i];
		if (!write_ressource(rm, r))
			return (FALSE);
	}
	return (TRUE);
}

t_bool		save_datapack(t_doom *doom)
{
	if ((doom->res_manager.reader.fd = open(doom->res_manager.path, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
		return (FALSE);
	if (lseek(doom->res_manager.reader.fd, sizeof(t_wr_header), SEEK_SET) == -1)
		return (FALSE);
	write_ressources(&doom->res_manager);
	write_header(&doom->res_manager);
	close_datapack(doom);
	return (TRUE);
}
