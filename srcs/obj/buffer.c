/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:15:03 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/20 16:29:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj.h"

t_bool		create_str_buffer(t_string_buffer *buffer, size_t capacity)
{
	ft_bzero(buffer, sizeof(t_string_buffer));
	buffer->capacity = capacity;
	if (!(buffer->content = (char *)malloc(sizeof(char) * capacity)))
		return (FALSE);
}

t_bool		append_str_buffer(t_string_buffer *buffer, char c)
{
	size_t	new_capacity;
	char	*new_content;

	if (buffer->len == buffer->capacity)
	{
		new_capacity = buffer->capacity * 2 * sizeof(char);
		if (!(new_content = (char *)ft_realloc(buffer->content,
			buffer->capacity * sizeof(char), new_capacity)))
			return (FALSE);
		buffer->content = new_content;
		buffer->capacity = new_capacity;
	}
	buffer->content[buffer->len++] = c;
}
