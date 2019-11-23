/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_to_bmp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsena <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 17:53:38 by nsena             #+#    #+#             */
/*   Updated: 2019/10/24 17:53:39 by nsena            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	write_raw_data_size(int fd, t_image *image)
{
	const uint32_t	colors_per_pixel = 3;
	const uint32_t	pixels_total = image->geometry.x * image->geometry.y;
	const uint32_t	size = colors_per_pixel * pixels_total;
	write(fd, &size, 4);
}

static void	write_print_resolution(int fd)
{
	const uint32_t	dpi = 2835;
	// Horizontal.
	write(fd, &dpi, 4);
	// Vertical.
	write(fd, &dpi, 4);
}

static size_t	calc_padding(size_t width)
{
	size_t padding;

	padding = 0;
	if ((width * 3) % 4 != 0)
		padding = 4 - ((width * 3) % 4);
	return padding;
}

static void	write_pixels(int fd, t_image *image)
{
	int			h;
	int			w;
	int				pixel;
	uint8_t			color;

	h = image->geometry.y-1;
	while (h >= 0)
	{
		w = 0;
		while (w < image->geometry.x)
		{
			pixel = image->get_pixel(image, (t_ivec2){w, h});
			color = pixel & 0x0000FF;
			write(fd, &color, 1);
			color = (pixel & 0x00FF00) >> 8;
			write(fd, &color, 1);
			color = (pixel & 0xFF0000) >> 16;
			write(fd, &color, 1);
			w++;
		}
		write_repeatedly(fd, 0x0, calc_padding(image->geometry.x));
		h--;
	}
}

void		image_to_bmp(t_image *image, char *filename)
{
	int fd;
	const char color_space[] = {0x42, 0x47, 0x52, 0x73};

	fd = open(filename, O_WRONLY | O_CREAT, 0666);
	if (fd < 1)
		return ;
	image_to_bmp2(fd, image);
	write_repeatedly(fd, 0x0, 4);
	write_raw_data_size(fd, image);
	write_print_resolution(fd);
	write_repeatedly(fd, 0x0, 4);
	write_repeatedly(fd, 0x0, 4);
	write(fd, color_space, 4);
	write_repeatedly(fd, 0x0, 24 + 40);
	write_pixels(fd, image);
	close(fd);
}
