/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_to_bmp2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 17:53:41 by nsena             #+#    #+#             */
/*   Updated: 2019/10/24 17:53:43 by nsena            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	write_file_size(int fd, const t_image *image)
{
	const uint32_t	header_size = 14;
	const uint32_t	dib_size = 108;
	const uint32_t	colors_per_pixel = 3;
	const uint32_t	pixels_total = image->geometry.x*image->geometry.y;
	const uint32_t	size = header_size + dib_size + pixels_total * colors_per_pixel;
	write(fd, &size, 4);
}

static void	write_raw_data_offset(int fd, t_image *image)
{
	const uint32_t	header_size = 14;
	const uint32_t	dib_size = 108;
	const uint32_t	offset = header_size + dib_size;
	write(fd, &offset, 4);
}

static void	write_dib_header_size(int fd)
{
	const uint32_t dib_header_size = 108;
	write(fd, &dib_header_size, 4);
}

void		image_to_bmp2(int fd, t_image *image)
{
	const int8_t	magic[2] = {0x42, 0x4D};
	const uint32_t width = image->geometry.x;
	const uint32_t height = image->geometry.y;
	uint16_t temp16;

	write(fd, magic, 2);
	write_file_size(fd, image);
	write_repeatedly(fd, 0x0, 4);
	write_raw_data_offset(fd, image);
	write_dib_header_size(fd);
	write(fd, &width, 4);
	write(fd, &height, 4);
	temp16 = 0x1;
	write(fd, &temp16, 2);
	temp16 = 24;
	write(fd, &temp16, 2);
}
