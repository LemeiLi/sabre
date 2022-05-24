#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <stdbool.h>
#include <errno.h>
#include "infra/utils/exception.h"
#include "infra/log/log.h"
#include "infra/net/udp_raw.h"

/**< TODO: this code not test */
static ssize_t udp_raw_send_data(void* pthis, void* data, size_t size) 
{
	ssize_t ret;
	struct udp_raw* pthis_ = (struct udp_raw*)pthis;
	ret = sendto(pthis_->super.fd, data, size, MSG_CONFIRM, 
		   (const struct sockaddr *)&pthis_->clt_addr,  
		   sizeof(pthis_->clt_addr));

	return ret;
}

static ssize_t udp_raw_recv_data(void* pthis, void* buf, size_t size) 
{
	ssize_t ret;
	struct udp_raw* pthis_ = (struct udp_raw*)pthis;

	ret = recvfrom(pthis_->super.fd, buf, size, 0, NULL, NULL);
	//log_info("udp_raw: recv %ld [%s:%d]", ret, 
	//		   inet_ntoa(pthis_->clt_addr.sin_addr), 
	//		   ntohs(pthis_->clt_addr.sin_port));
	
	return ret;
}

static net_state udp_raw_get_state(void* pthis)
{
	struct net* pthis_ = (struct net*)pthis;
	return pthis_->state;
}

#define MAC_SIZE	(18)

static int udp_raw_filter(const uint8_t *src_mac, void* raw)
{
	int ret = 0;
	uint8_t shost_mac[18] = {0};

	/**< Header structures: 以太网头 [ 6 dest_mac + 6 src_mac + 2 type = 12 bytes ]*/
	struct ether_header *eh = (struct ether_header *) raw;
	/**< Header structures: ip头
	 * [ 1 version + 1 dsf + 2 total_len + 2 id + 2 flag + 1 ttl 
	 *   + 1 protocol + 2head_check + 4src_ip + 4dest_ip = 20 bytes ] 
	 */
#if 0	
	struct iphdr *iph = (struct iphdr *) (raw + sizeof(struct ether_header));
	/**< Header structures: udp头 [ 2 src_port + 2 dest_port + 2 len + 
	 *                              2 checksum  = 8 bytes ] 
	 */
	struct udphdr *udph = (struct udphdr *) (raw + sizeof(struct ether_header) + 
			              sizeof(struct iphdr));
#endif
	/**< Check the packet is for me */
	snprintf((char* restrict)shost_mac, MAC_SIZE, "%02x:%02x:%02x:%02x:%02x:%02x",
             eh->ether_shost[0], eh->ether_shost[1],
		     eh->ether_shost[2], eh->ether_shost[3],
		     eh->ether_shost[4], eh->ether_shost[5]);

	if (strncmp((const char *)shost_mac, (const char *)src_mac, 17) == 0) {
		log_info("Package correct destination MAC address for me");
	} else {
		log_warn("Package destination MAC: %x:%x:%x:%x:%x:%x, not for me\n",
				 eh->ether_dhost[0], eh->ether_dhost[1], eh->ether_dhost[2],
				 eh->ether_dhost[3], eh->ether_dhost[4], eh->ether_dhost[5]);
		ret = -1;
		goto done;
	}

done:
	return ret;

}

static struct udp_raw_vmt vmt = {
	.send_data = udp_raw_send_data,
	.recv_data = udp_raw_recv_data,
	.get_state = udp_raw_get_state,
	.raw_filter = udp_raw_filter,
};

static int udp_raw_init(void* pthis, const uint8_t *itf, 
		                const uint8_t* ip, uint16_t port, bool io_mtplx)
{
	int sockfd, sockopt;
	struct ifreq ifopts;	/**< set promiscuous mode */
	struct ifreq if_ip;	    /**< get ip addr */
	char if_name[IFNAMSIZ];
	struct udp_raw* pthis_ = (struct udp_raw*)pthis;
	memset(&if_ip, 0, sizeof(struct ifreq));

	net_init(&pthis_->super, &vmt, NULL, NULL, ip, port, io_mtplx);
	pthis_->vptr = &vmt;

	pthis_->super.state = NETS_CONNECTED;

	/**< 1. Open PF_PACKET socket, listening for EtherType ETHER_TYPE (IPv4) */
	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1) {
		log_error("listener: socket %s", strerror(errno));	
		return HAL_EUDPRAW;
	}

	/**< 2. Set interface to promiscuous mode - do we need to do this every time? */
	strcpy((char *restrict)if_name, (const char *restrict)itf);
	strncpy((char *restrict)ifopts.ifr_name, (const char *restrict)if_name, IFNAMSIZ-1);
	ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
	ifopts.ifr_flags |= IFF_PROMISC;
	ioctl(sockfd, SIOCSIFFLAGS, &ifopts);

	/**< 3. Allow the socket to be reused - incase connection is closed prematurely */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, 
				   sizeof sockopt) == -1) {
		log_error("setsockopt: %s", strerror(errno));
		close(sockfd);
		return HAL_EUDPRAW;
	}

	/**< 4. Bind to device */
	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, if_name, 
				   IFNAMSIZ-1) == -1) {
		log_error("bind to %s error: %s", if_name, strerror(errno));
		close(sockfd);
		return HAL_EUDPRAW;
	}
	pthis_->super.fd = sockfd; 

	pthis_->send_data = pthis_->vptr->send_data;
	pthis_->recv_data = pthis_->vptr->recv_data;
	pthis_->get_state = pthis_->vptr->get_state;

	/**< use super class default realization */
	pthis_->close = pthis_->super.close;        
	pthis_->get_ip = pthis_->super.get_ip;      
	pthis_->get_port = pthis_->super.get_port;  

	pthis_->connect_server = pthis_->vptr->connect_server;
	pthis_->raw_filter = pthis_->vptr->raw_filter;

	return HAL_OK;
}

/**< A.1.1.3 */	
struct udp_raw* construct_udp_raw(const uint8_t *itf, const uint8_t *ip, 
		                          uint16_t port, bool io_mtplx)
{
	struct udp_raw* pthis = (struct udp_raw*)malloc(sizeof(*pthis));
	if (!pthis)	return NULL;
	if (!itf) return NULL;
	if (!ip) return NULL;

	/**< A.1.1.3.1 no check ret */	
	if (udp_raw_init(pthis, itf, ip, port, io_mtplx)) {
		free(pthis);
		return NULL;
	}
	log_info("construt_udp_raw: <%p> [%s %s:%d]", pthis, itf, ip, port);

	return pthis;
}

void destruct_udp_raw(void* pthis)
{
	struct udp_raw* pthis_ = (struct udp_raw*)pthis;
	log_info("destruct_udp_raw: <%p> [%s:%d]", 
			 pthis_, pthis_->get_ip(pthis), pthis_->get_port(pthis));
	free(pthis);
}




