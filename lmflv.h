#ifndef __LM_FLV_H__
#define __LM_FLV_H__

#ifdef __cplusplus
	extern "C"
	{
#endif

#define MAX_FRMAES  			(1024*256)
#define PER_RD_SZ				(1024*1024)
#define FLUSH_SZ                (1024*512)

#define AMF_END_OF_OBJECT   	0x09
/* offsets for packed values */
#define FLV_AUDIO_SAMPLESSIZE_OFFSET 1
#define FLV_AUDIO_SAMPLERATE_OFFSET  2
#define FLV_AUDIO_CODECID_OFFSET     4
	
#define FLV_VIDEO_FRAMETYPE_OFFSET   4
	
/* bitmasks to isolate specific values */
#define FLV_AUDIO_CHANNEL_MASK    0x01
#define FLV_AUDIO_SAMPLESIZE_MASK 0x02
#define FLV_AUDIO_SAMPLERATE_MASK 0x0c
#define FLV_AUDIO_CODECID_MASK    0xf0
	
#define FLV_VIDEO_CODECID_MASK    0x0f
#define FLV_VIDEO_FRAMETYPE_MASK  0xf0
	
#define ushort  unsigned short 
#define uchar   unsigned char
#define uint  	unsigned int
#define ullong  unsigned long long int 


#define AVI_VIDEO_FLAG			"00dc"
#define AVI_AUDIO_FLAG			"01wb"

enum
{
	PK_BY_FILE = 0,
	PK_BY_REAL_STREAM
}PK_type_t;

enum
{
    FLV_HEADER_FLAG_HASVIDEO = 1,
    FLV_HEADER_FLAG_HASAUDIO = 4,
};

enum
{
    FLV_TAG_AUDIO = 0x08,
    FLV_TAG_VIDEO = 0x09,
    FLV_TAG_META  = 0x12,
};

enum
{
    FLV_MONO   = 0,
    FLV_STEREO = 1,
};

enum
{
    FLV_SAMPLESSIZE_8BIT  = 0,
    FLV_SAMPLESSIZE_16BIT = 1 << FLV_AUDIO_SAMPLESSIZE_OFFSET,
};

enum {
    FLV_CODECID_PCM                  = 0,
    FLV_CODECID_ADPCM                = 1 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_MP3                  = 2 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_PCM_LE               = 3 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_NELLYMOSER_8KHZ_MONO = 5 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_NELLYMOSER           = 6 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_AAC                  = 10<< FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_SPEEX                = 11<< FLV_AUDIO_CODECID_OFFSET,
};

enum {
    FLV_CODECID_H263    = 2,
    FLV_CODECID_SCREEN  = 3,
    FLV_CODECID_VP6     = 4,
    FLV_CODECID_VP6A    = 5,
    FLV_CODECID_SCREEN2 = 6,
    FLV_CODECID_H264    = 7,
};

enum
{
    FLV_SAMPLERATE_SPECIAL = 0, /**< signifies 5512Hz and 8000Hz in the case of NELLYMOSER */
    FLV_SAMPLERATE_11025HZ = 1 << FLV_AUDIO_SAMPLERATE_OFFSET,
    FLV_SAMPLERATE_22050HZ = 2 << FLV_AUDIO_SAMPLERATE_OFFSET,
    FLV_SAMPLERATE_44100HZ = 3 << FLV_AUDIO_SAMPLERATE_OFFSET,
};
	
enum {
    FLV_CODECID_H263    = 2,
    FLV_CODECID_SCREEN  = 3,
    FLV_CODECID_VP6     = 4,
    FLV_CODECID_VP6A    = 5,
    FLV_CODECID_SCREEN2 = 6,
    FLV_CODECID_H264    = 7,
};	

enum
{
    FLV_FRAME_KEY   = 1 << FLV_VIDEO_FRAMETYPE_OFFSET | 7,
    FLV_FRAME_INTER = 2 << FLV_VIDEO_FRAMETYPE_OFFSET | 7,
};

typedef enum
{
    AMF_DATA_NUMBER      = 0x00,
    AMF_DATA_BOOL        = 0x01,
    AMF_DATA_STRING      = 0x02,
    AMF_DATA_OBJECT      = 0x03,
    AMF_DATA_NULL        = 0x05,
    AMF_DATA_UNDEFINED   = 0x06,
    AMF_DATA_REFERENCE   = 0x07,
    AMF_DATA_MIXEDARRAY  = 0x08,
    AMF_DATA_OBJECT_END  = 0x09,
    AMF_DATA_ARRAY       = 0x0a,
    AMF_DATA_DATE        = 0x0b,
    AMF_DATA_LONG_STRING = 0x0c,
    AMF_DATA_UNSUPPORTED = 0x0d,
} AMFDataType;


//注:官方版本不支持h265, 想用 FLV 封装 H.265 数据:
//   首先, 需要一套 H.265 的编解码器;
//   其次, 就是扩展 FLV 的头 header,增加对 H.265 CodecID 的支持.

/*
	audio support list:
		0 = Linear PCM, platform endian
		1 = ADPCM
		2 = MP3
		3 = Linear PCM, little endian
		4 = Nellymoser 16-kHz mono
		5 = Nellymoser 8-kHz mono
		6 = Nellymoser
		7 = G.711 A-law logarithmic PCM
		8 = G.711 mu-law logarithmic PCM
		9 = reserved
		10 = AAC
		11 = Speex
		14 = MP3 8-Khz
		15 = Device-specific sound
*/

enum
{
	FILE_MIN = 0,
	FILE_H264,
	FILE_H265, //需要解码端支持.
	FILE_AAC, 
	FILE_G711A, 
	FILE_G711U, 
	FILE_PCM, 
	FILE_MAX
};

enum
{
	SPS_FRAME,
	PPS_FRAME,
	I_FRAME,
	P_FRAME,
	B_FRAME,
};

typedef struct 
{
	FILE * v_fd;
	FILE * a_fd;
}Fhandle_t;

typedef struct
{
	char* data;
	uint  len;
}SPS_cfg_t;

typedef struct
{
    char* data;
	uint  len;
}PPS_cfg_t;

typedef struct 
{
	uint 	  frameRate; //为了方便动态修改,而不只是从nalu中获取.
	uint	  timeScale;
	ushort	  width;
	ushort	  height;
	uchar 	  AVCProfileIndication;
	uchar 	  profile_compat;
	uchar 	  AVCLevelIndication;
	uchar 	  sampleLenFieldSizeMinusOne;

	uchar	  fty;
	uint 	  fsz; //PK_BY_FILE模式下为vidoe文件的大小，PK_BY_FILE模式则为-1.
	SPS_cfg_t sps;
	PPS_cfg_t pps;
}Vcfg_t;

typedef struct 
{
	uchar fty;
	uchar bits;
	uchar chns;
	uchar audioType;
	
	uint  timeScale;
	uint  sampleDuration;
	uchar *pConfig;
	uint  configSize;

	uchar cfg;
}Acfg_t;

typedef struct 
{
	Fhandle_t  hd;
	Vcfg_t *v;
	Acfg_t *a;
}av_cfg_t;


typedef struct
{
	char flag[3];
	char version;
	char stream_inf;
	int  head_len;
	int  reserved;
}flv_head_t;

typedef struct
{
	char tag_ty;
	char tag_len[3];
	char timestamp[3];
	char extimestamp;
	char stream_id[3];
}flv_tag_t;

typedef struct
{
	ullong framerate_p;
	ullong duration_p;
	ullong filesize_p;
	ullong bitrate_p;
}pos_inf_t;


typedef struct
{
	int frame_pos; //能够记录1024 *256帧的位置.
	int frame_sz;  //记录帧的大小.
	int frame_type;
}frame_inf_t;

enum
{
	AP_AAC_MAIN = 0,
	AP_AAC_LC,
	AP_AAC_SSR
}Aac_profile;

enum
{
	SP_96000 = 0, 
	SP_88200,
	SP_64000,
	SP_48000,
	SP_44100,
	SP_32000,
	SP_24000,
	SP_22050,
	SP_16000,
	SP_12000,
	SP_11025,
	SP_8000,
	SP_7350, //12: 
	SP_MAX,
}Aac_sampling;

typedef struct
{
	char mpeg_ver;
	char profile;
	char sampling;
	char chns;
}Aac_t;

typedef struct
{
	char *fty;
	char id;
}f_ty_t;

int spare_file_type(const char *infiles[], char (*outfiles)[36]);

FILE* create_live_flvfile(uchar *filename, uchar stream);
int  write_video_2_flv(char *wbuf, int len, int *type);
int  write_audio_2_flv(char *wbuf, int len, Acfg_t *a);



#ifdef
	}
#endif

#endif
