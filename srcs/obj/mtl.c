/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mtl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 17:07:44 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/28 03:23:38 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "obj.h"

static t_bool	mtl_process_formaters(t_obj *obj, t_reader *reader)
{
	t_obj_prefix	prefixes[MTL_PREFIXES_COUNT + 1];
	t_obj_prefix	*formatter;
	size_t			i;

	prefixes[0] = (t_obj_prefix){ "newmtl", mtl_newmtl_formatter };
	prefixes[1] = (t_obj_prefix){ "map_Kd", mtl_map_kd_formatter };
	prefixes[MTL_PREFIXES_COUNT] = (t_obj_prefix){ NULL, NULL };
	i = 0;
	return (TRUE);
}

t_bool			mtllib_formatter(t_obj *obj, t_reader *reader)
{
	t_reader	mtl_reader;
	char		file[MATERIAL_FILE_LEN];
	size_t		len;
	char		c;
	char		*path;
	
	io_next(reader);
	ft_bzero(file, sizeof(char) * MATERIAL_FILE_LEN);
	len = 0;
	while ((c = io_peek(reader)) != -1 && c != '\n' && len < MATERIAL_FILE_LEN)
	{
		io_next(reader);
		file[len++] = c;
	}
	ft_bzero(&mtl_reader, sizeof(t_reader));
	if (!(path = path_join(obj->working_dir, file)) 
		|| (mtl_reader.fd = open(path, O_RDONLY)) == -1)
		return (FALSE);
	free(path);
	return (mtl_process_formaters(obj, &mtl_reader));
}

t_bool			usemtl_formatter(t_obj *obj, t_reader *reader)
{
	obj->can_add_materials = FALSE;
}