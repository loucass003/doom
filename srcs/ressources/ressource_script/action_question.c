/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_question.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/18 00:50:43 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 00:58:35 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "script.h"

t_bool					question_choice(t_action_question *question,
	t_json_array *array)
{
	t_json_string	*string;
	t_json_element	*element;

	element = array->elements;
	while (element)
	{
		if (!(string = json_to_string(element->value)))
			return (script_return_error("Invalid question type(NOT A STRING)"));
		if (!(question->quest[question->quest_count] = ft_memalloc(sizeof(char)
			* (string->value_len + 1))))
			return (script_return_error("Unnable to alloc strings"));
		ft_memcpy(question->quest[question->quest_count], string->value,
			string->value_len);
		question->quest_count++;
		element = element->next;
	}
	return (TRUE);
}

t_bool					question_answer(t_action_question *question,
	t_json_array *array)
{
	t_json_string	*string;
	t_json_element	*element;

	element = array->elements;
	while (element)
	{
		if (!(string = json_to_string(element->value)))
			return (script_return_error("Invalid question type(NOT A STRING)"));
		if (!(question->answer[question->answer_count] = ft_memalloc(
			sizeof(char) * (string->value_len + 1))))
			return (script_return_error("Unnable to alloc strings"));
		ft_memcpy(question->answer[question->answer_count], string->value,
			string->value_len);
		question->answer_count++;
		element = element->next;
	}
	return (TRUE);
}

t_bool					parse_action_question(t_action_question *question,
	t_json_object *object)
{
	t_json_array	*array;

	if (!(array = json_get_array(object, "choice")))
		return (script_return_error("No text element or invalid text element"));
	if (!(question->quest = ft_memalloc(array->elems_count * sizeof(char *))))
		return (script_return_error("Unnable to alloc strings"));
	if (!question_choice(question, array))
		return (FALSE);
	if (!(array = json_get_array(object, "answer choice")))
		return (script_return_error("No text element or invalid text element"));
	if (!(question->answer = ft_memalloc(array->elems_count * sizeof(char *))))
		return (script_return_error("Unnable to alloc strings"));
	if (!question_answer(question, array))
		return (FALSE);
	return (TRUE);
}
