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
/*函数说明：获取系统CPU当前使用率
/*函数参数：nCpuRate->CPU使用率
/*返回结果：返回结果状态
*/
bool GetSysCpu(int &nCpuRate);

/*
/*函数说明：获取系统内存信息
/*函数参数：nMemTotal->内存总共大小,(MB)；nMemUsed->内存已使用大小,(MB)
/*返回结果：返回结果状态
*/
bool GetSysMemory(int &nMemTotal, int &nMemUsed);

/*
/*函数说明：获取系统磁盘信息
/*函数参数：nDiskTotal->磁盘总共大小,(MB)；nDiskUsed->磁盘已使用大小,(MB)
/*返回结果：返回结果状态
*/
bool GetSysDisk(int &nDiskTotal, int &nDiskUsed);

/*
/*函数说明：遍历系统所有进程
/*函数参数：无
/*返回结果：进程PID和进程名称的映射集合
*/
list<vector<string>> GetAllProcess();

//通过PID获取进程占内存
bool GetProcessMemory(int nPid, int &nProcessMemRate);

//时间转换  
__int64 Filetime2Int64(const FILETIME *ftime);
//两个时间相减运算  
__int64 CompareFileTime(FILETIME preTime, FILETIME nowTime);

#endif