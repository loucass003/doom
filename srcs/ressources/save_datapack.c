/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_datapack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 22:14:40 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/25 07:08:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "ressource.h"
#include "datapack.h"
#include "doom.h"
#include "write_structs.h"

t_bool		write_header(t_doom *doom)
{
	return (TRUE);
}

t_bool		write_texture(t_ressource_manager *rm, t_img *img)
{
	const t_wr_img	wr_img = (t_wr_img){ 
		.size = img->size * sizeof(uint32_t),
		.width = img->width,
		.height = img->height
	};
	write(rm->fd, &wr_img, sizeof(t_wr_img));
	write(rm->fd, img->pixels, wr_img.size);
	return (TRUE);
}

t_bool		write_ressource(t_ressource_manager *rm, t_ressource *r)
{
	const t_wr_ressource	wr_res = (t_wr_ressource){ 
		.type = r->type,
		.fixed = r->fixed,
		.name_len = ft_strlen(r->display_name)
	};
	write(rm->fd, &wr_res, sizeof(t_wr_ressource));
	write(rm->fd, r->display_name, wr_res.name_len);
	if (r->type == RESSOURCE_TEXTURE)
		return (write_texture(rm, r->data.texture));
	return (TRUE);
}

t_bool		write_ressources(t_ressource_manager *rm)
{
	int				i;
	t_ressource		*r;

	write(rm->fd, rm->ressources->len, sizeof(int));
	i = -1;
	while (++i < rm->ressources->len)
	{
		r = rm->ressources->values[i];
		if (r->used && !write_ressource(rm, r))
			return (FALSE);
	}
	return (TRUE);
}

t_bool		save_datapack(t_doom *doom)
{
	write_header(doom);
	write_ressources(&doom->res_manager);
	close_datapack(doom);
	return (TRUE);
}