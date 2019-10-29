#ifndef ARRAYS_H
# define ARRAYS_H

# include <libft.h>
# include <maths/vec4.h>

typedef struct		s_4dvertices
{
	int				len;
	int				capacity;
	t_vec4			vertices[];
}					t_4dvertices;

typedef struct		s_3dvertices
{
	int				len;
	int				capacity;
	t_vec3			vertices[];
}					t_3dvertices;

typedef struct		s_2dvertices
{
	int				len;
	int				capacity;
	t_vec2			vertices[];
}					t_2dvertices;

typedef struct		s_ints
{
	int				len;
	int				capacity;
	int				values[];
}					t_ints;

typedef struct		s_ptrs
{
	int				len;
	int				capacity;
	void			*values[];
}					t_ptrs;


t_2dvertices		*create_2dvertices_array(int capacity);
t_2dvertices		*append_2dvertices_array(t_2dvertices **arr, t_vec2 v);
t_2dvertices		*splice_2dvertices_array(t_2dvertices *arr,
						int index, int n);
t_2dvertices		*copy_2dvertices_array(t_2dvertices *src,
						t_2dvertices **dst);
int					vertices2d_indexof(t_2dvertices *arr, t_vec2 v);

t_3dvertices		*create_3dvertices_array(int capacity);
t_3dvertices		*append_3dvertices_array(t_3dvertices **arr, t_vec3 v);
t_3dvertices		*splice_3dvertices_array(t_3dvertices *arr,
						int index, int n);
t_3dvertices		*copy_3dvertices_array(t_3dvertices *src,
						t_3dvertices **dst);

t_4dvertices		*create_4dvertices_array(int capacity);
t_4dvertices		*append_4dvertices_array(t_4dvertices **arr, t_vec4 v);
t_4dvertices		*splice_4dvertices_array(t_4dvertices *arr,
						int index, int n);
t_4dvertices		*copy_4dvertices_array(t_4dvertices *src,
						t_4dvertices **dst);

t_ints				*create_ints_array(int capacity);
t_ints				*append_ints_array(t_ints **arr, int i);
t_ints				*splice_ints_array(t_ints *arr,
						int index, int n);
t_ints				*copy_ints_array(t_ints *src,
						t_ints **dst);
int					ints_indexof(t_ints *arr, int i);


t_ptrs				*create_ptrs_array(int capacity);
t_ptrs				*append_ptrs_array(t_ptrs **arr, void *p);
t_ptrs				*splice_ptrs_array(t_ptrs *arr,
						int index, int n);
t_ptrs				*copy_ptrs_array(t_ptrs *src,
						t_ptrs **dst);

#endif
