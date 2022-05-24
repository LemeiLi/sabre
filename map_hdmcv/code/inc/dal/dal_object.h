
#ifndef _HAL_OBJECT_H
#define _HAL_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#define _base_object_methods \
	void* (*get_object_address)(void* pthis);

#define _base_object_data

struct BaseObjectVMT {
	_base_object_methods
};

typedef struct {
	struct BaseObjectVMT* vmt;
	///< 提供 对象./-> 成员函数 的访问形式
	_base_object_methods 
	_base_object_data
} BaseObject;

extern void BaseObjectInit(void* pthis, void* vmt);

#ifdef __cplusplus
}
#endif

#endif // _HAL_OBJECT_H
