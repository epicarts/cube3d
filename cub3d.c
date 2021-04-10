//
// Created by YoungHo on 2021/03/20.
//

#include <math.h>
#include "cub3d.h"

void	init_info(t_info *info)
{
	//mlx초기화, 윈도우초기화
	if (!(info->mlx_ptr = mlx_init()) || !(info->win = mlx_new_window(
			info->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "cub3d")))
		exit(0); //실패시 종료
	set_xy(&info->move, 0, 0);

	//회전 및 움직임 정도.
	info->move_speed = 0.05;
	info->rotate_speed = 0.05;

	// 키보드 초기화.
	info->key.w = 0;
	info->key.a = 0;
	info->key.s = 0;
	info->key.d = 0;
	info->key.l = 0;
	info->key.r = 0;
}


void	put_draw(t_info *info)
{
	int y;
	int x;

	y = 0;
	while (y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			info->img.data[y * WIN_WIDTH + x] = info->buf[y][x];
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(info->mlx_ptr, info->win, info->img.img_ptr, 0, 0);
}

int main_loop(t_info *info)
{
	t_ray	ray;
	int x;

	if (info->key.w || info->key.s)
		move_front_back(info);
	else if (info->key.a || info->key.d)
		move_left_right(info);
	if (info->key.r || info->key.l)
		rotate(info);
	x = -1; // 0 ~ 최대폭 까지
	while (++x < WIN_WIDTH)
	{
		calc_ray(info, &ray, x); // init ray
		draw_wall(info, &ray, x); // 벽 그리기.
	}
	draw_sprites(info);
	if (info->screenshot_flag == 1)
		screenshot(info);
	put_draw(info);
	return (0);
}

int			main(int argc, char *argv[])
{
	t_info	info;

	arg_parse(&info, argc, argv);
	read_map(&info, info.map_path);
	if (malloc_sprite(&info))
	{
		printf("sprite실패");
		free_map(&info); //위에서 맵을 동적할당했으므로 free
		exit(-1);
	}

	//todo 삭제.
	for (int i = 0; i < info.sprite_count; i++)
	{
		printf("(%f,%f)\n", info.sprite[i].y, info.sprite[i].x);
	}

	init_info(&info);

	//텍스쳐가 스크린 사이즈 버퍼. => 버퍼에 저장해놓고 그림.
	for (int i = 0; i < WIN_HEIGHT; i++)
	{
		for (int j = 0; j < WIN_WIDTH; j++)
		{
			info.buf[i][j] = 0;
		}
	}

	if (init_texture(&info))
	{
		printf("texture 초기화 실패");
		free_texture_path(&info);
		free_map(&info);
		free_sprite(&info);
		exit(-1);
	}
	if (load_textures(&info))
	{
		printf("texture 로드 실패");
		free_texture_path(&info);
		free_texture(&info);
		free_sprite(&info);
		free_map(&info);
		//todo free check
		exit(-1);
	}

	info.img.img_ptr = mlx_new_image(info.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	info.img.data = (int *)mlx_get_data_addr(info.img.img_ptr, &info.img.bpp, &info.img.size_line, &info.img.endian);

	//키 이벤트, 키 함수.
	mlx_hook(info.win, X_EVENT_KEY_PRESS, 0, &key_press, &info);
	mlx_hook(info.win, X_EVENT_KEY_RELEASE, 0, &key_release, &info);
	mlx_hook(info.win, X_EVENT_EXIT, 0, &exit_win, &info);

	mlx_loop_hook(info.mlx_ptr, &main_loop, &info);

	// 시작.
	mlx_loop(info.mlx_ptr);
	return (0);
}

// todo 스크린 자동 조절
//https://harm-smits.github.io/42docs/libs/minilibx/prototypes.html#mlx_get_screen_size
//int   *ft_screen_check(t_all *s)
//{
//	int max_x;
//	int max_y;
//	mlx_get_screen_size(s->mlx.ptr, &max_x, &max_y);
//	if (max_x < s->win.x)
//		s->win.x = max_x;
//	if (max_y < s->win.y)
//		s->win.y = max_y;
//	return (mlx_new_window(s->mlx.ptr, s->win.x, s->win.y, “cub3D”));
//}