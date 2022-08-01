#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*last;

	if (!new)
		return ;
	last = ft_lstlast(*alst);
	if (!last)
		ft_lstadd_front(alst, new);
	else
		last->next = new;
}
