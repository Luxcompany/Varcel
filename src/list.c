#include <stdlib.h>
#include <list.h>

typedef unsigned int uint;

struct List{
    uint size;           //number of elements in list
    uint possibleSize;   //number of elements that can be in memory before expandings
    uint stride;         //number of bytes between consecutive elements
    //Element data after
};

/** @returns size of list in bytes - use this instead of sizeof(list)
 */
static uint list_size_internal(uint stride, uint size){
    return sizeof(struct List) + stride*size;
}

List list_create(uint stride){
    List list = malloc(list_size_internal(stride, 1)); //alloc enough for list data, and one element
    list->size = 0;             //0 elements are initially stored
    list->possibleSize = 1;     //Enough room for 1 element
    list->stride = stride;

    return list;
}

void list_destroy(List list){
    free(list);
}

Element list_append(List *list){
    if(!(*list)) return NULL;
    if((*list)->size == (*list)->possibleSize){
        uint newSize = 2*((*list)->possibleSize);
        List newList = realloc((*list), list_size_internal((*list)->stride, newSize));

        if(!newList){
            newSize = 1+((*list)->possibleSize);
            newList = realloc((*list), list_size_internal((*list)->stride, newSize));

            if(!newList) return NULL;
        }

        *list = newList;
        (*list)->possibleSize = newSize;
    }

    ++((*list)->size);
    return list_get((*list), ((*list)->size)-1);
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

Element list_get(List list, uint index){
    if(index < list->size)
        return (Element)( ((void *)list) + sizeof(struct List) + index*(list->stride) );
    else
        return NULL;
}