cmake_minimum_required(VERSION 3.14)
project(fdf C)

set(CMAKE_C_STANDARD 90)

INCLUDE_DIRECTORIES(minilibx_macos libft)
LINK_DIRECTORIES(minilibx_macos libft)
add_executable(fdf main.c main_loop1.c linear_alg.c renderer.c image.c landscape.c math.c read_map.c gnl.c gnl.h ft_make_t_vector2.c ft_make_t_vector.c ft_destroy_t_vector.c read_map_2.c process_input.c linear_alg2.c landscape_2.c image_to_bmp.c image_to_bmp2.c tools1.c)
target_link_libraries(fdf mlx ft)
#target_link_libraries(fdf OpenGL)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
target_link_libraries(fdf ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
