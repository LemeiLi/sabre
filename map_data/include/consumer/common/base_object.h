/*************************************************************************
	> Created Time: Sat 31 Aug 2019 02:10:24 PM CST
 ************************************************************************/

#ifndef _BASE_OBJECT_H
#define _BASE_OBJECT_H

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define bobject_methods size_t instance_offset;
#define bobject_data 

///< virtual methods table
struct bobject_vmt {
	bobject_methods	 
};

///< base object
typedef struct bobject {
	const struct bobject_vmt* vmt;
	bobject_data
}bobject_t;

/**
 * @name    Macro Functions (BaseObject)
 * @{
 */
/**
 * @brief   Returns the instance pointer starting from an interface pointer.
 *
 * @param[in] type  the type of the instance pointer, it is used for casting
 * @param[in] ip    the interface pointer
 * @return          A pointer to the object implementing the interface
 */
 #define bobj_get_instance(type, ip) \
	(type)(((size_t)(ip)) - (ip)->vmt->instance_offset)
/** @} */

#ifdef __cplusplus
}
#endif

#endif // _BASE_OBJECT_H
