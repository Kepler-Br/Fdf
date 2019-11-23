/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linear_alg2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 18:49:39 by nsena             #+#    #+#             */
/*   Updated: 2019/10/21 18:49:40 by nsena            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_vec3	vec3_normalize(const t_vec3 vec)
{
	const float len = vec3_length(vec);

	return ((t_vec3){	vec.x / len,
						vec.y / len,
						vec.z / len});
}

t_vec3	vec3_cross(const t_vec3 a, const t_vec3 b)
{
	return ((t_vec3){	a.y * b.z - a.z * b.y,
						a.z * b.x - a.x * b.z,
						a.x * b.y - a.y * b.x});
}

float	vec3_dot(const t_vec3 x, const t_vec3 y)
{
	return (x.x * y.x + x.y * y.y + x.z * y.z);
}

t_mat4	look_at(const t_vec3 eye, const t_vec3 center, const t_vec3 up)
{
	t_vec3	u;
	t_vec3	f;
	t_vec3	s;
	t_vec3	u2;

	u = vec3_normalize(up);
	f = vec3_normalize((t_vec3){center.x - eye.x, center.y - eye.y,
							center.z - eye.z});
	s = vec3_normalize(vec3_cross(f, u));
	u2 = vec3_cross(s, f);
	return ((t_mat4){s.x, s.y, s.z, -vec3_dot(s, eye),
					u2.x, u2.y, u2.z, -vec3_dot(u2, eye),
					-f.x, -f.y, -f.z, vec3_dot(f, eye),
					0.0f, 0.0f, 0.0f, 1.0f});
}

/*
** bounds:
** x - right,
** y - left,
** z - top,
** w - bottom.
*/

t_mat4	orthographic(const t_vec4 bounds, const float near, const float far)
{
	return ((t_mat4){2.0f / (bounds.x - bounds.y), 0.0f, 0.0f, 0.0f,
					0.0f, 2.0f / (bounds.z - bounds.w), 0.0f, 0.0f,
					0.0f, 0.0f, -2.0f / (near - far), 0.0f,
					-(bounds.x + bounds.y) / (bounds.x - bounds.y),
					-(bounds.z + bounds.w) / (bounds.z - bounds.w),
					-(far + near) / (far - near), 1.0f});
}
