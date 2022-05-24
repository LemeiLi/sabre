/*************************************************************************
	> Created Time: Wed 04 Sep 2019 02:59:13 PM CST
 ************************************************************************/

#ifndef _PERSISTENCE_H
#define _PERSISTENCE_H

#ifdef __cplusplus
extern "C" {
#endif

extern void read_jpg_file(const char* name, uint8_t** ppdata, size_t* size);
extern int write_jpg_file(const char* name, const void* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif // _PERSISTENCE_H
