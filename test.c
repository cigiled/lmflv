#include "lmflv.h"

#define LOCALFILE


#ifdef LOCALFILE

//test1: 文件格式转化成flv.
int main(int argc, void *argv[])
{
	int ret;
	char files[3][36] = {'\0'};
	
	ret = spare_file_type(argv, files);
	if(ret <0)
	{
		printf("Spare file error , exit !\n");
		return -1;
	}
	
	write_flv_file(files, PK_BY_FILE);
	
	return 0;
}

#else

//test2:实时流存flv文件.
int main(int argc, void *argv[])
{
	int ret;
	char files[36] = {'\0'};
	
	create_live_flvfile(files, PK_BY_REAL_STREAM);
	write_video_2_flv();
	return 0;
}
#endif
