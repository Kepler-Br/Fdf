/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 18:48:53 by nsena             #+#    #+#             */
/*   Updated: 2019/10/24 17:53:32 by nsena            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void		t_image_set_pixel(t_image *this, t_ivec2 point, int color)
{
	int	i;

	if (point.x >= this->geometry.x || point.x < 0 ||
		point.y >= this->geometry.y || point.y < 0)
		return ;
	i = (point.x * this->bpp / 8) + (point.y * this->stride);
	this->ptr[i] = (char)color;
	this->ptr[++i] = (char)(color >> 8);
	this->ptr[++i] = (char)(color >> 16);
}

static int		t_image_get_pixel(t_image *this, t_ivec2 point)
{
	int i;
	int color;

	if (point.x > this->geometry.x || point.x < 0.0f ||
		point.y > this->geometry.y || point.y < 0.0f)
		return (0);
	color = 0;
	i = (point.x * this->bpp / 8) + (point.y * this->stride);
	color = *(int *)&this->ptr[i];
//	char pixel_color = this->ptr[i];
//	color = color | pixel_color;
//	color <<= 8;
//	color |= (char)this->ptr[++i];
//	color <<= 8;
//	color |= (char)this->ptr[++i];
//	color <<= 8;
//	color <<= 8;
	return (color);
}

/*
**Comment from t_image_clear
**ft_memset(this->ptr, 0xAF, this->geometry.x*this->geometry.y*(this->bpp/8));
**/

static void		t_image_clear(t_image *this)
{
	ft_bzero(this->ptr, (this->geometry.x *
						this->geometry.y * (this->bpp / 8)));
//ft_memset(this->ptr, 0xff, (this->geometry.x *
//						this->geometry.y * (this->bpp / 8)));
}

t_image			*make_t_image(void *mlx, t_ivec2 geometry)
{
	t_image		*object;

	if (!(object = malloc(sizeof(t_image))))
		return (NULL);
	object->geometry = geometry;
	object->get_pixel = &t_image_get_pixel;
	object->set_pixel = &t_image_set_pixel;
	object->clear = &t_image_clear;
	object->image = mlx_new_image(mlx, (int)geometry.x, (int)geometry.y);
	if (!object->image)
	{
		free(object);
		return (NULL);
	}
	object->ptr = mlx_get_data_addr(object->image, &(object->bpp),
									&(object->stride), &(object->endian));
	return (object);
}

void			destroy_t_image(t_image *object, void *mlx)
{
	mlx_destroy_image(mlx, object->image);
	free(object);
}
