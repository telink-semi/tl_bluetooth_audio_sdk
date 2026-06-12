#ifndef IO_STREAM_H
#define IO_STREAM_H

enum ioStreamDataType
{
    //16bit short: 0
    INT16_STREAM = 0,
    //24bit int : 1
    INT24_STREAM,
    //32bit float : 2
    FLOAT_STREAM
};

enum ioStreamErrorType
{
    IOSTREAM_SANITY_CHK_ERROR = 20,
    SAMPLE_RATE_ERROR,
    FRAME_SIZE_ERROR,
    CHANNEL_NUM_ERROR,
    DATA_TYPE_ERROR
};

typedef struct
{
    /* support 5ms/7.5ms
	 * 5ms	: 80
	 * 7.5ms: 120 */
    unsigned int frame_size;

    /* 16k / 48k
	16k : 16000
   	48k : 48000*/
    unsigned int sample_rate;

    /* channel number
	range : [1, 8]*/
    unsigned int channel_num;

    /*data type
	16bit short: 0
 	32bit int : 1
 	32bit float : 2
	*/
    unsigned int data_type;

    /*data pointer*/
    void *data;
} ioStream;


#endif
