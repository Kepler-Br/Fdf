/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_make_t_vector2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hturkatr <hturkatr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 13:59:45 by hturkatr          #+#    #+#             */
/*   Updated: 2019/10/02 16:31:40 by hturkatr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static size_t			t_vector_size(t_vector *this)
{
	return (this->last_item_index);
}

static void				t_vector_erase(t_vector *this)
{
	this->last_item_index = 0;
	free(this->data);
	this->data = malloc(0);
	this->capacity = 0;
}

void					ft_make_t_vector2(t_vector *object)
{
	object->erase = &t_vector_erase;
	object->size = &t_vector_size;
}


