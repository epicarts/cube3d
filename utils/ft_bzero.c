
#include "../cub3d.h"

void		*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*p_b;

	p_b = b;
	i = 0;
	while (i < len)
	{
		p_b[i] = (unsigned char)c;
		i++;
	}
	return (b);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}