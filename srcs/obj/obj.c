/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:28:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/20 16:34:12 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "obj.h"

t_bool		load_obj(t_obj *obj, char *file)
{
	t_reader	reader;

	ft_bzero(&reader, sizeof(t_reader));
	if ((reader.fd = open(file, O_RDONLY)) == -1)
		return (FALSE);
	
}

t_bool		obj_test()
{
	return (FALSE);
}
