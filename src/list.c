#include <stdlib.h>
#include <list.h>

struct List{
    uint size;           //number of elements in list
    uint possibleSize;   //number of elements that can be in memory before expandings
    uint stride;         //number of bytes between consecutive elements
    char *data;
};

List list_create_stride(uint stride){
    List list = malloc(sizeof(struct List)); //alloc list so it can be freed later
    list->size = 0;             //0 elements are initially stored
    list->possibleSize = 1;     //Enough room for 1 element is made
    list->stride = stride;
    list->data = malloc(stride);    //make room for 1 element

    return list;
}

void list_destroy(List list){
    free(list->data);
    free(list);
}

Element list_append(List list){
    if(!list) return NULL;

    if(list->size == list->possibleSize){
        uint newSize = 2*(list->possibleSize);
        char *newData = realloc(list->data, newSize*(list->stride));

        if(!newData){
            newSize = 1+(list->possibleSize);
            newData = realloc(list->data, newSize*(list->stride));

            if(!newData) return NULL;
        }

        list->data = newData;
        list->possibleSize = newSize;
    }

    ++(list->size);
    return list_get_ptr(list, (list->size)-1);
}

uint list_size(List list){
    return list->size;
}

uint list_possible_size(List list){
    return list->possibleSize;
}

uint list_stride(List list){
    return list->stride;
}

Element list_get_ptr(List list, uint index){
    return (Element)( list->data + index*(list->stride) );
}

void *list_ptr(List list){
    return (void *)(list->data);
}