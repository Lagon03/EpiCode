/*
**  QrCode Extractor
**  file : extcode.c
**  description : takes in the found centers of the Finder patterns, returns the
**  code in a matrix. For now, it DOESN'T take in account any geometric
**  rectification(NoG functions) and The FPs are already ideally placed. (INCOMMING IN PHASE 2)
*/

# include "extcode.h"

// STATIC FUNCTIONS

static inline
double get_module_size_NoG(struct Dvector *ems)
{
    double totalsize = ems->array[0] + ems->array[1] + ems->array[2];
    double m_size = round(totalsize / 3.0);
    return m_size;
}

static inline
double estimate_version_NoG(struct Finder *A, struct Finder *B, double m_size)
{
    double distAB = sqrt((A->center[0] - B->center[0]) *
     (A->center[0] - B->center[0]) +
     (A->center[1] - B->center[1]) *
     (A->center[1] - B->center[1]));
    
    double tmp = (distAB/(double)m_size - 10)/4;
    int tmp2 = (int)round(tmp);
    
    //warn("dist %lf, tmp %lf, tmp2 %d", distAB, tmp, tmp2);

    return tmp2;
}

static inline                                                                   
int get_BW(SDL_Surface *img, int x, int y) //returns 0 if black, 1 if white     
{                                                                               
     Uint32 pixel = getpixel(img, x, y);                                         
     Uint8 r, g, b;                                                              
     SDL_GetRGB(pixel, img->format, &r, &g, &b);                                 
                                                                                 
     if(r == 0)                                                                  
        return 0;                                                               
     else                                                                        
        return 1;                                                               
}  

static inline
void fill_qr(SDL_Surface *img, struct QrCode *qr)
{
    int mod_x = 0;
    int mod_y = 0;
    
    int dtcornerx = (int)qr->A->ul[0];
    int dtcornery = (int)qr->A->ul[1];
    int dbcornerx = (int)qr->B->dr[0];
    int dbcornery = (int) qr->C->dr[1];
    
    int size = qr->version * 4 + 17;

    //warn("version and size %d %d",qr->version, size);
    //warn("top corner %d:%d, Bottom corner %d:%d", dtcornerx, dtcornery
    //      dbcornerx, dbcornery);
    for(int y = dtcornery; y <= dbcornery && y < img->h && mod_y < size; y += qr->m_size)
    {  
        //warn("%d,%d", qr->A->ul[1], qr->C->dr[1]);
        mod_x = 0; 
        for(int x = dtcornerx; x <= dbcornerx && x < img->w && mod_x < size ; x+= qr->m_size)
        {
            //warn("%d,%d == %d,%d", x, y, mod_x, mod_y);
            if(get_BW(img, x, y) == 0)
            {   
                qr->mat[mod_y][mod_x] = '1';
            }
            else
            {
                qr->mat[mod_y][mod_x] = '0';
            }
            mod_x++;
        }
        mod_y++;
    }
}

static inline
void Get_QrCode_loc(struct QrCode *qr)
{
    double *coords = calloc(4, sizeof(double));
    coords[0] = qr->A->ul[0];
    coords[1] = qr->A->ul[1];
    coords[2] = qr->B->dr[0];
    coords[3] = qr->C->dr[1];
    qr->coords = coords;
}

// MAIN FUNCTION

//Extracts QrCode struct for further computations
struct QrCode *extract_QrCode_NoG(SDL_Surface *img, struct FPat *finders)
{
    struct QrCode *qr = malloc(sizeof(struct QrCode));
    double m_size = get_module_size_NoG(finders->ems_vector);
    qr->m_size = m_size;

    qr->A = init_Finder(finders->centers->mat[0][0], finders->centers->mat[0][1], m_size);
    qr->B = init_Finder(finders->centers->mat[1][0], finders->centers->mat[1][1], m_size);
    qr->C = init_Finder(finders->centers->mat[2][0], finders->centers->mat[2][1], m_size);
    
    Get_QrCode_loc(qr);
    
    qr->version = estimate_version_NoG(qr->A, qr->B, m_size);
    
    //warn("Result : version v%02d, module size %lf", qr->version, qr->m_size);
    
    int code_size = 17 + 4 * qr->version;
    //warn("B version size %d %d", qr->version, code_size);
    char **code_mat = malloc(sizeof(char *) * code_size);
    for (int i = 0; i < code_size; i++)
        code_mat[i] = malloc(sizeof(char) * code_size);
    
    //Nota bene : This will ONLY work with perfect images
    qr->mat = code_mat;
    fill_qr(img, qr);
    
    return qr;
}
