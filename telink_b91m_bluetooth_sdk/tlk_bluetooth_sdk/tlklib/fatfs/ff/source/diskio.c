/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include <stdint.h>
#include <time.h>
#include "../../tlkfs_diskio.h"

/* Definitions of physical drive number for each drive */
#define DEV_MMC		0	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_RAM		1	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

DWORD get_fattime(void)
{
    const struct tm *time =  tlkfs_diskio_getTime();
    uint16_t year = time->tm_year + 1900;
    uint8_t month = time->tm_mon + 1;
    uint8_t day   = time->tm_mday;
    uint8_t hour  = time->tm_hour;
    uint8_t min  = time->tm_min;
    uint8_t sec  = time->tm_sec;
    
    return ((DWORD)(year - 1980) << 25) |\
           ((DWORD)month << 21)         |\
           ((DWORD)day << 16)           |\
           ((DWORD)hour << 11)          |\
           ((DWORD)min << 5)            |\
           ((DWORD)sec >> 1);
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv) {
	case DEV_RAM :
	case DEV_MMC :
	case DEV_USB :
        // The device is ready now !
		stat = 0;
    default:
        break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	switch (pdrv) {
	case DEV_RAM :		
	case DEV_MMC :
	case DEV_USB :
        // The device is ready now !
		return 0;
    
    default:
        break;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{

    if(pdrv != DEV_MMC || count > 1)
    {
        return RES_PARERR;
    }

	return tlkfs_diskio_read(buff, sector, count);
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
    if(pdrv != DEV_MMC || count > 1)
    {
        return RES_PARERR;
    }

    return tlkfs_diskio_write((uint8_t*)buff, sector, count);
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT r = RES_PARERR;
    
    if(pdrv != DEV_MMC)
    {
        return RES_PARERR;
    }

    switch(cmd)
    {
        case CTRL_SYNC:
            // Nothing to do for this command 
            // if each write operation to the medium is completed in the disk_write function.
            r = RES_OK;
            break;
            
        case GET_SECTOR_COUNT:
            
            *((LBA_t*)(buff)) = tlkfs_diskio_getDiskInfo().sector_num;
            r = RES_OK;
            break;
        
        case GET_SECTOR_SIZE:
            //  This command is required only if FF_MAX_SS > FF_MIN_SS.
            break;
            
        case GET_BLOCK_SIZE:
            *((DWORD*)(buff)) = tlkfs_diskio_getDiskInfo().sector_size;
            break;
            
        case CTRL_TRIM:
            // It is required when FF_USE_TRIM == 1.
            break;
        default:
            break;
    }

	return r;
}

