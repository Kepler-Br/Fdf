/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linear_alg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 18:49:30 by nsena             #+#    #+#             */
/*   Updated: 2019/10/21 18:49:32 by nsena            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

t_vec4	mat4_vec4_mul(const t_mat4 *mat, const t_vec4 *vec)
{
	t_vec4 new_vec;

	new_vec.x = mat->a * vec->x + mat->b * vec->y + mat->c * vec->z +
			mat->d * vec->w;
	new_vec.y = mat->e * vec->x + mat->f * vec->y + mat->g * vec->z +
			mat->h * vec->w;
	new_vec.z = mat->i * vec->x + mat->j * vec->y + mat->k * vec->z +
			mat->l * vec->w;
	new_vec.w = mat->m * vec->x + mat->n * vec->y + mat->o * vec->z +
			mat->p * vec->w;
	return (new_vec);
}

t_mat4	mat4_mat4_mul(const t_mat4 *m1, const t_mat4 *m2)
{
	t_mat4 res;

	res.a = m1->a * m2->a + m1->b * m2->e + m1->c * m2->i + m1->d * m2->m;
	res.b = m1->a * m2->b + m1->b * m2->f + m1->c * m2->j + m1->d * m2->n;
	res.c = m1->a * m2->c + m1->b * m2->g + m1->c * m2->k + m1->d * m2->o;
	res.d = m1->a * m2->d + m1->b * m2->h + m1->c * m2->l + m1->d * m2->p;
	res.e = m1->e * m2->a + m1->f * m2->e + m1->g * m2->i + m1->h * m2->m;
	res.f = m1->e * m2->b + m1->f * m2->f + m1->g * m2->j + m1->h * m2->n;
	res.g = m1->e * m2->c + m1->f * m2->g + m1->g * m2->k + m1->h * m2->o;
	res.h = m1->e * m2->d + m1->f * m2->h + m1->g * m2->l + m1->h * m2->p;
	res.i = m1->i * m2->a + m1->j * m2->e + m1->k * m2->i + m1->l * m2->m;
	res.j = m1->i * m2->b + m1->j * m2->f + m1->k * m2->j + m1->l * m2->n;
	res.k = m1->i * m2->c + m1->j * m2->g + m1->k * m2->k + m1->l * m2->o;
	res.l = m1->i * m2->d + m1->j * m2->h + m1->k * m2->l + m1->l * m2->p;
	res.m = m1->m * m2->a + m1->n * m2->e + m1->o * m2->i + m1->p * m2->m;
	res.n = m1->m * m2->b + m1->n * m2->f + m1->o * m2->j + m1->p * m2->n;
	res.o = m1->m * m2->c + m1->n * m2->g + m1->o * m2->k + m1->p * m2->o;
	res.p = m1->m * m2->d + m1->n * m2->h + m1->o * m2->l + m1->p * m2->p;
	return (res);
}

t_mat4	trans_mat(const float x, const float y, const float z)
{
	return ((t_mat4){1.0f, 0.0f, 0.0f, x,
					0.0f, 1.0f, 0.0f, y,
					0.0f, 0.0f, 1.0f, z,
					0.0f, 0.0f, 0.0f, 1.0f});
}

t_mat4	scale_mat(const float x, const float y, const float z)
{
	return ((t_mat4){x, 0.0f, 0.0f, 0.0f,
					0.0f, y, 0.0f, 0.0f,
					0.0f, 0.0f, z, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f});
}

float	vec3_length(const t_vec3 vec)
{
	return (sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}
