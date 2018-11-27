/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 16:11:49 by afeuerst          #+#    #+#             */
/*   Updated: 2018/11/24 11:43:58 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static inline void			*ft_strjoin(char *s1, char *s2,
		ssize_t l1, ssize_t l2)
{
	char					*new;
	char					*ptr;

	if (!(new = malloc(l1 + l2 + 1)))
		return (NULL);
	ptr = new;
	if (s1)
	{
		while (l1--)
			*new++ = *s1++;
		free(s1 - (new - ptr));
	}
	while (l2--)
		*new++ = *s2++;
	*new = 0;
	return (ptr);
}

static struct s_element		*get_element(const int fd,
		char *befor, char *const buffer)
{
	struct s_element *const	new = malloc(sizeof(struct s_element));

	if (!(new->next = NULL) && (!new || fd < 0))
		return (NULL);
	new->fd = fd;
	new->total = 0;
	while ((new->read = read(fd, buffer, BUFF_SIZE)) > 0)
	{
		if (!(buffer[(int)new->read] = 0) && !befor)
		{
			if (!(new->file = ft_strjoin(NULL, buffer, 0, new->read)))
				return (NULL);
			befor = new->file;
		}
		else
		{
			if (!(new->file = ft_strjoin(befor, buffer, new->total, new->read)))
				return (NULL);
			befor = new->file;
		}
		new->total += new->read;
	}
	if (new->read == -1 || (new->read = 0))
		free(new);
	return ((new->read == 0 ? new : NULL));
}

static struct s_element		*get_current_element(const int fd,
		struct s_element **const elements)
{
	struct s_element		*ptr;
	char					buffer[BUFF_SIZE + 1];

	if (!*elements)
	{
		if (!(*elements = get_element(fd, NULL, buffer)))
			return (NULL);
		ptr = *elements;
	}
	else
	{
		ptr = *elements;
		while (1)
		{
			if (ptr->fd == fd)
				return (ptr);
			else if (ptr->next && (ptr = ptr->next))
				continue ;
			if (!(ptr->next = get_element(fd, NULL, buffer)))
				return (NULL);
		}
	}
	return (ptr);
}

static inline void			destroy_element(const int fd,
		struct s_element **const elements)
{
	struct s_element		*ptr;
	struct s_element		*befor;

	befor = NULL;
	if (!(ptr = *elements))
		return ;
	if (ptr->fd == fd)
	{
		free(ptr->file);
		*elements = ptr->next;
		return (free(ptr));
	}
	while (ptr)
	{
		if (ptr->fd == fd)
		{
			free(ptr->file);
			if (befor)
				befor->next = ptr->next;
			free(ptr);
			break ;
		}
		befor = ptr;
		ptr = ptr->next;
	}
}

int							get_next_line(const int fd, char **line)
{
	static struct s_element	*elements = NULL;
	struct s_element		*element;

	if (!(element = get_current_element(fd, &elements)))
		return (GNL_ERROR);
	if (element->read == element->total)
	{
		destroy_element(fd, &elements);
		*line = NULL;
		return (GNL_END);
	}
	*line = element->file + element->read;
	while (element->read < element->total &&
			element->file[element->read] != '\n')
		element->read++;
	while (element->read < element->total &&
			element->file[element->read] == '\n')
		element->file[element->read++] = 0;
	return (GNL_READED);
}
