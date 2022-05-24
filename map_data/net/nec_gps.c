/*************************************************************************
	> Created Time: Tue 16 Jul 2019 05:56:53 PM CST
 ************************************************************************/

#include "net/nrb_gps.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

static inline void set_nrb_gps_head(struct nrb_gps_desc* pthis)
{
	pthis->data[0] = 0xFF;	
	pthis->data[1] = 0x01;	
	pthis->data[2] = 0xAA;	
	pthis->data[3] = 0x88;	

	return;
}

static inline void set_nrb_gps_length(struct nrb_gps_desc* pthis, size_t size)
{
	pthis->data[4] = size & 0xFF;	
	pthis->data[5] = (size >> 8)  & 0xFF;
	pthis->data[6] = (size >> 16) & 0xFF;
	pthis->data[7] = (size >> 24) & 0xFF;

	return;
}

static inline void set_nrb_gps_timestamp(struct nrb_gps_desc* pthis, uint64_t ts64)
{
	uint32_t data_times = (uint32_t)(ts64 / 1000000);
	uint32_t data_timeu = (uint32_t)(ts64 % 1000000);

	/**< timestamp 8byte */
	pthis->data[8]  = data_times         & 0xFF;
	pthis->data[9]  = (data_times >> 8)  & 0xFF;
	pthis->data[10] = (data_times >> 16) & 0xFF;
	pthis->data[11] = (data_times >> 24) & 0xFF;
	pthis->data[12] = data_timeu         & 0xFF;
	pthis->data[13] = (data_timeu >> 8)  & 0xFF;
	pthis->data[14] = (data_timeu >> 16) & 0xFF;
	pthis->data[15] = (data_timeu >> 24) & 0xFF;  

	return;
}

static inline void set_nrb_gps_id(struct nrb_gps_desc* pthis, uint32_t id)
{
	pthis->data[16] = id;

	return;
}

static inline void set_nrb_gps_data(struct nrb_gps_desc* pthis, void* pdata, size_t size)
{
	memcpy(&pthis->data[17], pdata, size);
		
	return;
}

static inline void set_nrb_gps_checksum(struct nrb_gps_desc* pthis, size_t size)
{
	uint16_t checksum = check_sum_add16(&pthis->data[16], size);
	pthis->data[16+ size + 1] = checksum & 0xFF;
	pthis->data[16+ size + 2] = (checksum >> 8) & 0xFF;

	return;
}

void nrb_gps_encoder(struct nrb_gps_desc* pthis, void* pdata, size_t data_size)
{
	set_nrb_gps_head(pthis);
	set_nrb_gps_length(pthis, data_size); ///< size + 1 byte id
	set_nrb_gps_timestamp(pthis, 0); ///< force this stamp to 0 
	set_nrb_gps_id(pthis, 1);        ///< TODO id
	set_nrb_gps_data(pthis, pdata, data_size);
	set_nrb_gps_checksum(pthis, data_size);

	return;
}



