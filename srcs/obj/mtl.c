/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 17:07:44 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/20 17:43:48 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "obj.h"

int				get_material(t_renderable *r, char *name, size_t len)
{
	int		i;

	if (!name)
		return (-1);
	i = 0;
	while (i < r->materials->len)
	{
		if (len == ft_strlen(r->materials->values[i].name)
			&& ft_strncmp(r->materials->values[i].name, name, len) == 0)
			return (i);
		i++;
	}
	return (-1);
}

static t_bool	mtl_process_formaters(t_obj *obj, t_reader *reader,
	t_renderable *r)
{
	t_obj_prefix	prefixes[MTL_PREFIXES_COUNT + 1];
	t_obj_prefix	*formatter;

	prefixes[0] = (t_obj_prefix){ "newmtl", mtl_newmtl_formatter };
	prefixes[1] = (t_obj_prefix){ "map_Kd", mtl_map_kd_formatter };
	prefixes[2] = (t_obj_prefix){ "Kd", mtl_kd_formatter };
	prefixes[MTL_PREFIXES_COUNT] = (t_obj_prefix){ NULL, NULL };
	while (!!(formatter = get_formatter(prefixes, MTL_PREFIXES_COUNT, reader)))
	{
		if (formatter->formatter && !formatter->formatter(obj, reader, r))
			return (free_obj(obj, FALSE));
		io_skip_until(reader, "\n");
		io_skip_empty(reader);
	}
	if (!formatter && io_peek(reader, NULL))
		return (FALSE);
	return (TRUE);
}

t_bool			mtllib_formatter(t_obj *obj, t_reader *reader, t_renderable *r)
{
	t_reader	mtl_reader;
	char		file[MATERIAL_FILE_LEN + 1];
	size_t		len;
	char		c;
	char		*path;

	if (!obj->can_add_materials)
	{
		ft_putendl("Can't load mtl after usemtl");
		return (FALSE);
	}
	io_next(reader);
	ft_bzero(file, sizeof(char) * (MATERIAL_FILE_LEN + 1));
	len = 0;
	while (io_peek(reader, &c) && c != '\n' && len < MATERIAL_FILE_LEN)
	{
		io_next(reader);
		file[len++] = c;
	}
	ft_bzero(&mtl_reader, sizeof(t_reader));
	if (!(path = path_join(obj->working_dir, file))
		|| (mtl_reader.fd = open(path, O_RDONLY)) == -1)
		return (FALSE);
	free(path);
	return (mtl_process_formaters(obj, &mtl_reader, r));
}

t_bool			usemtl_formatter(t_obj *obj, t_reader *reader, t_renderable *r)
{
	char		name[MATERIAL_NAME_LEN];
	size_t		len;
	char		c;

	obj->can_add_materials = FALSE;
	io_next(reader);
	len = 0;
	while (io_peek(reader, &c) && c != '\n' && len < MATERIAL_NAME_LEN)
	{
		io_next(reader);
		name[len++] = c;
	}
	if ((obj->current_mtl = get_material(r, name, len)) == -1)
	{
		ft_putstr("usemtl: Unknown material name : ");
		write(1, name, len);
		write(1, "\n", 1);
		return (FALSE);
	}
	return (TRUE);
}
