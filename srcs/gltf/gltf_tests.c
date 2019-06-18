/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gltf_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/18 11:48:44 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/18 14:27:36 by llelievr         ###   ########.fr       */
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
		content = ft_realloc(content, *content_len, *content_len + rd);
		ft_memcpy(content + *content_len, buf, rd);
		*content_len += rd;
	}
	if (rd != -1)
		return (content);
	if (content)
		free(content);
	return (NULL);
}

t_bool	gltf_test()
{
	t_json_state	state;
	t_json_value	*val;
	int				content_len;
	const char		*content = get_file_content("assets/gltf/BrainStem.gltf", &content_len);

	state = (t_json_state){(char *)content, 0, content_len};
	if (!content || !(val = parse_value(&state)))
	{
		ft_putendl("Invalid file");
		if (content)
			free((void *)content);
		return (FALSE);
	}
	free((void *)content);
	t_json_array *obj = json_get_array((t_json_object *)val, "nodes");
	printf("%d\n", obj->elems_count);
	t_json_element *elem = obj->elements;
	while (elem)
	{
		t_json_array *rot = json_get_array((t_json_object *)elem->value, "rotation");
		if (rot)
		{
			double lol = ((t_json_number *)rot->elements->value)->value;
			printf("%lf\n", lol);
		}
		elem = elem->next;
	}
	return (TRUE);
}
