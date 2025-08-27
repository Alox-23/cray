#include "../include/renderqueue.h"
#include "../include/renderobject.h"
#include <stdlib.h>

RenderQueue* renderqueue_create(int initial_capacity){
  RenderQueue* rq = malloc(sizeof(RenderQueue));
  if (!rq){
    printf("Unable to dynamicly allocate memory for renderqueue in create\n");
    return NULL;
  }
  
  rq->capacity = initial_capacity;
  rq->count = 0;

  rq->render_object_array = malloc(initial_capacity * sizeof(RenderObject));

  return rq;
}

void renderqueue_clear(RenderQueue* rq){
  if (!rq){
    printf("Invalid renderqueue pointer passed to clear\n");
    return;
  }

  rq->count = 0;
}

void renderqueue_sort(RenderQueue* rq){
  if (!rq){
    printf("Invalid renderqueue pointer passed to sort\n");
    return;
  }

  qsort(rq->render_object_array, rq->count, sizeof(RenderObject), compare_render_object);
}

RenderObject* renderqueue_get_object(RenderQueue* rq){
  if (!rq){
    printf("Invalid renderqueue pointer passed to get_object\n");
    return NULL;
  }


  if (rq->count+1 >= rq->capacity){
    printf("Out of Memory in render queue: %li/%li\n", rq->count, rq->capacity);
    return NULL;
  }

  rq->count+=1;
  
  return &rq->render_object_array[rq->count];
}

void renderqueue_destroy(RenderQueue* rq){
  if (!rq){
    printf("Invalid renderqueue pointer passed to destroy\n");
    return;
  }
  
  free(rq->render_object_array);
  rq->render_object_array = NULL;
  free(rq);
}
