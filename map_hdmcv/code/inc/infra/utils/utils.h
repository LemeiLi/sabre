/*************************************************************************
 * @file utils.h
 * @brief This provide API for signal handler and some tools.
 * @author lijianhua@momenta.ai
 * @date Tue 09 Apr 2019 03:44:49 PM CST
 ************************************************************************/

#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TIC \
	struct timeval time1, time2; \
    gettimeofday(&time1, NULL); \

#define TOC \
	    gettimeofday(&time2, NULL); \
    double elapsed_time = (time2.tv_sec - time1.tv_sec) * 1000. + \
                (time2.tv_usec - time1.tv_usec) / 1000.; \
    fprintf(stdout, "Elapsed time: %lf(ms)\n", elapsed_time); \

/**
 * @brief  container_of - get the struct for this entry
 * @param[in] ptr:	  the &struct member's pointer.
 * @param[in] type:	  the type of the struct this member embedded in.
 * @param[in] member: the name of the member within the struct.
 */
#define container_of(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define DO_PRAGMA(x) _Pragma (#x)
#define TODO(x) DO_PRAGMA(message ("TODO - " #x))

#ifndef min
#define min(a, b) ((a) <= (b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a) >= (b) ? (a) : (b))
#endif

#ifdef __GNUC__
#define likely(cond)   __builtin_expect(!!(cond), 1)
#define unlikely(cond) __builtin_expect(  (cond), 0)
#define CONSTEXPR __attribute__((const))
#define PURE      __attribute__((pure))
#else
#define likely(cond)   (cond)
#define unlikely(cond) (cond)
#define CONSTEXPR
#define PURE
#endif

#define DEBUG 1
#define WARNING 2
#define ERROR 3

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

#define exit_if(ret, ...)                                                                          \
    if (ret) {                                                                                     \
        fprintf(stderr, __VA_ARGS__);                                                                     \
        fprintf(stderr, "->>> %s: %d error no: %d error msg %s\n", __func__, __LINE__, errno, strerror(errno)); \
        exit(1);                                                                                 \
    }

#define logger(...) do{\
	/*fprintf(stdout, "<%s, %s(), %d>: \n", __FILENAME__, __func__, __LINE__);*/ \
	fprintf(stdout,">>>> ");\
	fprintf(stdout, __VA_ARGS__);}while(0)  

#define error(...) do{\
	fprintf(stderr, "<%s, %s(), %d>: ", __FILENAME__, __func__, __LINE__);\
	fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, ">>>>: %s\n", strerror(errno));}while(0)  

/**
 * @brief Get CTRL+C pressed flag.
 * @return 
 * @retval 0 not pressed.
 * @retval 1 pressed.
 */
extern uint8_t get_exit_flag(void);
extern void set_exit_flag(void);
extern void clear_exit_flag(void);

/**
 * @brief SIG_INT handler.
 */
extern void signal_stop(int sig);

/**
 * @brief SIG_PIPE handler.
 */
extern void signal_pipe(int sig);

/**
 * @brief SIG_PIPE handler.
 */
extern void signal_bus(int sig);

/**
 * @brief SIG_PIPE handler.
 */
extern void signal_segv(int sig);

/**
 * @brief Get current cpu count.
 */
extern int get_cpu_count(void);

/**
 * @brief Set no blocking.
 * @param[in] fd file descriptor.
 */
extern int set_non_blocking(int fd);

extern void update_events(int efd, int fd, uint32_t events, int op);

/**
 * @brief Set current thread cpu affinity.
 * @param[in] pattr     The thread attributer pointer.
 * @param[in] cpu_index The cpu index.
 */
extern int set_thread_cpu_affinity(pthread_attr_t* pattr, int cpu_index);

extern uint8_t check_sum_add8(const uint8_t buf[], size_t size);
extern uint16_t check_sum_add16(const uint8_t buf[], size_t size);

//异或校验运算
extern uint8_t check_sum_xor8(const uint8_t buf[], size_t size);
/**
 * @brief evaluate shift number
 */
extern uint8_t bit2shift(uint32_t x);

#ifdef __cplusplus
}
#endif

#endif // _UTILS_H
