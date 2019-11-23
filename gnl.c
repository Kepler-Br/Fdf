/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 15:06:30 by nsena             #+#    #+#             */
/*   Updated: 2019/09/25 14:15:20 by nsena            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include "gnl.h"

static t_map_item	*find_or_create(t_map *fd_map, const int fd)
{
	t_map_item		*found;
	char			*buff;

	if (!(found = fd_map->find(fd_map, (int *)&fd, sizeof(int))))
	{
		buff = ft_strnew(BUFF_SIZE + 1);
		fd_map->emplace(fd_map,
						(t_map_item) {.key=(int *)&fd, .key_size=sizeof(int),
								.value=buff, .value_size=BUFF_SIZE + 1});
		free(buff);
		if (!(found = fd_map->find(fd_map, (int *)&fd, sizeof(int))))
			return (NULL);
	}
	return (found);
}

static int			i_love_the_norm1(char **line, t_map_item *found)
{
	char			*buff;
	char			*buff3;
	char			*buff4;

	buff = ft_strchr((char *)found->value, '\n');
	if (buff == NULL)
	{
		buff4 = (*line);
		(*line) = ft_strjoin((*line), (char *)found->value);
		free(buff4);
		((char *)found->value)[0] = '\0';
		return (1);
	}
	else
	{
		buff3 = ft_strsub((char *)found->value, 0,
						  (int)(buff - (char *)found->value));
		buff4 = (*line);
		(*line) = ft_strjoin((*line), buff3);
		free(buff4);
		free(buff3);
		found->value = ft_memmove(found->value, buff + 1,
								  (int)(ft_strchr((char *)found->value, '\0') - (char *)found->value));
		return (0);
	}
}

static int			i_love_the_norm2(t_map_item *found, int fd, int *red)
{
	if (ft_strlen((char *)found->value) == 0)
	{
		*red = read(fd, found->value, BUFF_SIZE);
		if (*red == -1)
			return (-1);
		if (*red == 0)
			return (1);
		((char *)found->value)[*red] = '\0';
	}
	return (0);
}

static int			i_love_the_norm3(int fd, char **line, t_map **fd_map,
									   t_map_item **found)
{
	if (fd < 0 || !line || BUFF_SIZE < 1 || fd == 1 || fd == 2)
		return (-1);
	if (*fd_map == NULL)
		*fd_map = ft_make_t_map();
	if (!((*found) = find_or_create(*fd_map, fd)))
		return (-1);
	(*line) = ft_strnew(1);
	return (0);
}

int					gnl(const int fd, char **line)
{
	static t_map	*fd_map;
	t_map_item		*found;
	int				red;
	int				buff;

	red = 1;
	if (i_love_the_norm3(fd, line, &fd_map, &found) == -1)
		return (-1);
	while (1)
	{
		if ((buff = i_love_the_norm2(found, fd, &red)) == 1)
			break ;
		else if (buff == -1)
			return (-1);
		if (i_love_the_norm1(line, found) == 1)
			continue ;
		else
			break ;
	}
	if (ft_strlen((*line)) == 0 && red == 0)
	{
		return (0);
	}
	return (1);
}
