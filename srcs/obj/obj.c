/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:28:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/24 14:39:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "obj.h"

t_bool		vertice_formatter(t_obj *obj, t_reader *reader)
{
	
}

t_bool		load_obj(t_obj *obj, char *file)
{
	t_reader		reader;
	t_obj_prefix	prefixes[2];

	prefixes[0] = (t_obj_prefix){ "#", vertice_formatter };
	prefixes[1] = (t_obj_prefix){ "v", vertice_formatter };
	ft_bzero(&reader, sizeof(t_reader));
	if ((reader.fd = open(file, O_RDONLY)) == -1)
		return (FALSE);
	skip_ws(&reader);
	if ()
	printf("%s\n", reader.buffer + reader.pos);
}

t_bool		obj_test()
{
	t_obj obj;
	load_obj(&obj, "assets/obj/cow-normals.obj");
	return (FALSE);
}
