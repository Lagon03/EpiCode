/*
** Test File and Print on Image File
*/

#include "loc.h"
#include "../preproc/preproc.h"

//----------------------------Print Functions----------------------------------
void draw_hline(SDL_Surface *img, int begin, int end, int height){
    for(int x = begin; x <= end; x++){
        putpixel(img, x, height, SDL_MapRGB(img->format, 255, 0, 0));
    }
}

void print_seg(SDL_Surface *img, struct list *head){
    struct list *cur;
    foreach_list(head, cur){
        struct coord_list *coord_cur = CONTAINER_OF(struct coord_list, list,
cur));
        draw_hline(img, coord_cur->begin[0], coord_cur->end[0], coord_cur->begin[1]);
    }
}

//-----------------------------------------------------------------------------

int main(int argc, char* argv[]){
    init_sdl();
    if( argc > 1){
        SDL_Surface *img = load_image(argv[1]);
        img = grayscale(img);
        img = blackAndWhite(img, 0);
        struct list *head_FP = list_init();
        struct list *head_AP = list_init();
        h_search (img, head_FP, head_AP);
        print_seg(img, head_FP);
        display_image(img);  
    }
    SDL_Quit();
    return 1;
}
