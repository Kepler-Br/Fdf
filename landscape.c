/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   landscape.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 18:48:12 by nsena             #+#    #+#             */
/*   Updated: 2019/10/22 13:37:35 by hturkatr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static float	t_landscape_get(t_landscape *this, t_ivec2 point)
{
	int i;

	if (point.x >= this->dimensions.x || point.y >= this->dimensions.y)
		return (0);
	if (point.x < 0 || point.y < 0)
		return (0);
	i = point.x * this->dimensions.y + point.y;
	return (this->points[i]);
}

static void		t_landscape_calculate_min_max(t_landscape *this)
{
	const int	total_points = this->dimensions.x * this->dimensions.y;
	int			i;

	this->min = this->points[0];
	this->max = this->points[0];
	i = 0;
	while (i < total_points)
	{
		if (this->min > this->points[i])
			this->min = this->points[i];
		if (this->max < this->points[i])
			this->max = this->points[i];
		i++;
	}
	if (this->min == 0 && this->max == 0)
	{
		this->min = 0.0f;
		this->max = 1.0f;
	}
}

static void		t_landscape_normalize(t_landscape *this)
{
	const int	total_points = this->dimensions.x * this->dimensions.y;
	int			i;

	this->calculate_min_max(this);
	if (this->min == 0)
		return ;
	if (this->min < 0.0f)
	{
		i = 0;
		while (i < total_points)
		{
			this->points[i] += -this->min;
			i++;
		}
	}
	else if (this->min > 0.0f)
	{
		i = 0;
		while (i < total_points)
		{
			this->points[i] -= this->min;
			i++;
		}
	}
	this->calculate_min_max(this);
}

t_landscape		*make_t_landscape(t_ivec2 dimensions)
{
	t_landscape		*object;
	const int		point_count = dimensions.x * dimensions.y;

	if (!(object = (t_landscape*)malloc(sizeof(t_landscape))))
		return (NULL);
	if (!(object->points = (float*)malloc(sizeof(float) * point_count)))
	{
		free(object);
		return (NULL);
	}
	ft_bzero(object->points, point_count * sizeof(float));
	object->dimensions = dimensions;
	object->get = &t_landscape_get;
	object->calculate_min_max = &t_landscape_calculate_min_max;
	object->normalize = &t_landscape_normalize;
	make_t_landscape2(object);
	return (object);
}

void			destroy_t_landscape(t_landscape *object)
{
	free(object->points);
	free(object);
}
