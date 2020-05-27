typedef struct 
{
    unsigned char * data;
    unsigned char * begin;
    unsigned char * end;
    unsigned long size;
} 
queue;

void queue_construct(queue * const q, 
                     unsigned long const size)
{
    q->data = (unsigned char *)malloc(size * sizeof(unsigned char));
    q->begin = q->data;
    q->end = NULL;
    q->size = size;
}

void queue_destruct(queue * const q)
{
    free((void *)(q));
}

unsigned char queue_empty(queue * const q)
// returns non-zero if the argument q is empty.
// A queue q is considered empty if the end 
// of the queue is NULL. 
{
    return q->end == NULL;
}

unsigned long queue_size(queue * const q)
// returns the size of the argument q.
{
    return q->size;
}

unsigned long queue_abs(long const val)
// returns the absolute value of argument val.
{
    long ret = val;
    if(ret < 0)
    {
        ret *= -1;
    }
    return (unsigned long)ret;
}

unsigned char queue_full(queue * const q)
// returns non-zero if the argument q is full. 
// A queue q is considered full if incrementing
// the end of the q will hit the beginning
// of the q or the absolute value of the difference
// between the end and begining is greater than or
// equal to the size.
{
    return q->end + 1 == q->begin 
            || (queue_abs(q->end - q->begin) + 1) == q->size;
    // note there are some dangerous edge cases here.
}

void queue_push(queue * const q,
                unsigned char data)
// push argument data into an argument queue q. 
{
    if(queue_full(q))
    {
        return;
    }
    else if(queue_empty(q))
    {
        q->begin = q->data;
        q->end = q->data;
        *q->end = data;
    }
    else
    {
        ++q->end;
        if(q->end >= q->data + q->size)    // out of bounds
        {
            q->end = q->data;
        }
        *q->end = data;
    }
}

unsigned char queue_pop(queue * const q)
{
    unsigned char ret = 0x00;
    if(queue_empty(q))
    {
        return ret;
    }
    else
    {
        ret = *q->begin;
        ++q->begin;
        if(q->begin > q->end)
        {
            q->end = NULL;
        }
        if(q->begin >= q->data + q->size)  // out of bounds
        {
            q->begin = q->data;
        }
    }
    return ret;
}
