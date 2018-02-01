/*
**  LOCALIZATION
**  file: loc.h
**  Description: Header of loc.c
*/

#ifndef LOC_H_
#define LOC_H_

/*
** mysdl.h : operation on images
** includes : <stdlib.h> <err.h> <SDL/SDL.h> <SDL/SDL_image.h>
**
** strbasic.h : basic algorithmic structures
*/
#include "../sdl/mysdl.h"
#include "../strbasic.h"

/*
** Structures
*/

// used with CONTAINER_OF
struct coord_list{
    int *begin;
    int *end;
    struct list list;  
};

//Head of list
static inline
void add_coord_list(struct list *head, int *begin, int *end){
    struct coord_list *nov = malloc(sizeof(struct coord_list));
    nov->list.next = NULL;
    nov->begin = begin;
    nov->end = end;
    list_push_front(head, &nov->list);
}

/*
** Functions
*/

void h_search(SDL_Surface *img, struct list *head_FP_list, struct list
*head_AP_list);

# endif

