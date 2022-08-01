#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*newlink;

	newlink = (t_list*)malloc(sizeof(t_list));
	if (!newlink)
		return (NULL);
	newlink->content = content;
	newlink->next = NULL;
	return (newlink);
}
