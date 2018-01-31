# include "../sdl/mysdl.h"

//Struct to store Finder Patterns
struct Qr_FP{

    size_t x1, x2, y1, y2, center;

}Qr_FP;

//Struct to store the Alignement Patterns
struct Qr_AP{

    size_t x1, x2, y1, y2, center;

}Qr_AP;

//End Struct
struct QrCode{
    
    **Qr_AP all_AP;  //depends on the version
    **Qr_FP corners; //Only 3
    **int data_matrix; //binary matrix
}QrCode;
