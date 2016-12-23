#ifndef _SYSTEM_INFO_H_
#define _SYSTEM_INFO_H_

#include <Windows.h>  
#include <TlHelp32.h>  
#include <direct.h>  
#include <winternl.h>  
#include <Psapi.h>  

#include <vector>
#include <string>
#include <list>
#include <sstream>

using std::list;
using std::string;
using std::vector;

/*
/*����˵������ȡϵͳCPU��ǰʹ����
/*����������nCpuRate->CPUʹ����
/*���ؽ�������ؽ��״̬
*/
bool GetSysCpu(int &nCpuRate);

/*
/*����˵������ȡϵͳ�ڴ���Ϣ
/*����������nMemTotal->�ڴ��ܹ���С,(MB)��nMemUsed->�ڴ���ʹ�ô�С,(MB)
/*���ؽ�������ؽ��״̬
*/
bool GetSysMemory(int &nMemTotal, int &nMemUsed);

/*
/*����˵������ȡϵͳ������Ϣ
/*����������nDiskTotal->�����ܹ���С,(MB)��nDiskUsed->������ʹ�ô�С,(MB)
/*���ؽ�������ؽ��״̬
*/
bool GetSysDisk(int &nDiskTotal, int &nDiskUsed);

/*
/*����˵��������ϵͳ���н���
/*������������
/*���ؽ��������PID�ͽ������Ƶ�ӳ�伯��
*/
list<vector<string>> GetAllProcess();

//ͨ��PID��ȡ����ռ�ڴ�
bool GetProcessMemory(int nPid, int &nProcessMemRate);

//ʱ��ת��  
__int64 Filetime2Int64(const FILETIME *ftime);
//����ʱ���������  
__int64 CompareFileTime(FILETIME preTime, FILETIME nowTime);

#endif