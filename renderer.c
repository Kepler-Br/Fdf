/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 18:50:12 by nsena             #+#    #+#             */
/*   Updated: 2019/10/21 18:50:16 by nsena            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void t_renderer_plot_line(t_renderer *this, t_vec3 one, t_vec3 two, int color)
{
	(void)this;
	(void)one;
	(void)two;
	(void)color;
//	const int dx = (int)(two.x - one.x);
//	const int dy = (int)(two.y - one.y);
//
//	// calculate steps required for generating pixels
//	const int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
//
//	// calculate increment in x & y for each steps
//	const float Xinc = (float)dx / (float) steps;
//	const float Yinc = (float)dy / (float) steps;
//
//	// Put pixel for each step
//	float X = one.x;
//	float Y = one.y;
//	for (int i = 0; i <= steps; i++)
//	{
//		this->set_pixel(this, (t_ivec3){X, Y, lerpf(one.z, two.z, (float)i/(float)steps)}, color);
//		X += Xinc;           // increment in x at each step
//		Y += Yinc;           // increment in y at each step
//		// generation step by step
//	}
}

static void t_renderer_plot_line_gradient(t_renderer *this, t_vec3 one, t_vec3 two, t_ivec2 color)
{
	const int dx = (int)(two.x - one.x);
	const int dy = (int)(two.y - one.y);

	// calculate steps required for generating pixels
	const int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

	// calculate increment in x & y for each steps
	const float Xinc = (float)dx / (float) steps;
	const float Yinc = (float)dy / (float) steps;

	// Put pixel for each step
	float X = one.x;
	float Y = one.y;
	int i;
	NWHILE(i = 0, i <= steps);
		this->set_pixel(this, (t_ivec2) {X, Y}, one.z,
						lerpcolor(color.x, color.y, (float) i++ / (float) steps));
		X += Xinc;
		Y += Yinc;
	CLOSE_BRACKET;
//	for (int i = 0; i <= steps; i++)
//	{
//
//		this->set_pixel(this, (t_ivec2) {X, Y}, one.z,
//						lerpcolor(color.x, color.y, (float) i / (float) steps));
//		X += Xinc;           // increment in x at each step
//		Y += Yinc;           // increment in y at each step
//		// generation step by step
//	}
}

static void t_renderer_set_pixel(struct s_renderer *this, t_ivec2 point, float depth, int color)
{
	const int j = point.x * this->viewport_geometry.y + point.y;
	if (j > this->viewport_geometry.x*this->viewport_geometry.y||
		j < 0)
		return ;
	if (this->zbuffer[j] > depth)
	{
		this->buffer->set_pixel(this->buffer, point, color);
		this->zbuffer[j] = depth;
	}

}

static void t_renderer_flush(struct s_renderer *this)
{
	mlx_put_image_to_window(this->mlx, this->window, this->buffer->image, 0, 0);
}

static void t_renderer_clear(struct s_renderer *this)
{
	int i;
	const int dimensions = this->viewport_geometry.x*this->viewport_geometry.y;
	this->buffer->clear(this->buffer);
	i = 0;
	while(i < dimensions)
		this->zbuffer[i++] = 9999999.0f;
//	ft_memset(this->z_buffer->ptr, 0xFF, (int)(this->z_buffer->geometry.x*this->z_buffer->geometry.y*(float)(this->z_buffer->bpp/8)));
}

void	t_renderer_line(t_renderer *this, t_vec4 one, t_vec4 two, t_ivec2 color)
{
	const static t_vec2 viewport_pos = (t_vec2){0, 0};
	const t_vec3	ndc_one = (t_vec3){one.x/one.w, one.y/one.w, -one.z};
	const t_vec3	ndc_two = (t_vec3){two.x/two.w, two.y/two.w, -two.z};
	t_vec3 ss_one;
	t_vec3 ss_two;

	if ((ndc_one.x > 1.0f || ndc_one.x < -1.0f) || (ndc_two.x > 1.0f || ndc_two.x < -1.0f))
		return ;
	if ((ndc_one.y > 1.0f || ndc_one.y < -1.0f) || (ndc_two.y > 1.0f || ndc_two.y < -1.0f))
		return ;
	if (two.z > 0.0f || one.z > 0.0f)
		return ;
	ss_two = (t_vec3){ndc_two.x*(float)this->viewport_geometry.x/2.0f+(viewport_pos.x+(float)this->viewport_geometry.x/2.0f),
					  ndc_two.y*(float)this->viewport_geometry.y/2.0f+(viewport_pos.y+(float)this->viewport_geometry.y/2.0f),
					  ndc_two.z};
	ss_one = (t_vec3){ndc_one.x*(float)this->viewport_geometry.x/2.0f+(viewport_pos.x+(float)this->viewport_geometry.x/2.0f),
					  ndc_one.y*(float)this->viewport_geometry.y/2.0f+(viewport_pos.y+(float)this->viewport_geometry.y/2.0f),
					  ndc_one.z};
	this->plot_line_gradient(this, ss_one, ss_two, color);
}

void	t_renderer_line_ortho(t_renderer *this, t_vec4 one, t_vec4 two, t_ivec2 color)
{
//	const static t_vec2 viewport_pos = (t_vec2){0, 0};
	const t_vec3	ssTwo = (t_vec3){two.x+(float)this->viewport_geometry.x/2.0f,
							   two.y+(float)this->viewport_geometry.y/2.0f,
							   -two.z};
	const t_vec3	ssOne = (t_vec3){one.x+(float)this->viewport_geometry.x/2.0f,
							   one.y+(float)this->viewport_geometry.y/2.0f,
								-one.z};

	this->plot_line_gradient(this, ssOne, ssTwo, color);
}

t_renderer		*make_t_renderer(void *mlx, void *window, t_ivec2 geometry)
{
	t_renderer *object;

	if(!(object = (t_renderer *)malloc(sizeof(t_renderer))))
		return (NULL);
	object->mlx = mlx;
	object->window = window;
	object->viewport_geometry = geometry;
	if(!(object->buffer = make_t_image(mlx, geometry)))
	{
		free(object);
		return (NULL);
	}
	if(!(object->zbuffer = malloc(sizeof(float)*geometry.x*geometry.y)))
	{
		destroy_t_image(object->buffer, object->mlx);
		free(object);
		return (NULL);
	}
	ft_bzero(object->zbuffer, sizeof(float)*geometry.x*geometry.y);
	object->plot_line = &t_renderer_plot_line;
	object->set_pixel = &t_renderer_set_pixel;
	object->clear = &t_renderer_clear;
	object->flush = &t_renderer_flush;
	object->line = &t_renderer_line;
	object->line_ortho = &t_renderer_line_ortho;
	object->plot_line_gradient = &t_renderer_plot_line_gradient;
	return (object);
}

void		destroy_t_renderer(t_renderer *object)
{
	destroy_t_image(object->buffer, object->mlx);
	free(object->zbuffer);
	free(object);
}
