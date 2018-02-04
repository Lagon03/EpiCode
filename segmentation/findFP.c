/*
**  Finder Pattern Locater
**  file : scan.c
**  description : Locate the Finder Patterns
*/

# include "findFP.h"

# define GET_SIZE(_begin_ , _end_) \
    ((_end_ - _begin_));

# define PRECISION 0.03

// Functions

static inline
int seg_inter(int *h, int *v)
{
    int tier_v = (v[3] - v[1]) / 2 - 1;
    int tier_h = (h[2] - h[0]) / 2 - 1;
    
    if(v[0] < h[0] + tier_h)
        return 0;
    if(v[2] > h[2] - tier_h)
        return 0;
    if(v[1] + tier_v > h[1])
        return 0;
    if(v[3] - tier_v < h[3])
        return 0;
    return 1;
}

static inline
double ratio_hv(int *h, int *v)
{
    double v_len = GET_SIZE(h[0], h[2]);
    double h_len = GET_SIZE(v[1], v[3]);
    
    //warn("%lf,%lf", h_len, v_len);
    double ratio = (double) h_len / (double)v_len;
    return ratio;
}

static inline
void get_intersection (struct Dmat *inter, int *h, int *v)
{
    int *h_v = calloc(2, sizeof(int));
    h_v[0] = v[0];
    h_v[1] = h[1];
    add_Dmat(inter, h_v);
}

// Main Functions

struct Dmat *find_FP (struct Dmat *h_seg, struct Dmat *v_seg)
{    
    struct Dmat *inter2 = init_Dmat(2 , 4);
    struct Dmat *inter = init_Dmat(2, 2);
    
    for(size_t i = 0; i < h_seg->lines; i++)
    {   
        for(size_t j = 0;  j < v_seg->lines; j++)
        {   
            if(seg_inter(h_seg->mat[i], v_seg->mat[j]) == 0)
                continue;

            double ratio = ratio_hv(h_seg->mat[i], v_seg->mat[j]);
            if(ratio >= 1 + PRECISION || ratio <= 1 - PRECISION )
                continue;

            //warn("%lf ratio", ratio_hv(h_seg->mat[i], v_seg->mat[j]));
            add_Dmat(inter2, h_seg->mat[i]);
            add_Dmat(inter2, v_seg->mat[j]);
        
            get_intersection(inter, h_seg->mat[i], v_seg->mat[i]);
            
        }
    }
    return inter2;
}



