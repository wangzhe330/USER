/* $Id: H:/drh/idioms/book/RCS/list.doc,v 1.11 1997/02/21 19:46:01 drh Exp $ */
#ifndef LIST_INCLUDED
#define LIST_INCLUDED
#define T List_T
typedef struct T *T;
struct T {
	T rest;
	void *first;
};
extern T      List_append (T list, T tail);
extern T      List_copy   (T list);
extern T      List_list   (void *x, ...);
extern T      List_pop    (T list, void **x);
extern T      List_push   (T list, void *x);
extern T      List_reverse(T list);
extern int    List_length (T list);
extern void   List_free   (T *list);
extern void   List_map    (T list,
	void apply(void **x, void *cl), void *cl);
extern void **List_toArray(T list, void *end);

#define  NEW(p) ((p) = (void*)malloc((long)sizeof *(p)))
#define NEW0(p) ((p) = (void*)calloc(1, (long)sizeof *(p)))


#undef T
#endif
