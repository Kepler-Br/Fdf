/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 18:50:04 by nsena             #+#    #+#             */
/*   Updated: 2019/10/21 18:50:05 by nsena            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

float 	lerpf(float source, float target, float t)
{
	return ((1.0f - t) * source + t * target);
}

t_vec3	lerpvec3(t_vec3 source, t_vec3 target, float t)
{
	return ((t_vec3){(1.0f - t) * source.x + t * target.x,
					 (1.0f - t) * source.y + t * target.y,
					 (1.0f - t) * source.z + t * target.z});
}

int 	lerpcolor(int source, int target, float t)
{
	const t_vec3 color1 = int_to_rgb(source);
	const t_vec3 color2 = int_to_rgb(target);
	return rgb_to_int(lerpvec3(color1, color2, t));
}

t_vec3		int_to_rgb(int color)
{
	return ((t_vec3){(float)((color & 0xFF0000) >> 16)/255.0f,
					 (float)((color & 0x00FF00) >> 8)/255.0f,
					 (float)((color & 0x0000FF))/255.0f});
}

float clampf(float min, float max, float num)
{
	if (num < min)
		return (min);
	if (num > max)
		return (max);
	return (num);
}

int		rgb_to_int(t_vec3 rgb)
{

	const int r = (int)(clampf(0.0f, 1.0f, rgb.x) * 255);
	const int g = (int)(clampf(0.0f, 1.0f, rgb.y) * 255);
	const int b = (int)(clampf(0.0f, 1.0f, rgb.z) * 255);
	int color;
	color = 0;
	color |= r;
	color <<= 8;
	color |= g;
	color <<= 8;
	color |= b;
	return (color);
}

t_vec3	int_to_hsv(int color)
{
	const t_vec3 rgb = int_to_rgb(color);
	const float c_max = fmaxf(rgb.x, fmaxf(rgb.y, rgb.z));
	const float c_min = fminf(rgb.x, fminf(rgb.y, rgb.z));
	const float delta = c_max - c_min;
	t_vec3 hsv;

	if (delta == 0.0f)
		hsv.x = 0.0f;
	else if(c_max == rgb.x)
		hsv.x = 60.0f * fmodf((rgb.y - rgb.z)/delta, 6.0f);
	else if(c_max == rgb.y)
		hsv.x = 60.0f * (rgb.z - rgb.x)/delta + 2.0f;
	else if(c_max == rgb.y)
		hsv.x = 60.0f * (rgb.x - rgb.y)/delta + 4.0f;
	if (c_max == 0.0f)
		hsv.y = 0.0f;
	else
		hsv.y = delta/c_max;
	hsv.z = c_max;
	while (hsv.x < 0)
		hsv.x += 360;
	while (hsv.x > 360)
		hsv.x -= 360;
	return (hsv);
}

int 	hsv_to_int(t_vec3 hsv)
{
	const float c = hsv.y * hsv.z;
	const float x = c * (1.0f - fabsf(fmodf(hsv.x / 60.0f, 2.0f) - 1.0f));
	const float m = hsv.z - c;
	t_vec3 rgb;

	if (hsv.x >= 0.0f && hsv.x < 60.0f)
		rgb = (t_vec3){c, x, 0.0f};
	else if (hsv.x >= 60.0f && hsv.x < 120.0f)
		rgb = (t_vec3){x, c, 0.0f};
	else if (hsv.x >= 120.0f && hsv.x < 180.0f)
		rgb = (t_vec3){0.0f, c, x};
	else if (hsv.x >= 180.0f && hsv.x < 240.0f)
		rgb = (t_vec3){0.0f, x, c};
	else if (hsv.x >= 240.0f && hsv.x < 300.0f)
		rgb = (t_vec3){x, 0.0f, c};
	else
		rgb = (t_vec3){c, 0.0f, x};
	rgb = (t_vec3){rgb.x + m, rgb.y + m, rgb.z + m};
	return (rgb_to_int(rgb));
}
