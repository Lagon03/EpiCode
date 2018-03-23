#include "op.h"
#include "array.h"
#include "encode.h"
#include "decode.h"

int main()
{
    struct gf_tables *gf_table = malloc(sizeof(struct gf_tables));
    gf_table->gf_exp = malloc(sizeof(struct Array));
    gf_table->gf_log = malloc(sizeof(struct Array));
    initArray(gf_table->gf_exp, 512);
    initArray(gf_table->gf_log, 256);
    gf_table = init_tables();


    struct Array *msg_in = malloc(sizeof(struct Array));

    initArray(msg_in, 100);
    msg_in->array[0] = 16;
    msg_in->array[1] = 4;
    msg_in->array[2] = 236;
    msg_in->array[3] = 17;
    msg_in->array[4] = 236;
    msg_in->array[5] = 17;
    msg_in->array[6] = 236;
    
    msg_in->used = 7;

    struct Array *msg = malloc(sizeof(struct Array));
    initArray(msg, 170);

    struct Array *synd = malloc(sizeof(struct Array));


    msg = rs_encode_msg(msg_in, 17, gf_table);
    for(size_t i = 0; i < msg->used; i++){
        printf("msg_out[%u] = %i\n", i, msg->array[i]);
    }
    /*msg->array[0] = 0;
    synd = rs_calc_syndromes(msg, 10, gf_table);
    int a = rs_check(msg,10, gf_table);
    printf("a = %d",a);
    for(size_t i = 0; i < msg->used; i++){
        printf("msg_out[%u] = %x\n", i, msg->array[i]);
    }
    for(size_t i = 0; i < synd->used; i++){
        printf("synd[%u] = %u\n", i, synd->array[i]);
    }

    struct Array *list = malloc(sizeof(struct Array));
    initArray(list,2);
    list->array[0] = 0;
    list->used = 1;
    
    msg = rs_correct_errdata(msg, synd, list, gf_table);
    for(size_t i = 0; i < msg->used; i++){
        printf("msg_out[%u] = %x\n", i, msg->array[i]);
    }*/
    
    return 0;
}
