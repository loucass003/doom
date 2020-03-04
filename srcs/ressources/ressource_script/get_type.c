/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 17:09:40 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/04 17:10:45 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "ressource.h"
#include "file_utils.h"
#include "item.h"

t_trigger_type			get_trigger_type(char *str)
{
	const int	len = ft_strlen(str);

	if (len == 5 && ft_strncmp(str, "SPAWN", 5) == 0)
		return (TRIG_SPAWN);
	if (len == 9 && ft_strncmp(str, "PICK_ITEM", 9) == 0)
		return (TRIG_PICK_ITEM);
	if (len == 4 && ft_strncmp(str, "AREA", 4) == 0)
		return (TRIG_AREA);
	return (TRIG_NONE);
}

t_action_type			get_action_type(char *str)
{
	const int	len = ft_strlen(str);

	if (len == 7 && ft_strncmp(str, "MESSAGE", 7) == 0)
		return (ACTION_MESSAGE);
	if (len == 8 && ft_strncmp(str, "TELEPORT", 8) == 0)
		return (ACTION_TELEPORT);
	if (len == 8 && ft_strncmp(str, "QUESTION", 8) == 0)
		return (ACTION_QUESTION);
	return (ACTION_NONE);
}
