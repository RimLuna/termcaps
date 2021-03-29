#ifndef FT_HISTORY_H
# define FT_HISTORY_H
# include "minishell.h"

int			readline(char **line, t_hist *hist);
t_hlist		*dlist_push_back(t_hlist **lst, t_hlist *new);
void		del(int size);
#endif
