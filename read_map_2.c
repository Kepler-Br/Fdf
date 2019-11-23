/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hturkatr <hturkatr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 12:51:46 by hturkatr          #+#    #+#             */
/*   Updated: 2019/10/20 12:52:30 by hturkatr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	init_data(t_data *data)
{
	data->x_size = 0;
	data->y_size = 0;
	data->map_points = ft_make_t_vector(10, sizeof(t_map_point));
}