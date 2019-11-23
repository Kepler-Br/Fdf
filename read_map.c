#include "fdf.h"

int		get_index(char *str, char c)
{
	int i;
	i = 0;
	while ((size_t)i < ft_strlen(str))
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

int		get_color(char *f_color)
{
	char	*digits;
	int		result;
	int		i;

	if (!f_color)
		return (-1);
	digits = "0123456789ABCDEF";
	result = 0;
	i = 2;
	while ((size_t)i < ft_strlen(f_color))
	{
		result += get_index(digits, f_color[i]);
		result *= 16;
		i++;
	}
	result /= 16;
	return (result);
}

void	free_str_arr(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr[i]);
	free(arr);
}

int		read_map(t_data *data, int fd)
{
	char	**points;
	int		ret;

	INIT_COORD(coord);
	INIT_STR(line);
	init_data(data);
	while ((ret = gnl(fd, &line)) != 0)
	{
		if (ret == -1)
			return (-1);
		points = ft_strsplit(line, ' ');
		if ((process_line(points, data, &coord)) == -1)
			return (-1);
		coord.x = 0;
		coord.y++;
		free_str_arr(points);
		free(line);
	}
	data->y_size = coord.y;
	free(line);
	return (0);
}