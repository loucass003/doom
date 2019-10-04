/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_post_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 16:26:43 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/04 17:33:57 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "collision.h"
#include "maths/triangle.h"
#include "render.h"
#include "octree.h"

t_bool			post_process_obj(t_renderable *r)
{
	int			i;
	t_face		*face;


	i = -1;
	while (++i < r->faces->len)
	{
		face = &r->faces->values[i];
		face->collidable = face_collidable(r, i, r->vertices->vertices);
	}
	r->octree = create_octree(r);
	printf("%p %d\n", r->octree, r->faces->len);
	print_octree(r->octree);
	transform_renderable(r);
	
	return (TRUE);
}
