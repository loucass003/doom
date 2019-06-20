/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gltf_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/18 11:48:44 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/20 15:06:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include <fcntl.h>
#include <unistd.h>
#include "gltf.h"

#define BUFF_SIZE 2048

static char			*get_file_content(char *file, int *content_len)
{
	int		fd;
	char	*content;
	char	buf[BUFF_SIZE + 1];
	int		rd;

	if ((fd = open(file, O_RDONLY)) == -1)
		return (NULL);
	*content_len = 0;
	content = NULL;
	while ((rd = read(fd, &buf, BUFF_SIZE)) > 0)
	{
		buf[rd] = 0;
		if (!(content = ft_realloc(content, *content_len, *content_len + rd)))
			return (NULL);
		ft_memcpy(content + *content_len, buf, rd);
		*content_len += rd;
	}
	if (rd != -1)
		return (content);
	if (content)
		free(content);
	return (NULL);
}

t_json_object	*gltf_json_root(char *file)
{
	t_json_state	state;
	t_json_value	*val;
	int				content_len;
	const char		*content = get_file_content(file, &content_len);

	state = (t_json_state){(char *)content, 0, content_len};
	if (!content || !(val = parse_value(&state)))
	{
		ft_putendl("Invalid file (JSON parsing error)");
		if (content)
			free((void *)content);
		return (NULL);
	}
	free((void *)content);
	if (val->type != JSON_OBJECT)
		return (NULL);
	return ((t_json_object *)val);
}

t_bool	load_gltf(t_gltf *gltf, char *file)
{
	t_json_object	*root;

	if (!(root = gltf_json_root(file)) || !gltf_check_version(root))
		return (FALSE);
	ft_bzero(gltf, sizeof(t_gltf));
	gltf_load_scenes(gltf, root);
	return (TRUE);
}

t_bool	gltf_test(void)
{
	t_gltf gltf;
	if (!load_gltf(&gltf, "assets/gltf/BrainStem.gltf"))
	{
		ft_putendl("Invalid gltf");
		return (FALSE);
	}
	
	return (TRUE);
}
