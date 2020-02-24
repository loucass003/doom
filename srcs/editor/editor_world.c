/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_world.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 16:03:32 by louali            #+#    #+#             */
/*   Updated: 2020/02/20 17:06:47 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "doom.h"
#include "editor.h"
#include "render.h"
#include "octree.h"
#include "sprite.h"
#include "ellipsoid.h"
#include "door.h"

t_vec3		editor_to_world(t_vec3 pos)
{
	float	ratio;

	ratio = 1. / 5.;
	pos.x *= ratio;
	pos.z *= ratio;
	return (pos);
}

t_vec3		world_to_editor(t_vec3 pos)
{
	float	ratio;

	ratio = 5;
	pos.x *= ratio;
	pos.z *= ratio;
	return (pos);
}
