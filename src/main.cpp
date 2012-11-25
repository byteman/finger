// ctxserial.cpp : Defines the entry point for the console application.
//
#include <cstdio>
#include "FingerMgr.h"
#ifdef WIN32
#include <windows.h>
#define WaitForMs(n) Sleep(n);
#else
#define WaitForMs(n) sleep(n/1000);
#endif

int finger_page = 0;
bool exit_code = false;

void record_finger(CFingerMgr& mgr)
{
	int id = 0;
	printf("********record_finger*********\r\n");
	printf("input id\r\n");
	scanf("%d",&id);
	if (id <= 0)
	{
		printf("error exit\r\n");
		return;
	}
	int retry = 3;
	
	while(retry--)
	{
        int ret = mgr.record_finger(finger_page,id,1);
		if(ret == 0)
		{
			printf("ok id = %d\r\n",id);
            finger_page++;
            if(finger_page>10)finger_page=0;
			break;
		}
		else
		{
			printf("err:%s\n",mgr.get_last_error());
		}
        //WaitForMs(1000);
	}

}

void compare_finger(CFingerMgr& mgr)
{
	int id = -1;
	int retry = 5;

	while(retry--)
	{
		id = mgr.login();
		if(id >= 0)
		{
			printf("login ok id=%d\r\n",id);
			break;
		}else
		{
			printf("login failed\r\n");
		}
        WaitForMs(1000);
		
	}

}

void dump_dev_param(CFingerMgr& mgr)
{
	TDevParam param;
	
	if(ERR_OK == mgr.m_finger_dev.get_dev_pararm(param))
		CFingerMgr::dump_param(param);
	else
	{
		printf("err:%s\n",mgr.get_last_error());
	}

}

/*
����ָ�����ݵ��豸��ָ�ƿ��е�ָ����ҳλ�ã����Ҹ������ݿ�����ԱID��ָ��ҳ��ӳ���ϵ
*/
void download_finger(CFingerMgr& mgr)
{
	printf("***********download finger to device************\n");

	unsigned char buf[1024];
	char path[32] = {0,};
	int id = 0;
	printf("please input worker id\n");
	scanf("%d",&id);
	sprintf(path,"finger_%d.dat",id);
	FILE* fp = fopen(path,"rb");
	if(fp != NULL)
	{
		size_t sz = fread(buf,512,1,fp);
		if(sz == 1)
		{
			int id=0,page=0,version=0;
			printf("please input id & page & version\n");
			scanf("%d %d %d",&id,&page,&version);
			printf("update id[%d] finger data to page[%d]\n ",id,page);
			if(0 != mgr.download_info(id,page,buf,512,version))
			{
				printf("download finger data failed[%s]\n",mgr.get_last_error());
			}
			else
			{
				printf("download finger data ok\n");

			}
		}
		fclose(fp);
	}
}
/*
�ϴ�ָ��λ�õ�ָ�����ݵ�������
*/
void upload_finger(CFingerMgr& mgr)
{
	char path[32]={0};
	unsigned char buf[1024];
	
	int id,page,version;

	printf("***********upload_finger to pc************\n");

	printf("input id\n");
	scanf("%d",&id);

    sprintf(path,"finger_%d.dat",id);

	if(0 == mgr.upload_info(id,buf,1024,page,version))
	{
		FILE *fp = fopen(path,"wb");
		if(fp)
		{
			if(1 == fwrite(buf,512,1,fp))
			{
				printf("upload to %s ok\n",path);
			}
			fclose(fp);
		}
	}
    else
    {
        printf("upload failed\n");
    }

}

void list_all_worker(CFingerMgr& mgr)
{
	printf("\r\n**************woker list***************\n");
	int num = mgr.get_worker_num();

	printf("finger num=%d\n",mgr.get_finger_num());
	for(int i = 0; i < num; i++)
	{
        CWorker w;

        if(mgr.get_worker_by_index(i,w))
		{
            printf("worker[%02d] id=%04d page=%04d version=%04d\n",i+1,w.m_id,w.m_finger_index,w.m_finger_version);
		}
	}
}
//¼ȡָ�ƣ�Ȼ��ָ�������ϴ�������
void gen_feature_up(CFingerMgr& mgr)
{
	unsigned char buf[1024];
	char path[32] = {0,};
	if(0 == mgr.gen_feature_up(buf,1024))
	{
		int id;
		
		printf("***********gen_feature_up to pc************\n");
		
		printf("input id\n");
		scanf("%d",&id);
		
        sprintf(path,"finger_%d.dat",id);
		
		FILE *fp = fopen(path,"wb");
		if(fp)
		{
			if(1 == fwrite(buf,512,1,fp))
			{
				printf("gen_feature_up to %s ok\n",path);
			}
			fclose(fp);
		}
		
	}
	else
	{
		printf("failed to gen_feature_up\n");
		printf("err:%s",mgr.get_last_error());
	}
}

void exitfunc(CFingerMgr& mgr)
{
	
	exit_code = 1; 

}
void clear_fingers(CFingerMgr& mgr)
{
	if(0 == mgr.clear_fingers())
	{
		printf("clear ok\n");
	}
	else
	{
		printf("clear db failed\n");
	}
	
}
void rw_note_test(CFingerMgr& mgr)
{
	ERR_FINGER ret;
	std::string text = "hello world";
	ret = mgr.m_finger_dev.write_note_page(1,(unsigned char*)text.c_str(),text.length());

	if(ERR_OK != ret)
	{
		printf("error:%s\n",CFinger::format_error(ret));
		return;
	}
	char buf[33] = {0,};
	ret = mgr.m_finger_dev.read_note_page(1,(unsigned char*)buf,33);
	if(ERR_OK != ret)
	{
		printf("error:%s\n",CFinger::format_error(ret));
		return;
	}
	std::string wr = buf;

	printf("write = %s read = %s \n",text.c_str(),wr.c_str());
}


typedef void (*TestFunc)(CFingerMgr& mgr);

typedef struct tag_TestItem{
	TestFunc func;
	const char* help;
}TestItem;

static TestItem test_items[] = {
	{record_finger,"¼��ָ�Ʋ���¼�û�ID"},
	{compare_finger, "�û�ָ�Ƶ�¼"},
	{list_all_worker,"�г����������û���Ϣ"},
	{dump_dev_param,"�г�ָ��ģ�����"},
	{download_finger,"����ָ�����ݵ�ָ��ģ����"},
	{upload_finger,"��ָ��ģ�����ϴ����ݵ�����"},
	{gen_feature_up,"¼��ָ�Ʋ����ϴ�ָ�����ݵ�����"},
	{rw_note_test,"��д���±�����"},
	{clear_fingers,"���ָ�����ݿ�"},
	{exitfunc,"�˳�����"}
};


int main(int argc, char* argv[])
{

	CFingerMgr fgm;
#ifdef WIN32
	if(0 != fgm.init("com1",115200))
#else
    if(0 != fgm.init("/dev/ttyUSB0",115200))
#endif
	{
		printf("ָ��ģ���ʼ��ʧ��,���������豸!!\r\n");
		return -2;
	}


	while(!exit_code)
	{
		printf("\r\n***********��ѡ�����ѡ��***********\r\n");

		int max_choice = (sizeof(test_items)/sizeof(TestItem));
		for(int i = 0; i < max_choice; i++)
		{
			printf("[%d]: %s\n",i,test_items[i].help);
		}
		printf("************************************\r\n");

		int ch = 0;
		scanf("%d",&ch);

		if(ch < max_choice)
		{
			test_items[ch].func(fgm);
		}

	}


	
	return 0;
}
