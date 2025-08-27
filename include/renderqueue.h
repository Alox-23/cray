#ifdef RENDERQUEUE_H
#define RENDERQUEUE_H

#include "renderobject.h"

typedef struct {
  RenderObject* render_object_array;
  size_t capacity;
  int count;
}RenderQueue;

RenderQueue* renderqueue_create(int initial_capacity);
void renderqueue_clear(RenderQueue* rq);
void renderqueue_sort(RenderQueue* rq);
RenderObject* renderqueue_get_object(RenderQueue* rq);
void renderqueue_destroy(RenderQueue* rq);

#endif
