/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_ressource.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 01:51:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/13 01:51:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"

t_bool		load_ressource(t_doom *doom, t_ressource *r, char *path)
{
	if (r->type == RESSOURCE_TEXTURE)
		return (load_texture(doom, r, path));
	else
		return (FALSE);
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