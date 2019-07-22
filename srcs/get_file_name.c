/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file_name.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 16:46:07 by louali            #+#    #+#             */
/*   Updated: 2019/06/20 15:26:46 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_files		*copy_name(struct dirent *file, t_files *start, char *s2)
{
	start = malloc(sizeof(t_files) + (ft_strlen(s2)
	+ ft_strlen(file->d_name) + 1) * sizeof(char));
	ft_bzero(start->s, ft_strlen(s2) + ft_strlen(file->d_name));
	ft_strcat(start->s, s2);
	ft_strcat(start->s + ft_strlen(s2), file->d_name);
	start->next = NULL;
	return (start);
}

DIR			*open_dir(char *s)
{
	DIR		*rep;

	rep = opendir(s);
	if (!rep)
		exit(0);
	return (rep);
}

void		close_dir(DIR *rep)
{
	if (closedir(rep) == -1)
	{
		printf("close error\n");
		exit(-1);
	}
}

void		find_files(t_files **list, int *files_count, char *s, char *s2)
{
	DIR				*rep;
	struct dirent	*file;
	t_files			*start;
	t_files			*l;

	l = *list;
	rep = open_dir(s);
	while ((file = readdir(rep)) != NULL)
		if (ft_strcmp(file->d_name, ".") != 0
		&& ft_strcmp(file->d_name, "..") != 0)
		{
			start = copy_name(file, start, s2);
			if (!*list)
			{
				*list = start;
				l = *list;
			}
			else
			{
				l->next = start;
				l = l->next;
			}
			(*files_count)++;
		}
	close_dir(rep);
}
