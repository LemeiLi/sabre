/*************************************************************************
	> File Name: serial.c
	> Author: XXDK
	> Email: v.manstein@qq.com 
	> Created Time: Wed 06 Nov 2019 11:14:55 AM CST
 ************************************************************************/

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "infra/log/log.h"
#include "infra/utils/exception.h"
#include "infra/serial/serial.h"
#include "infra/stream/stream.h"

static int set_baud(void* pthis, uint32_t baud)
{
	struct termios  oldtio, newtio;
	serial* pthis_ = (serial*)pthis;
	int fd = pthis_->fd;
	if (fd <= 0) {
		log_error("serial %s not opened", pthis_->dev_path);
		return HAL_ESERIAL;
	}

	speed_t bd;
	switch(baud) {
		case 230400:
			bd = B230400;
			break;
		case 115200:
			bd = B115200;
			break;
		case 57600:
			bd = B57600;
			break;
		case 38400:
			bd = B38400;
			break;
		case 9600:
			bd = B9600;
			break;
		default:
			log_error("serial %s unsuport baud %d", pthis_->dev_path, baud);
			return HAL_ESERIAL;
	}

	tcgetattr(fd, &oldtio); ///< get the current option mode parameters
	memset(&newtio, 0, sizeof(newtio));

	fcntl(fd, F_SETFL, 0);  
	/**< 使能接收和设置本地模式 */
	newtio.c_cflag |= (CLOCAL | CREAD);

	/**< 8N1 */
	newtio.c_cflag &= ~(tcflag_t)PARENB;
	newtio.c_cflag &= ~(tcflag_t)CSTOPB;
	newtio.c_cflag &= ~(tcflag_t)CSIZE;
	newtio.c_cflag |= (tcflag_t)CS8;

	/**< 原始数据输入 */
	newtio.c_lflag &= ~(tcflag_t)(ICANON | ECHO | ECHOE | ISIG);

	/**< 原始数据输出 */
	newtio.c_oflag &= ~(tcflag_t)OPOST;

	/**< 解决了接收0x0d却收到0x0a的问题 */
	newtio.c_iflag &= ~(tcflag_t)(ICRNL | INPCK | ISTRIP | IXON | BRKINT );
	newtio.c_iflag = IGNPAR;

	/**< 波特率 */
	cfsetispeed(&newtio, bd);
	cfsetospeed(&newtio, bd);

	/**< set wait time and receive min byte data to return */
	newtio.c_cc[VTIME] = 3;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd, TCIFLUSH); ///< clean the input and output data buffer
	tcsetattr(fd, TCSANOW, &newtio);///< set the new option mode parameters

	return HAL_OK;
}

/**< 清空串口发送与接收缓冲区 */
static int sflush(void* pthis)
{
	serial* pthis_ = (serial*)pthis;
    if (pthis_->fd <= 0) {
        log_error("serial %s not opened", pthis_->dev_path);
        return HAL_ESERIAL;
    }

    tcflush(pthis_->fd, TCIOFLUSH);

	return HAL_OK;
}

static ssize_t tx(void* pthis, const char *wbuf, size_t size)
{
	ssize_t ret = -1;
	serial* pthis_ = (serial*)pthis;

	if (pthis_->fd <= 0) {
        log_error("serial %s not opened", pthis_->dev_path);
        return HAL_ESERIAL;
	}

	ret = writen(pthis_->fd, wbuf, size);
	if (ret == -1) {
		log_error("serial %s tx failed", pthis_->dev_path);
		return HAL_ESERIAL;
	}
	return ret; 
}

static ssize_t rx(void* pthis, char* rbuf, size_t size)
{
	serial* pthis_ = (serial*)pthis;

	if (pthis_->fd <= 0) {
        log_error("serial %s not opened", pthis_->dev_path);
        return HAL_ESERIAL;
	}

	ssize_t ret = readn(pthis_->fd, rbuf, size);
	if (ret < 0) {
		log_error("serial %s rx failed", pthis_->dev_path);
		return HAL_ESERIAL;	
	}

	return ret;
}

static ssize_t rl(void* pthis, char* rbuf, size_t max_len)
{
	serial* pthis_ = (serial*)pthis;

	if (pthis_->fd <= 0) {
        log_error("serial %s not opened", pthis_->dev_path);
        return HAL_ESERIAL;
	}

	ssize_t ret = readline(pthis_->fd, rbuf, max_len);
	if (ret < 0) {
		log_error("serial %s rx failed", pthis_->dev_path);
		return HAL_ESERIAL;	
	}

	return ret;
}

static serial_vmt vmt = {
	.set_baud = set_baud,
	.sflush = sflush,
	.tx = tx,
	.rx = rx,
	.rl = rl,
};

int serial_init(void* pthis, const char *dev_path, uint32_t baudrate)
{
	serial* pthis_ = (serial*)pthis;
	pthis_->fd = open(dev_path, O_RDWR|O_NOCTTY);
	if (pthis_->fd < 0) {
		log_error("open %s failed!", dev_path);
		return HAL_ESERIAL; ///< open serial failed
	} 
	pthis_->baudrate = baudrate;
	strcpy(pthis_->dev_path, dev_path);

	if (set_baud(pthis, baudrate) < 0) return HAL_ESERIAL;

	pthis_->vptr = &vmt;
	pthis_->set_baud = set_baud;
	pthis_->sflush = sflush;
	pthis_->tx = tx;
	pthis_->rx = rx;
	pthis_->rl = rl;

	log_info("serial [%s fd %d baud %d] init success!", 
			 dev_path, pthis_->fd, baudrate);

	return HAL_OK;
}

int serial_deinit(void* pthis)
{
	serial* pthis_ = (serial*)pthis;
	sflush(pthis);
	close(pthis_->fd);
	pthis_->fd = 0;
	log_info("serial %s[%d] closed", pthis_->dev_path, pthis_->baudrate);

	return HAL_OK;
}

serial* construct_serial(const char *dev_path, uint32_t baudrate)
{
	serial* pthis = (serial*)malloc(sizeof(*pthis));
	if (!pthis) {
		log_error("allocate serial object falied: %s", strerror(errno));
		return NULL;
	}
	memset(pthis, 0, sizeof(serial));
	int ret = serial_init(pthis, dev_path, baudrate);
	if (ret) {
		log_error("init serial object falied");
		return NULL;		
	}

	return pthis;
}

int destruct_serial(void* pthis)
{
	if (pthis) {
		serial_deinit(pthis);
		free(pthis);
	}

	return HAL_OK;
}


