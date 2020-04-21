/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   json_null.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/16 22:59:13 by llelievr          #+#    #+#             */
/*   Updated: 2019/03/01 15:36:38 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_json_value	*json_parse_null(t_json_state *state)
{
	t_json_value	*nil;

	if (!(nil = (t_json_value *)malloc(sizeof(t_json_value))))
		return (NULL);
	state->pos += 4;
	nil->type = JSON_NULL;
	return (nil);
}
