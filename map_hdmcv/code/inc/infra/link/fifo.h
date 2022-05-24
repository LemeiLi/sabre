#pragma once /* If your compiler doesn't support this construct, feel free to
                use include guards. Most do, so I'm keeping it in. */
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define PURE                __attribute__((pure))
#define MALLOC_LIKE         __attribute__((malloc))
#define NO_NULL_POINTERS    __attribute__((nonnull))
#define WARN_UNUSED_RESULT  __attribute__((warn_unused_result))
#elif _WIN32
/* Feel free to fill in results for more compilers =) */
#define PURE
#define MALLOC_LIKE
#define NO_NULL_POINTERS
#define WARN_UNUSED_RESULT 
#if (_MSC_VER == 1900)
    #define restrict __restrict
#endif
#endif

#define FIFO_THRESHOLD 500
typedef struct node_t {
    struct node_t* next;
    void* data;    
}node_t;

typedef struct fifo {
    struct node_t* rear; 
    struct node_t* front; 
    pthread_mutex_t mutex;
    pthread_cond_t cond;
	volatile uint32_t nr_active;
}fifo_t;

extern fifo_t* WARN_UNUSED_RESULT fifo_new(void);
extern int NO_NULL_POINTERS fifo_push(fifo_t*, void*);
extern void* NO_NULL_POINTERS WARN_UNUSED_RESULT fifo_pop(fifo_t* fifo, int32_t timeout_sec);
extern int WARN_UNUSED_RESULT fifo_is_empty(fifo_t*);
extern void NO_NULL_POINTERS fifo_free(fifo_t*);
extern uint32_t NO_NULL_POINTERS get_nr_active(fifo_t* fifo);

#undef WARN_UNUSED_RESULT
#undef NO_NULL_POINTERS
#undef MALLOC_LIKE
#undef PURE

#ifdef __cplusplus
}
#endif

