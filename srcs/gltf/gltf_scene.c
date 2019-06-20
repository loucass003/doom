/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gltf_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 14:28:49 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/20 15:13:52 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gltf.h"

t_bool		gltf_load_scene(t_json_object *scene, t_scene *out)
{
	
}

t_bool		gltf_load_scenes(t_gltf *gltf, t_json_object *root)
{
	t_json_number	*scene;
	t_json_array	*scenes;
	t_json_element	*elem;
	size_t			i;

	if (!!(scene = json_get_number(root, "scene")) && (gltf->scene_set = TRUE))
		gltf->scene = scene->value;
	if (!(gltf->scenes = (t_scene *)ft_memalloc(
		sizeof(t_scene) * scenes->elems_count)))
		return (FALSE);
	elem = scenes->elements;
	i = 0;
	while (elem)
	{
		if (elem->value->type != JSON_OBJECT 
			|| !gltf_load_scene((t_json_object *)elem->value, 
				gltf->scenes + i++))
			return (FALSE); //TODO: possible free
		elem = elem->next;
	}
	return (TRUE);
}
