/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   landscape_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hturkatr <hturkatr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 13:32:35 by hturkatr          #+#    #+#             */
/*   Updated: 2019/10/22 13:32:35 by hturkatr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void		t_landscape_set(t_landscape *this, t_ivec2 point, float num)
{
	int i;

	if (point.x >= this->dimensions.x || point.y >= this->dimensions.y)
		return ;
	if (point.x < 0 || point.y < 0)
		return ;
	i = point.x * this->dimensions.y + point.y;
	this->points[i] = num;
}

void make_t_landscape2(t_landscape *object)
{
	object->set = &t_landscape_set;
}