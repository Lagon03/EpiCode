/*
**  Localization
**  file: loc.c
**  description : code to localize the pattern elements of a QrCode
*/

#include "loc.h"

//-----------------------------------------------------------------------------
//                              SUB_ FUNCTIONS
//-----------------------------------------------------------------------------
static inline
void add_to_list (struct list *head, int *begin, int *end){
    int *mbegin = malloc(sizeof(int)*2);
    int *mend = malloc(sizeof(int)*2);
    mbegin[0] = begin[0];
    mbegin[1] = begin[1];
    mend[0] = end[0];
    mend[1] = end[1];
    add_coord_list(head, mbegin, mend);     
}

//-----------------------------------------------------------------------------
//                               FUNCTIONS
//-----------------------------------------------------------------------------

//Horizontal scan to retrieve horizontal segments matching the format
//Call function after image treatement
void h_search(SDL_Surface *img, struct list *head_FP_list,  struct list *head_AP_list){
    
    int stepF = 0;                           //count pattern from 1 - 5
    int mod_sizeF = 1;
    int mod_counter = 1;
    int segF_begin[2];
    int segF_end[2];
    //struct list head_AP_list = list_init();

    for(int y = 0; y < img->w; y += 10) {       //going vertical
        for(int x = 0; x < img->h; x++) {   //going horizontal
            Uint32 pixel = getpixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            
            /*FP loc, Pattern to match : 1b,1w,3b,1w,1b*/
            
            if (stepF == 0 && r == 0){
                stepF++;
                segF_begin[1] = y;
                segF_begin[0] = x;
                segF_end[1] = y;
            }

            if (stepF == 1){
                
                if (r == 0){
                    mod_sizeF++;
                }

                if (r == 255){
                    stepF++;
                }
            }
    
            if (stepF == 2 || stepF == 4){
               // warn("Step 2 or 4");
                if (r == 0){
                    
                    if(mod_sizeF != mod_counter){ //bad end
                        mod_sizeF = 0;
                        mod_counter = 0;
                        stepF = 0;
                        segF_begin[0] = 0;
                        segF_begin[1] = 0;
                        segF_end[0] = 0;
                        segF_end[1] = 0;
                    }
                    else
                        stepF++;
                }
                
                if (r == 255){
                    mod_counter++;

                    if(mod_counter > mod_sizeF){ //bad end
                        mod_sizeF = 0;
                        mod_counter = 0;
                        stepF = 0;
                        segF_begin[0] = 0;
                        segF_begin[1] = 0;
                        segF_end[0] = 0;
                        segF_end[1] = 0;
                    }
                }
            }
            
            if (stepF == 3){
                //warn("Step 3");
                if(r == 255){
                    
                    //putpixel(img, y, x, SDL_MapRGB(img->format, 0, 0, 0));
                    if(mod_sizeF * 3 != mod_counter){ //bad end
                        mod_sizeF = 0;
                        mod_counter = 0;
                        stepF = 0;
                        segF_begin[0] = 0;
                        segF_begin[1] = 0;
                        segF_end[0] = 0;
                        segF_end[1] = 0;
                        warn("white, %d|%d", x, y);
                       // putpixel(img, y, x, SDL_MapRGB(img->format, 255, 0, 0));
                    }
                    else
                        stepF++;
                }
                
                if(r == 0){
                    mod_counter++;
                    //putpixel(img, y, x, SDL_MapRGB(img->format, 255, 255, 255));
                    if(mod_counter > mod_sizeF * 3){ //bad end
                        mod_sizeF = 0;
                        mod_counter = 0;
                        stepF = 0;
                        segF_begin[0] = 0;
                        segF_begin[1] = 0;
                        segF_end[0] = 0;
                        segF_end[1] = 0;
                        warn("black, %d|%d", x, y);
                        //putpixel(img, y, x, SDL_MapRGB(img->format, 255, 255, 255));
                    }
                }      
            }

            if (stepF == 5){
                warn("Step 5");
                if(r == 255){
                    
                    if(mod_sizeF != mod_counter){ //bad end
                        mod_sizeF = 0;
                        mod_counter = 0;
                        stepF = 0;
                        segF_begin[0] = 0;
                        segF_begin[1] = 0;
                        segF_end[0] = 0;
                        segF_end[1] = 0;
                    }
                    else{ //good end FP
                        segF_end[0] = x;
                        add_to_list(head_FP_list, segF_begin, segF_end);
                        mod_sizeF = 0;
                        mod_counter = 0;
                        stepF = 0;
                        segF_begin[0] = 0;
                        segF_begin[1] = 0;
                        segF_end[0] = 0;
                        segF_end[1] = 0;
                    }
                }
                
                if(r == 0){
                    mod_counter++;

                    if(mod_counter > mod_sizeF){ //bad end
                        mod_sizeF = 0;
                        mod_counter = 0;
                        stepF = 0;
                        segF_begin[0] = 0;
                        segF_begin[1] = 0;
                        segF_end[0] = 0;
                        segF_end[1] = 0;
                    }
                }      
            }
            putpixel(img, x, y, SDL_MapRGB(img->format, 0, 255, 0));  
        }

        mod_sizeF = 0;
        mod_counter = 0;
        stepF = 0;
        segF_begin[0] = 0;
        segF_begin[1] = 0;
        segF_end[0] = 0;
        segF_end[1] = 0;
    }
}

