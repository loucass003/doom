/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trigger_pick_item.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 13:25:50 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/11 16:10:10 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"
#include "ressource.h"
#include "file_utils.h"
#include "item.h"

static t_weapon_type	get_weapon_type(char *str)
{
	const int	len = ft_strlen(str);

	if (len == 3 && ft_strncmp(str, "GUN", 3) == 0)
		return (WEAPON_GUN);
	if (len == 7 && ft_strncmp(str, "GRENADA", 7) == 0)
		return (WEAPON_GRENADA);
	if (len == 3 && ft_strncmp(str, "AXE", 3) == 0)
		return (WEAPON_AXE);
	return (WEAPON_NONE);
}

static t_item_type	get_item_type(char *str)
{
	const int	len = ft_strlen(str);

	if (len == 4 && ft_strncmp(str, "AMMO", 4) == 0)
		return (ITEM_AMMO);
	if (len == 6 && ft_strncmp(str, "WEAPON", 6) == 0)
		return (ITEM_WEAPON);
	if (len == 4 && ft_strncmp(str, "HEAL", 4) == 0)
		return (ITEM_HEAL);
	if (len == 3 && ft_strncmp(str, "KEY", 3) == 0)
		return (ITEM_KEY);
	if (len == 7 && ft_strncmp(str, "JETPACK", 7) == 0)
		return (ITEM_JETPACK);
	return (ITEM_UNKNOWN);
}

t_bool          trigger_item_weapon(t_trigger_pick_item *trigger, t_json_object *object)
{
    t_json_string	*j_string;
    
    if (!(j_string = json_get_string(object, "weapon")))
        return (FALSE);
    trigger->weapon_type = get_weapon_type(j_string->value);
    if (trigger->weapon_type == WEAPON_NONE)
        return(script_return_error(j_string->value));
    trigger->is_weapon_set = TRUE;
    return (TRUE);
}

t_bool          trigger_pick_item(t_trigger_pick_item *trigger, t_json_object *object)
{
    t_json_string	*j_string;
    
    if (!object)
        return (FALSE);
    if (!(j_string = json_get_string(object, "item_type")))
        return (FALSE);
    trigger->item_type = get_item_type(j_string->value);
    if (trigger->item_type == ITEM_UNKNOWN)
        return(script_return_error(j_string->value));
    trigger->is_weapon_set = FALSE;
    if (trigger->item_type == ITEM_WEAPON)
        trigger_item_weapon(trigger, object);       
    return (TRUE);
}