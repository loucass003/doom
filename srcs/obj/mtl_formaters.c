/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtl_formaters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/28 03:24:11 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/16 22:59:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <SDL_image.h>
#include "obj.h"
#include "image.h"
#include "doom.h"

t_bool			mtl_newmtl_formatter(t_obj *obj, t_reader *reader, t_renderable *r)
{
	t_mtl		mtl;
	char		name[MATERIAL_NAME_LEN];
	size_t		len;
	char		c;

	len = 0;
	io_next(reader);
	while (io_peek(reader, &c) && c != '\n' && len < GROUPS_NAME_LEN)
	{
		io_next(reader);
		name[len++] = c;
	}
	if (c == '\n' && len == MATERIAL_NAME_LEN)
	{
		ft_putendl("newmtl: Max material name length exeed");
		return (FALSE);
	}
	if ((obj->current_mtl = get_material(r, name, len)) != -1)
		return (TRUE);
	ft_bzero(&mtl, sizeof(t_mtl));
	ft_strncpy(mtl.name, name, len);
	if (!append_mtllist(&r->materials, mtl))
		return (FALSE);
	obj->current_mtl = r->materials->len - 1;
	return (TRUE);
}

t_bool	load_texture(char *path, t_mtl *mtl)
{
	if (!path)
		return (FALSE);
	if (!(mtl->texture_map = surface_to_image(NULL, SDL_ConvertSurfaceFormat(IMG_Load(path), SDL_PIXELFORMAT_ARGB8888, 0))))
	{
		free(path);
		ft_putstr("Unable to load material texture: ");
		ft_putendl(SDL_GetError());
		return (FALSE);
	}
	free(path);
//	mtl->texture_map = surface_to_image(NULL, SDL_ConvertSurfaceFormat(mtl->texture_map, SDL_PIXELFORMAT_ARGB8888, 0));
	mtl->texture_map_set = TRUE;
	return (TRUE);
} 

t_bool			mtl_map_kd_formatter(t_obj *o, t_reader *reader, t_renderable *r)
{
	char		name[MATERIAL_TEXTURE_LEN + 1];
	size_t		l;
	char		c;
	char		*p;

	if (o->current_mtl == -1)
	{
		ft_putendl("map_kd: no material set");
		return (FALSE);
	}
	l = 0;
	io_next(reader);
	ft_bzero(name, sizeof(char) * (MATERIAL_TEXTURE_LEN + 1));
	while (io_peek(reader, &c) && c != '\n' && l < MATERIAL_TEXTURE_LEN)
	{
		io_next(reader);
		name[l++] = c;
	}
	if (c == '\n' && l == MATERIAL_TEXTURE_LEN)
	{
		ft_putendl("map_kd: Max texture name length exeed");
		return (FALSE);
	}
	return ((p = path_join(o->working_dir, name)) && 
		load_texture(p, &r->materials->values[o->current_mtl]));
}

t_bool			mtl_kd_formatter(t_obj *o, t_reader *reader, t_renderable *r)
{
	char		c;
	t_vec3_u	color;
	t_mtl		*current;
	int			i;
	
	i = 0;
	while (io_peek(reader, &c) && c == ' ')
	{
		io_next(reader);
		if(!io_readfloat(reader, &color.a[i]))
			return (FALSE);
		if (++i > 3)
			return (FALSE);
	}
	if (i != 3)
		return (FALSE);
	current = &r->materials->values[o->current_mtl];
	current->material_color_set = TRUE;
	current->material_color = ft_color_i(ft_color(color.v.x * 255, 
		color.v.y * 255, color.v.z * 255));
	return (TRUE);
}