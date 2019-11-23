/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hturkatr <hturkatr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 12:37:51 by hturkatr          #+#    #+#             */
/*   Updated: 2019/10/20 15:54:14 by hturkatr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

int				process_line(char **points, t_data *data, t_ivec2 *coord)
{
	INIT_MAP_POINT(map_point);
	while (points[coord->x])
	{
		process_points(points[coord->x], map_point, data, coord);
		coord->x++;
	}
	if (data->x_size == 0)
		data->x_size = coord->x;
	else
	{
		if (data->x_size != coord->x)
			return (-1);
	}
	return (0);
}

t_landscape		*process_input(char *filename)
{
	int			fd;
	t_data		data;
	t_landscape	*land;

	INIT_COUNTER(i);
	fd = open(filename, O_RDONLY);
	if ((read_map(&data, fd) == -1))
	{
		ft_puterr("Error during map reading. Exiting...");
		exit(0);
	}
	land = make_t_landscape((t_ivec2){data.x_size, data.y_size});
	while ((size_t)i < data.map_points->last_item_index)
	{
		land->set(land, ((t_map_point*)(data.map_points->at(data.map_points,
															i)))->xy,
				  ((t_map_point*)(data.map_points->at(data.map_points, i)))->z);
		i++;
	}
	ft_destroy_t_vector(data.map_points);
	land->normalize(land);
	return (land);
}

int				process_points(char *points, t_map_point map_point,
								t_data *data, t_ivec2 *coord)
{
	char	**temp;

	temp = ft_strsplit(points, ',');
	map_point.color = get_color(temp[1]);
	map_point = (t_map_point){(t_ivec2){coord->x,
										coord->y}, (float)ft_atoi(temp[0]), 0};
	data->map_points->push(data->map_points, &map_point);
	free_str_arr(temp);
	return (0);
}