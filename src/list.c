#include <stdlib.h>
#include <list.h>

struct List{
    int size;           //number of elements in list
    int possibleSize;   //number of elements that can be in memory before expandings
    int stride;         //number of bytes between consecutive elements
    //Element data after
};

/** @returns size of list in bytes - use this instead of sizeof(list)
 */
static int list_size_internal(int stride, int size){
    return sizeof(struct List) + stride*size;
}

List list_create(int stride){
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
        int newSize = 2*((*list)->possibleSize);
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

int list_size(List list){
    return list->size;
}

int list_possible_size(List list){
    return list->possibleSize;
}

int list_stride(List list){
    return list->stride;
}

Element list_get(List list, int index){
    if(index < list->size)
        return (Element)( ((void *)list) + sizeof(struct List) + index*(list->stride) );
    else
        return NULL;
}
