//1、支持 其他格式的 音、视频文件转化成flv.
//2、支持 实时的 h264 ,h265视频帧[h265官方不支持,这里只是留出来] + g7x,aac音频帧 打包成 flv格式存储. 

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "lmflv.h"
#include "sps.h"

typedef union
{
	double  t;
	ullong  j;
}db2ll;


static uchar video_type = 0;
static uchar audio_type = 0;
static uchar first = 0;

pos_inf_t p_m;
static FILE *FLVFD = NULL;


f_ty_t fty_m[ ] ={ {"h264", FILE_H264},{"h265", FILE_H265},{"aac", FILE_AAC},
				   {"g711a", FILE_G711A}, {"G711u", FILE_G711A}, {"pcm", FILE_PCM},
				 }; 

void print_tnt(char *buf, int len, const char *comment)
{
	int i = 0;
	printf("\r\n%s start buf:%p len:%d \r\n", comment, buf, len);
	for(i = 0; i < len; i++)
	{
    	 printf("%02x ", buf[i] & 0xff); //&0xff 是为了防止64位平台中打印0xffffffa0 这样的，实际打印0xa0即可
	}
	printf("\r\n");
}

int print_frmae_info(frame_inf_t * inf, int nums)
{
	int i = 0;

	while( i < nums)
	{
		printf("seq:[%d], fm_sz:[%d], fm_pos:[%d], fm_type:[%d]\n", i+1, inf[i].frame_sz, inf[i].frame_pos, inf[i].frame_type );
		i++;
	}
	printf("all frame : [%d]\n", nums);
}

int get_sps_pps(FILE *vfd, SPS_cfg_t *sps, PPS_cfg_t *pps)
{
	int buflen = 1024 *256, i = 0, int data = 0,  offsz = 0;
    char tmp[buflen], nalu_ty = 0, flag = 0x00;
	int ret = fread(tmp, 1, buflen, vfd);
	if(ret <= 0)
	{
		perror("Fread vidoe failed:");
		return -1;
	}
	
	while(i < buflen)
	{
		data = tmp[i] << 24 | tmp[i+1] << 16 | tmp[i+2] << 8;
		if(data != 0x000001)
		{
			data |= tmp[i+3];
			if(data != 0x00000001)
			{
				i++;
				continue;
			}
			else
			{
				nalu_ty = tmp[i+3] & 0x1f;
				offsz = 3;
			}
		}
		else 
		{
			nalu_ty = tmp[i+4] & 0x1f;
			offsz = 4;
		}

		if(nalu_ty == 0x07)
		{
			sps->data = &tmp[i+offsz];
		}
		else if(nalu_ty == 0x08)
		{
			pps->data = &tmp[i+offsz];
			sps->len = pps->data - sps->data - offsz; // get last nalu size: sps size.
			memcpy(sps->data, sps->data, sps->len);
			flag = 1<<1;
		}
		else if(pps)
		{
			pps->len = &tmp[i+offsz] - pps->data - offsz; // get last nalu size: pps size.
			memcpy(pps->data, pps->data, pps->len);
			flag =< 1;
			break;
		}	
	}

	if(flag != 0x02)
	{
		printf("Do't find the sps ,pps , eixt!");
		return -1;
	}
	
	return 0;
}

//暂时没添加解析h265的功能.
int spare_sps(Vcfg_t *v)
{
	char *date = v->sps.data;
	int sz = v->sps.len;
	
	sps_t _sps; 
	char tmpbuf[40] ={'\0'};
	int len = sz;
	float fps = 0.0; 
	int rc = nal_to_rbsp(0, date, &sz, tmpbuf, &len );
	bs_t* b = bs_new(tmpbuf, len);

	//从sps pps中获取信息  
	read_seq_parameter_set_rbsp(b, &_sps); 

	v->width  = h264_get_width(&_sps);  
	v->height = h264_get_height(&_sps);
	fps = h264_get_framerate(&_sps);  
	if(fps < 0.0)    
		v->frameRate = 21;
	
	bs_free(b);
	
	v->AVCProfileIndication = date[1];
	v->profile_compat 	  = date[2];
	v->AVCLevelIndication = date[3];
	v->frameRate = fps;
	v->timeScale = 100000;
	
	return 0;
}

int spare_aac(FILE *fd,  Aac_t * aac)
{
	if(!aac)
	{
		printf("Input aac point is NULL, exit\n");
		return -1;
	}
	
	if(fd <= 0)
	{
		perror("Open file failed :");
		return -1;
	}
	
	char head[9] = {'\0'};
	int sz = fread(head, sizeof(head), 1, fd);
	if(sz > 0)
	{
		if((head[0]&0xff == 0xff) && (head[1]&0xf0 == 0xf0))
		{
			aac->mpeg_ver = (head[1] & 0x08) >> 3;
			aac->profile  = (head[2] & 0xc0) >> 6;
			aac->sampling = (head[2] & 0x3c) >> 2;
			aac->chns 	  = (head[2] & 0x01) << 2 | ((head[3] & 0xc0) >> 6);
		}
	}

	fseek(fd, 0L, SEEK_SET);
}

int get_sampling(char samp_seq)
{
	int samp[] = {96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000, 12000, 11025, 8000};
	if( (samp_seq > sizeof(samp))  || (samp_seq < 0))
	{
		printf("Input sampling que is error, do not rang.\n");
		return -1;
	}
	
	return samp[samp_seq];
}

int pri_aac_inf(Aac_t *aac)
{
	if(!aac)
		return -1;
	
	switch(aac->mpeg_ver)
	{ 
		case 0: printf("mpeg version :""MPEG_4\n");break;
		case 1: printf("mpeg version :""MPEG_2\n");break;
		default:printf("mpeg version :""unknown\n");break;
	}
	
	switch(aac->profile)
	{ 
		case 0: printf("profile      :""Main\n");break;
		case 1: printf("profile      :""LC\n");break;
		case 2: printf("profile      :""SSR\n");break;
		default:printf("profile      :""unknown\n");break;
	}

	int samp = 0;
	switch(aac->sampling)
	{
		case 0:  samp = 96000;break;
		case 1:  samp = 88200;break;
		case 2:  samp = 64000;break;
		case 3:  samp = 48000;break;
		case 4:  samp = 44100;break;
		case 5:  samp = 32000;break;
		case 6:  samp = 24000;break;
		case 7:  samp = 22050;break;
		case 8:  samp = 16000;break;
		case 9:  samp = 12000;break;
		case 10: samp = 11025;break;
		case 11: samp = 8000; break;
		default:printf("sampling     :""unknown\n");break;
	}

	printf("sampling     :""%dHz\n", samp);
	
	switch(aac->chns)
	{ 
		case 0: printf("chns         :""0\n");break;
		case 1: printf("chns         :""1\n");break;
		case 2: printf("chns         :""2\n");break;
		default:printf("chns   :""unknown\n");break;
	}
	
	return 0;
}

void write_flush_file(char *data, int len)
{
	static int size = 0;
	int ret = fwrite(data, 1, len, FLVFD);
	if(ret <= 0)
	{
		perror("Fwrite flv file failed:");
		return -1;
	}
	
	size += len;
	if(size > FLUSH_SZ)
	{
		fflush(FLVFD);
		fsync(FLVFD);
	}

	return 0;
}


int write_video_2_flv(char *wbuf, int len, int *type)
{
	uchar naluType;
    int width = 0, hight = 0, fps = 0;
	char *pNalu = NULL;
	
	if (wbuf[0] != 0 || wbuf[1] != 0 || wbuf[2] != 0 || wbuf[3] != 1)
		return -1;
	
	len -= 4;
	pNalu = wbuf+4;
	naluType = pNalu[0]&0x1F;
	
	switch (naluType)
	{
		case 0x07: 	// SPS
					*type = SPS_FRAME;									
				break;
					
		case 0x08:  // PPS
					*type = PPS_FRAME;	
				break;
			
		default:
				wbuf[0] = (len>>24)&0xFF;
				wbuf[1] = (len>>16)&0xFF;
				wbuf[2] = (len>>8)&0xFF;
				wbuf[3] = (len>>0)&0xFF;
				*type = I_FRAME;
				write_video_frame(naluType, wbuf+4, len, 25 );
			  break;
	}
	
	return 0;
}

int read_aac_frame(char *rbuf, int len, FILE * a_fd)
{
		char aac_head[7] = {0};
		
		int ret  = fread(aac_head, 1, sizeof(aac_head),  a_fd);
		if(ret <= 0 )
		{
			printf("read aac head failed,  exit !\n");
			return -1;
		}

		//printf("[%02x %02x %02x]\n", aac_head[0]&0xff, aac_head[1]&0xff, aac_head[2]&0xff);
		int body_size = (aac_head[3] & 0x03) << 11;
		body_size += aac_head[4] << 3;
		body_size += (aac_head[5] & 0xe0) >> 5;
		
		ret = fread(rbuf, 1, body_size - 7,  a_fd);
		if(ret <= 0 )
		{
			printf("read aac frame body failed,  exit !\n");
			return -1;
		}
		
	return ret;
}

int write_audio_2_flv(char *wbuf, int len, Acfg_t *a)
{
	//print_tnt(wbuf, 12, "audio");
	write_audio_frame(wbuf, len, a);

	return 0;
}

int parse_pps_sps(Vcfg_t *v	)
{
	if(!v)
		return  -1;
	
	SPS_cfg_t  *sps;
	PPS_cfg_t  *pps;

	return 0;
}

uint get_time_ms()
{
	struct timeval _stime;
	
	gettimeofday(&_stime, NULL);
	return (_stime.tv_sec * 1000 + _stime.tv_usec / 1000);
}

int av_file_proc(av_cfg_t *tk)
{	
	//video --- 
	char end_flag = 1;
	int sqe = 0, sz = 0, pos = 0, len = 0;
	int frames = 0, remain = 0, file_sz = 0;

	char *start = NULL;
	char wbuf = malloc(1024 * 720);
	char *vbf = malloc(PER_RD_SZ);
	frame_inf_t  v_frame[MAX_FRMAES];
	memset(&v_frame, 0, sizeof(v_frame));
	memset(wbuf, 0, sizeof(wbuf));

	uint video_tick_gap = (1000.0 + 1.0) / tk->v->frameRate;//计算视频每帧的时间
	
	//audio --- 
	static int szs  = 0;
	char audio_end = 0, video_end = 0;
	int i = 0, re_sz  = 0, offse = 0;
	int a_remain = 0, frame_len = 0;
	int last_frame_len = 0;
	char *aframe = NULL, *abf= NULL;
	uint tick_exp_tmp = 0, tick_exp = 0, audio_tick_gap = 0; 
	uint audio_tick_now, video_tick_now, last_update; 
	int FM_SE = 1024*16, RD_SE = 1024*1024;
	//--------------------------------------

	if(tk->a)
	{
		audio_tick_gap = tk->a->sampleDuration * 1000 / tk->a->timeScale;//计算音频每帧的时间
		printf("[%ld]:[%d]\n", tk->a->sampleDuration, tk->a->timeScale);
		printf("++++[%d]:[%d]++++\n", audio_tick_gap, video_tick_gap);

		aframe = (char *)malloc(FM_SE);
		abf = (char *)malloc(RD_SE);
	}

	video_tick_now = audio_tick_now = get_time_ms();
    while(1)
    {
    	last_update = get_time_ms();
    	//write video 
		if( (tk->hd.v_fd > 0) && (!video_end))
		{
			if(last_update - video_tick_now > video_tick_gap - tick_exp)
			{
					if(end_flag)
					{
						pos = 0;
						if((len = fread(vbf, 1, PER_RD_SZ, tk->hd.v_fd)) <= 0)
						{
							video_end = 1;
							continue;
						}
					}

					while(1)
					{
						if((vbf[pos+0] == 0 && vbf[pos+1] == 0 && vbf[pos+2] == 0 && vbf[pos+3] == 1)
							|| (vbf[pos+0] == 0 && vbf[pos+1] == 0 && vbf[pos+2] == 1))
						{				
							(vbf[pos+2] == 1)?(sqe = 3):(sqe = 4);
							
							if(!start)
							{ //首次检测到帧.
								int pos1 = file_sz + pos;
								v_frame[frames].frame_pos = pos1; //记录帧的在文件的位置, 便于以后查找.
								
								start = &vbf[pos];
								pos += sqe;
								frames++;
								end_flag = 0;
								continue;
							}
							else
							{
								int type = 0;
								int pos2 = file_sz + pos; //加上上次read的长度，得到当前在文件中的位置.
								
								if(remain > 0)
								{
									memcpy(wbuf + remain, &vbf[0], pos); //加上 上次不完整部分数据.
									sz = pos + remain;
									remain = 0; //上次read 剩下的不完整帧.
								}
								else
								{
									sz = pos2 - v_frame[frames - 1].frame_pos;
									memcpy(wbuf, start, sz);
								}
	
								write_video_2_flv(wbuf, sz, tk, &type);

								v_frame[frames].frame_pos    = pos2;
								v_frame[frames-1].frame_type = type;
								v_frame[frames-1].frame_sz   = sz;
								
								memset(wbuf, 0, sizeof(sz));
								start = &vbf[pos];
								frames++;
								pos += sqe;
								end_flag = 0;
							
								continue;
							}
						}
						else if( len == pos)
						{//存下这次read帧中, 剩下的不完整帧数据.
							remain = file_sz + pos - v_frame[frames-1].frame_pos;	
							if(remain > 0)
							{
								int add = v_frame[frames-1].frame_pos - file_sz;
								memset(wbuf, 0, sizeof(sz));
								memcpy(wbuf, &vbf[add] ,remain);
								if(file_sz + pos == tk->v->fsz)
								{ //写最后一帧数据.
									int type = 0;
									write_video_2_flv(wbuf, remain, tk, &type);
				
									v_frame[frames-1].frame_type = type;
									v_frame[frames-1].frame_sz   = remain;
								}
							}

							end_flag = 1;
							file_sz += len;
							break;
						}
						
						pos++;
					}

					video_tick_now = get_time_ms();
			 }
		}

		//write audio. 
		if((tk->hd.a_fd > 0) && (!audio_end))
		{
			if(last_update - audio_tick_now > audio_tick_gap - tick_exp)
			{	

				memset(abf, '\0', RD_SE);
				re_sz = fread(abf, 1, RD_SE, tk->hd.a_fd);
				if( re_sz<= 0)
				{
					printf("read over !\n");
					audio_end = 1;
					continue;
				}
				
				while(1)
				{
					//Sync words: 0xfff;
					if((abf[i]&0xff == 0xff) && (abf[i+1]&0xf0 == 0xf0) )
					{   
						//frame_length: 13 bit
						frame_len = 0;
						frame_len |= (abf[i+3] & 0x03) <<11;   //low    2 bit
						frame_len |= (abf[i+4] & 0xff) <<3;	//middle 8 bit
						frame_len |= (abf[i+5] & 0xe0) >>5;	//hight  3bit
						
						memset(aframe, '\0', FM_SE);
						if(re_sz >= frame_len)
						{
							memcpy(aframe, &abf[i],frame_len);
							write_audio_2_flv(&aframe[7], frame_len -1, tk);
							
							i += frame_len;    //加一帧的长度.
							re_sz -= frame_len;//剩下的帧数据...
							frame_len = 0;
							if(re_sz == 0)
								break;
						}
						else
						{
							a_remain = frame_len - re_sz;
							memcpy(aframe, &abf[i], re_sz); //保存剩下的帧数.
							
							last_frame_len = frame_len;
							offse = re_sz;
							re_sz = 0;
							frame_len = 0;
							i = 0;
							break;
						}
						continue;
					}
					else
					{
						if(a_remain > 0)
						{
							memcpy(aframe + offse, &abf[i], a_remain); //保存剩下的帧数.
							write_audio_2_flv(&aframe[7], last_frame_len -1, tk);
							
							i += a_remain;    //加上次剩下的帧数据的长度.
							re_sz -= a_remain;//接着上次剩下的帧数据...
							a_remain = 0;
							continue;
						}
					}
					i++;
				}   
				audio_tick_now = get_time_ms();
			}
		}

		tick_exp_tmp = get_time_ms();
		tick_exp = tick_exp_tmp - last_update;//计算循环一次的时间间隔.

		if(audio_end && video_end)
			break;

		usleep(800);
    }

	printf("pack end-\n");
	//print_frmae_info(frame, frames);
	
	free(vbf);
	free(wbuf);
	free(abf);
	free(aframe);
	
    return 0;
}

int spare_file_type(const char *infiles[], char (*outfiles)[36])
{
	int filenums = 3;
	if((infiles[1] == NULL) || (infiles[2] == NULL) )
	{
		printf("Input file is NULL, exit\n");
		return -1;
	}

	if((outfiles[0] == NULL) || (outfiles[1] == NULL) )
	{
		printf("out file is NULL, exit\n");
		return -1;
	}

	int ftynum = strlen( fty_m) / sizeof(f_ty_t);
	int i = 0, j = 0;
	char *tmp = NULL;
	while(i < filenums)
	{
		if((tmp = strrchr(infiles[i], '.')))
		{
			tmp++;
			for(j = 0; j < ftynum; j++)
			{	
				if(!strcasecmp(tmp, fty_m[j].fty))
				{
					if( j > FILE_MIN && j <= FILE_H265)
					{
						video_type = fty_m[j].id;
						memcpy(outfiles[0], (char *)infiles[i],  strlen(infiles[i]));
					}
					else if( j > FILE_H265 && j > FILE_MAX)
					{
						audio_type = fty_m[j].id;
						memcpy(outfiles[1], (char *)infiles[i],  strlen(infiles[i]));
					}

				}
				else if(!strcasecmp(tmp, "flv"))
				{
					memcpy(outfiles[2], (char *)infiles[i],  strlen(infiles[i]));
				}
			}	
		}

		i++;
	}

	if(!video_type || !audio_type)
	{
		printf("Input src file, don't  spared the  video or audio file."
			   "please input file such as: xx.h264 or yy.aac，or zz.g711a !\n");
		return -1;
	}
	
	return 0;
}

void fill_int_data(char *dest, int data, int len)
{
	int i = len -1;
	while( i >= 0)
	{
		dest[i--] = data; 
		data >>= 8; 
	}

	dest += len;
}

void fill_double_data(char *dest, double data)
{
	db2ll convt.t = data; 
	ullong val = convt.j; 

	fill_int_data(dest, AMF_DATA_NUMBER, 1);
	fill_int_data(dest, val>>32, 4);
	fill_int_data(dest, val, 4);
}


void fill_str(char *dest, char *data, int len)
{
	int i = 0;
	while(i < len)
	{
		dest[i] = data[i]; 
	}	
}

void fill_str_data(char *dest, char *data)
{
	int len = strlen(data);
	int byte = len <= 0xff ? 1 :(len <= 0xffff ? 2 : 3);
	fill_int_data(dest,  len, byte);
	fill_str(dest, data, len);

	dest += len;
}

//=======================flv file proc=====================
int set_header(  char *data, FILE *fd, uchar stream_ty)
{	
	flv_head_t head;
	int size = sizeof(head);
	//Signature
	head.flag[0] 	= 'F'; 
	head.flag[1] 	= 'L'; 
	head.flag[2] 	= 'V'; 
	head.version 	= 1;
	head.stream_inf = stream_ty;
	head.head_len 	= 9;
	head.reserved   = 0; //

	memcpy(data, head, size);
	
	return size;
}

int set_script_param(char *data, av_cfg_t *av)
{
	flv_tag_t  tag;

	double width = av->v->width;
	double hight = av->v->height; 
	double fps   = av->v->frameRate;
	int  nums = 0;
	char *buf = data;
	char *tp  = buf, *begin = NULL;

	//1、fill script tag head.
	fill_int_data(tp, FLV_TAG_META, 1); // Tag Type "script data"
	begin = tp; //记录data lenth的位置，后面会再填入.
	fill_int_data(tp, 0, 3); 		   // data length
	fill_int_data(tp, 0, 3);  		  // timestamp
	fill_int_data(tp, 0, 1); 	     // ex timestamp
	fill_int_data(tp, 0, 3);       //stream id

	//2、fill script tag data.
	// first    AMF pack: "onMetaData"
	fill_int_data(tp, AMF_DATA_STRING, 1); 
	if(av->v)
		nums +=5;
	if(av->a)
		nums +=5;
	fill_str_data(tp, "onMetaData");

	// second    AMF pack: Meta data array.
	fill_int_data(tp, AMF_DATA_MIXEDARRAY, 1); //AMF包类型
	fill_int_data(tp, 2+nums, 4); //元素的个数

	//下面是元素的封装.
	if(av->v)
	{ 
		fill_str_data(tp, "width");  //PropertyName 
		fill_double_data(tp, width); //PropertyData
		
		fill_str_data(tp, "height");
		fill_double_data(tp, hight);   

		fill_str_data(tp, "framerate");
	    fill_double_data(tp, fps); // written at end of encoding

	    fill_str_data(tp, "videocodecid");
	    fill_double_data(tp, FLV_CODECID_H264);

	   // fill_str_data(tp, "videodatarate");
	   // fill_double_data(tp, 384.00); // written at end of encoding
	}

	if(av->a)
	{ //有音频
		//fill_str_data(tp, "audiodatarate");
		//fill_double_data(tp, audio_enc->bit_rate / 1024.0);

		fill_str_data(tp, "audiosamplerate");
		fill_double_data(tp, av->a->sampleDuration);//sample_rate

		fill_str_data(tp, "audiosamplesize");
		fill_double_data(tp, av->a->bits); //8 or 16.

		fill_str_data(tp, "stereo");
		fill_int_data(tp, av->a->chns);//channels.

		fill_str_data(tp, "audiocodecid");
		fill_int_data(tp, AMF_DATA_STRING);
		fill_str_data(tp, "mp4a");
	}

	fill_str_data(tp, "duration");
	p_m.duration_p = tp;
    fill_double_data(tp, 1.64);

    fill_str_data(tp, "filesize");
	p_m.filesize_p = tp;
    fill_double_data(tp, 0); // written at end of encoding
	    
	//3、fill script tag over. 
   	// over flae:   "00 00 09".
   	fill_int_data(tp, 0, 2); // fill_str_data(tp, "");
    fill_int_data(tp, AMF_END_OF_OBJECT, 1);

	//4、fill  data len. 
	//第一处: 在 填充到 tag head 里的 len, 3个字节: script tag data len.
    uint length = tp - begin; 
    fill_int_data(begin, length, 3);

	//第二处: 在包的最后面, 4个字节: script tag head len + script tag len len.
    fill_int_data(tp, length + 11,  4); //script tag head len 为11 字节.

	return tp - buf;
}

int set_vidoe_tag_sps_pps(char *data, int data_sz, av_cfg_t *av)
{
	char *buf1 = data;
	char *tp1  = buf1, *begin1 = NULL;
	char *sps  = av->v->sps->data, *pps   = av->v->pps->data;
	int sps_sz = av->v->sps->len , pps_sz = av->v->pps->len;

	if(sps || sps_sz > 0)
	{
		//1、fill video tag head.
		fill_int_data(tp1, FLV_TAG_VIDEO, 1); // Tag Type "video data"
		begin1 = tp1; 
		fill_int_data(tp1, 0, 3); 	    // data length // rewrite later
		fill_int_data(tp1, 0, 3);  		// timestamp
		fill_int_data(tp1, 0, 1); 	    // ex timestamp
		fill_int_data(tp1, 0, 3);       // stream id
		
	    fill_int_data(tp1, 7 | FLV_FRAME_KEY, 1); // Frametype and CodecID
	    fill_int_data(tp1, 0, 1); // AVC sequence header
	    fill_int_data(tp1, 0, 3); // composition time

	    fill_int_data(tp1, 1, 1);	   // version   
	    fill_int_data(tp1, sps[1], 1); // sps[1]{profile}
	    fill_int_data(tp1, sps[2], 1); // sps[2]{profile}
		fill_int_data(tp1, sps[3], 1); // sps[3]{level}
		  
	    fill_int_data(tp1, 0xff, 1);   // 6 bits reserved (111111) + 2 bits nal size length - 1 (11)
	    fill_int_data(tp1, 0xe1, 1);   // 3 bits reserved (111) + 5 bits number of sps (00001)

		//3、fill sps len and data.
		fill_int_data(tp1, sps_sz, 2); // sps len
	    fill_str(tp1, sps, sps_sz);    // sps data
		tp1 += sps_sz;
	}
	
	if(pps || pps_sz > 0)
	{
		// PPS    TNT: pps 这里默认只有1个.
	    fill_int_data(tp1, 1, 1); // number of pps
	    fill_int_data(tp1, pps_sz, 2);
	    fill_str(tp1, pps, pps_sz);
		tp1 += pps_sz;
	}

	// rewrite data length info
    uint length = tp1 - begin1; 
    fill_int_data(begin1, length, 3);
    fill_int_data(tp1, length + 11, 4); // Last tag size
	
	return tp1 - buf1;
}

int set_audio_tag_specif_cfg(char *data, int data_sz, Acfg_t *a)
{
	char *buf3 = data;
	char *tp3  = buf3, *begin3 = NULL;
	int sz = 0;
	
	//1、fill audio tag head.
	fill_int_data(tp3, FLV_TAG_AUDIO, 1); // Tag Type "audio data"
	begin3 = tp3; 
	fill_int_data(tp3, 0, 3);		// data length // rewrite later
	fill_int_data(tp3, 0, 3);		// timestamp
	fill_int_data(tp3, 0, 1);		// ex timestamp
	fill_int_data(tp3, 0, 3);		// stream id
	
	fill_int_data(tp3,  a->cfg, 1); // Frametype
	fill_int_data(tp3, 0, 1); 	// pack type: AudioSpecificConfig after.
	
	uint length3 = tp3 - begin3 + sz; 
	fill_int_data(begin3, length3, 3); // rewrite tag body len.

	fill_int_data(tp3, length3 +11, 4);// fill video tag len.

	return 0;
}

int write_video_frame(char naluty, char *frame, int sz, int fps)
{
	char buf2[128] = {'\0'};
	char *tp2 = buf2, *begin2 = NULL;
	static uint dts = 0, cts = 0; //在baseline下的pts和dts一样,cts is 0.
	int offset = cts - dts;
	dts = cts;
	
	//1、fill video tag head.
	fill_int_data(tp2, FLV_TAG_VIDEO, 1); // Tag Type "video data"
	fill_int_data(tp2, 0, 3);		   // data length // rewrite later
	fill_int_data(tp2, cts, 3);		  // timestamp
	fill_int_data(tp2, cts>>8, 1);	 // ex timestamp
	fill_int_data(tp2, 0, 3);		// stream id

	begin2 = tp2; 
	//2、fill frmae info.
    if(naluty == 5)//frame type
        fill_int_data(tp2, FLV_FRAME_KEY, 1);
    else 
		fill_int_data(tp2, FLV_FRAME_INTER, 1);
	//fill encode pack type and offsize.
	fill_int_data(tp2, 1, 1);    //  1:AVC NALU数据; 0:解码器配置{sps,pps}.
    fill_int_data(tp2, offset, 3);// AVC packet为1:cts偏移, 为0:则为0.
	
	//3、fill frame data
	fill_int_data(tp2, sz, 4); // filled frame len.

	uint length2 = tp2 - begin2 + sz + 4; 
	fill_int_data(begin2, length2, 3); // rewrite tag body len.
	write_flush_file(buf2, tp2 - buf2);

	//这里为了避免在应用层组装tag包时，产生的再次拷贝 frame数据的情况
	//单独把frame数据，以及tag包的最后4个字节 一起写到文件.
	fill_int_data(&frame[sz], length2 +11, 4);// fill video tag len.
	write_flush_file(frame, sz + 4);  // filled frame data +  Byte video tag len.

	cts += 90000 /fps; //default is 40ms/fps. 
	return 0;
}

int write_audio_frame(char *frame, int sz, Acfg_t *a)
{
	char *tp4  = frame, *begin4 = NULL;
	Acfg_t *au = av->a;
	int sz = 0;
	
	//1、fill audio tag head.
	fill_int_data(tp4, FLV_TAG_AUDIO, 1); // Tag Type "audio data"
	begin4 = tp4; 
	fill_int_data(tp4, 0, 3);		// data length // rewrite later
	fill_int_data(tp4, 0, 3);		// timestamp
	fill_int_data(tp4, 0, 1);		// ex timestamp
	fill_int_data(tp4, 0, 3);		// stream id

	fill_int_data(tp4, a->cfg, 1); // Frametype
	fill_int_data(tp4, 0, 1); 	// pack type: audio raw data after

	uint length4 = tp4 - begin4 + sz + 4; 
	fill_int_data(begin4, length4, 3); // rewrite tag body len.
	write_flush_file(tp4, tp4 - frame);

	fill_int_data(&frame[sz], length4 +11, 4);// fill audio tag len.
	write_flush_file(tp4, sz + 4); //write sz Byte audio data + 4 Byte len.

	return 0;
}

FILE* create_flv_file(char *filename, char streamty, av_cfg_t *av)
{
	FILE *fd = NULL;
	int  sz = 1024, ret = 0;
	char tmpflv[36] = {'\0'};
	char data[sz] = {'\0'};
	
	if(filename)
		memcpy(tmpflv, filename, strlen(filename));
	else
		strcpy(tmpflv, "tets.flv");
	
	fd = fopen(filename, "wb");
	if(fd <= 0)
	{
		perror("Open new flv file failed:");
		return NULL;
	}
	
	ret += set_header(data, fd, streamty);
	ret += set_script_param(&data[ret], av);
	ret += set_vidoe_tag_sps_pps(&data[ret], sz, av);
	ret += set_audio_tag_specif_cfg(&data[ret], sz, av);

	ret = write_flush_file(data, ret);
	if(ret <= 0)
	{
		printf("Fwrite new flv file failed:");
		return NULL;
	}

	return fd ;
}

int file_init(const char (*files)[36], av_cfg_t *tk)
{
	const char *video_file = files[0];
	const char *audio_file = files[1];
	const char *flv_file   = files[2];

	printf("****[%s]:[%s]:[%s]\n", files[0], files[1]);
	
	//video file
	if(video_file)
	{
		if(!(tk->hd.v_fd = fopen(video_file, "rb")) )
		{
			perror("Open vidoe file faile:");
			return -1;
		}

		tk->v->fty = video_type;

		int buf_len = 1024 *2, sps_len = 0, pps_len =0; 
		struct stat st;
		lstat(video_file, &st);
		tk->v->fsz = st.st_size;

		tk->v->sps.data =(char *)malloc(buf_len);
		tk->v->pps.data =(char *)malloc(buf_len);
		
		get_sps_pps(tk->hd.v_fd, tk->v->sps, tk->v->pps);
		spare_sps(tk->v);
		fseek(tk->hd.v_fd, 0L, SEEK_SET);
	}
	
	//audio file
	if(audio_file)
	{
		if(!(tk->hd.a_fd = fopen(audio_file, "rb")) )
		{
			perror("Open audio file failed:");
			return -1;
		}

		tk->a->fty = audio_type;
		if(audio_type == FILE_AAC)
		{
			Aac_t aac;
			memset(&aac, 0,  sizeof(Aac_t));
	
			spare_aac(tk->hd.a_fd, &aac);
			tk->a->sampleDuration = (aac.chns==2) ? (1024*2) : (1024); //AAC的一帧是 1024个采样点数
			tk->a->timeScale = get_sampling(aac.sampling);
			//在采样率为44100Hz时(即每秒44100个sample),	
			//当前AAC一帧的播放时间[frame_duration]是 = 1024*(1000000/44100) = 22.32ms (单位为ms)
			pri_aac_inf(&aac);

			tk->a->cfg = FLV_CODECID_AAC | FLV_SAMPLERATE_44100HZ | FLV_STEREO | FLV_SAMPLESSIZE_16BIT; //flv 封装 aac 这3个是固定的.; ;
			fseek(tk->hd.v_fd, 0L, SEEK_SET);
		}
	}

	uchar stream = 0
	if(video_type && audio_type)
		stream = 0x05; //有视频又有音频就是0x01|0x04 (0x05)
	else if(video_type)
		stream = 0x01;
		
	//flv file
	FLVFD = create_flv_file(flv_file, stream, tk);
	if(FLVFD == NULL)
	{
		printf("Create flv file failed , exit !\n");
		return -1;
	}

	return 0;
}

int file_destroy(Fhandle_t *hand)
{
	fclose(hand->v_fd);
	fclose(hand->a_fd);
	fclose(FLVFD);
	first = 0;
	
	return 0;
}

int pack_av_by_file_stream(const char (*file)[36], av_cfg_t *tk)
{
	file_init(file, tk);
	av_file_proc(tk);
	file_destroy(&tk->hd);

	return 0;
}

FILE * create_live_flvfile(uchar *filename, uchar stream, av_cfg_t *avm)
{
	if(!filename)
	{
		printf("Input dest flv filename is NULL, exit !\n");
		return -1;
	}
	
	FILE *flvfd = NULL;
	flvfd = create_flv_file(filename, stream, avm);
	if(flvfd == NULL)
	{
		printf("Create flv file failed , exit !\n");
		return NULL;
	}
	
	FLVFD = flvfd;
    return FLVFD;
}

int write_flv_file(const char (*file)[36], int type)
{
	if(type == PK_BY_FILE)
	{
		if(!file[0])
		{
			printf("Input src file can't is empty, exit !\n");
			return -1;
		}

		av_cfg_t tk;
		memset(&tk, 0,  sizeof(tk));

		//video
		if(file[0])
		{
			tk.v = (Vcfg_t *)malloc(sizeof(Vcfg_t));
			memset(tk.v, 0, sizeof(Vcfg_t));
		}
		
		//audio
		if(file[1])
		{
			tk.a = (Acfg_t *)malloc(sizeof(Acfg_t));
			memset(tk.a, 0, sizeof(Acfg_t));
		}

		if(tk.v) //可以没有音频, 必须得有视频.
			pack_av_by_file_stream(file, &tk);

		if(tk.v)
			free(tk.v);
		if(tk.v->sps.data)
			free(tk.v->sps.data);
		if(tk.v->pps.data)
			free(tk.v->pps.data);
		if(tk.a)
			free(tk.a);
	}
	
	return 0;
}
