/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_datapack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 02:52:17 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/26 19:37:08 by llelievr         ###   ########.fr       */
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
	if (!io_memcpy(&rm->reader, &rm->dp_header, sizeof(t_wr_header)))
		return (FALSE);
	return (TRUE);
}

t_bool		read_texture(t_ressource_manager *rm, t_img **img)
{
	t_wr_img wr_img;

	if (!io_memcpy(&rm->reader, &wr_img, sizeof(t_wr_img)))
		return (FALSE);
	if (!(*img = malloc(sizeof(t_img))))
		return (FALSE);
	(*img)->ignore_texture = TRUE;
	if (!create_image(rm->doom->renderer, wr_img.width, wr_img.height, *img))
		return (FALSE);
	if ((*img)->size * sizeof(uint32_t) != wr_img.size)
		return (FALSE);
	if (!io_memcpy(&rm->reader, (*img)->pixels, wr_img.size))
		return (FALSE);
	return (TRUE);
}

t_bool		read_ressource(t_ressource_manager *rm, t_ressource *r)
{
	t_wr_ressource	wr_res;
	t_bool			result;

	if (!io_memcpy(&rm->reader, &wr_res, sizeof(t_wr_ressource)))
		return (FALSE);
	if (wr_res.name_len < 0 || wr_res.name_len > 255)
		return (FALSE);
	ft_bzero(r->display_name, 255);
	if (!io_memcpy(&rm->reader, &r->display_name, wr_res.name_len))
		return (FALSE);
	if (!r->fixed)
	{
		r->type = wr_res.type;
		r->fixed = wr_res.fixed;
	}
	if (!wr_res.loaded)
		return (TRUE);
	result = FALSE;
	if (r->type == RESSOURCE_TEXTURE)
		result = read_texture(rm, &r->data.texture);
	if (result)
		r->loaded = TRUE;
	return (TRUE);
}

t_bool		read_ressources(t_ressource_manager *rm)
{
	int				res_count;
	int				i;

	if (!io_memcpy(&rm->reader, &res_count, sizeof(int)))
		return (FALSE);
	if (res_count <= 0)
		return (FALSE);
	printf("RES COUNT %d %d\n", res_count, rm->ressources->len);
	i = -1;
	while (++i < res_count)
	{
		if (i >= rm->ressources->len)
			if (!append_ressources_array(&rm->ressources, ft_memalloc(sizeof(t_ressource))))
				return (FALSE);
		if(!read_ressource(rm, rm->ressources->values[i]))
			return (FALSE);
	}
	return (TRUE);
}

t_bool		load_datapack(t_doom *doom, char *path)
{
	struct stat	stats;

	doom->res_manager.doom = doom;
	doom->res_manager.reader = (t_reader) { .pos = 0 };
	doom->res_manager.path = path;
	if (stat(path, &stats) == -1)
	{
		printf("No file\n");
		return (TRUE);
	}
	if ((doom->res_manager.reader.fd = open(doom->res_manager.path, O_RDONLY, 0666)) == -1)
		return (FALSE);
	if (!read_header(&doom->res_manager))
		return (FALSE);
	if (!read_ressources(&doom->res_manager))
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
