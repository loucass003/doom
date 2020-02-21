/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_ressource.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 01:51:49 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/21 17:08:29 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "ressource.h"
#include "write_structs.h"
#include "script.h"

t_bool		load_ressource(t_doom *doom, t_ressource *r, char *path)
{
	if (r->type == RESSOURCE_TEXTURE)
		return (load_texture_file(doom, r, path));
	else if (r->type == RESSOURCE_MODEL)
		return (load_model_file(doom, r, path));
	else if (r->type == RESSOURCE_SOUND)
		return (load_sound(r, path));
	else if (r->type == RESSOURCE_SCRIPT)
		return (load_script(r, path));
	else
		return (FALSE);
}

t_bool		result_ress(t_bool result, t_ressource_manager *rm, t_ressource *r)
{
	if (r->type == RESSOURCE_TEXTURE)
		result = read_texture(rm, &r->data.texture);
	else if (r->type == RESSOURCE_MODEL)
		result = read_model(rm, &r->data.model);
	else if (r->type == RESSOURCE_SOUND)
		result = read_songs(rm, &r->data.sound);
	else if (r->type == RESSOURCE_SCRIPT)
		result = read_script(rm, &r->data.script_data);
	return (result);
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
	result = result_ress(result, rm, r);
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
			if (!append_ressources_array(&rm->ressources,
				ft_memalloc(sizeof(t_ressource))))
				return (FALSE);
		if (!read_ressource(rm, rm->ressources->values[i]))
			return (FALSE);
	}
	return (TRUE);
}
