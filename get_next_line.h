/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afeuerst <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 16:11:03 by afeuerst          #+#    #+#             */
/*   Updated: 2018/11/24 11:02:41 by afeuerst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# define GNL_ERROR -1
# define GNL_END 0
# define GNL_READED 1

# define BUFF_SIZE 4096

int						get_next_line(const int fd, char **line);

struct					s_element
{
	int					fd;
	int					padding;
	char				*file;
	ssize_t				read;
	ssize_t				total;
	struct s_element	*next;
};

#endif
