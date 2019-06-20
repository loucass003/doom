/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gltf_version.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 14:09:17 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/20 14:23:54 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gltf.h"

t_bool		gltf_check_version(t_json_object *root)
{
	t_json_object	*asset;
	t_json_string	*version;
	const char		*version_str = "2.0";

	if (!(asset = json_get_object(root, "asset")))
		return (FALSE);
	if (!(version = json_get_string(asset, "version")))
		return (FALSE);
	return (version->value_len == (int)ft_strlen(version_str)
		&& ft_strncmp(version->value, version_str, version->value_len) == 0);
}
