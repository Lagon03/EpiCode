/* strbasic.h : provides basic structures for algorithmic computations such as
 * basic intrusive lists
*/

# ifndef BASIC_STRUCTS_H_
# define BASIC_STRUCTS_H_

# include <stddef.h>

# define CONTAINER_OF(_type_, _field_, _ptr_) \
         ((_type_*)((char*)(_ptr_) - offsetof(_type_, _field_)) 

/* classic intrusive lists */

struct list
{
    struct list *next;
};

/*
** initializes a new list with head as a sentinel 
*/
static inline
struct list* list_init(){
    struct list *head = malloc(sizeof(struct list));
    head->next = NULL;
    return head;
}

/*
** pushes elm in front of the sentinel head
** elm and head must be allocated cells 
*/
static inline
void list_push_front(struct list *head, struct list *elm){
    elm->next = head->next;
    head->next = elm;
}

/*
** pops the first element of the list and returns it
** returns NULL if the list is empty
*/
static inline
struct list* list_pop_front(struct list *head){
    struct list *elm = head->next;
    if (elm) {
        head->next = elm->next;
        elm->next = NULL;
    }
    return elm;
}

/* 
**iterating macro on list
** _list_iter_ must be name of an existing variable of type struct list
*/

# define foreach_list(_head_ , _list_iter_) \
    for (_list_iter_ = _head_;  \
         _list_iter_->next;     \
         _list_iter_ = _list_iter_->next )

# endif
