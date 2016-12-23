#include "systemInfo.h"

using namespace std;

__int64 Filetime2Int64(const FILETIME* ftime)
{
	LARGE_INTEGER li;
	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

__int64 CompareFileTime(FILETIME preTime, FILETIME nowTime)
{
	return Filetime2Int64(&nowTime) - Filetime2Int64(&preTime);
}

//�����ֽ�char*ת��Ϊ���ֽ�wchar_t*    
wchar_t* AnsiToUnicode(const char* szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}

//�����ֽ�wchar_t*ת��Ϊ���ֽ�char*    
inline char* UnicodeToAnsi(const wchar_t* szStr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
	return pResult;
}

bool GetSysCpu(int& nCpuRate)
{
	HANDLE hEvent;
	bool res;
	static FILETIME preIdleTime;
	static FILETIME preKernelTime;
	static FILETIME preUserTime;

	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

	preIdleTime = idleTime;
	preKernelTime = kernelTime;
	preUserTime = userTime;

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);//��ʼֵΪnonsignaled  

	WaitForSingleObject(hEvent, 200);//�ȴ�500����  

	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);

	int idle = CompareFileTime(preIdleTime, idleTime);
	int kernel = CompareFileTime(preKernelTime, kernelTime);
	int user = CompareFileTime(preUserTime, userTime);

	nCpuRate = (int)ceil(100.0*(kernel + user - idle) / (kernel + user));

	return res;
}

bool GetSysMemory(int& nMemTotal, int& nMemUsed)
{
	MEMORYSTATUSEX memsStat;
	memsStat.dwLength = sizeof(memsStat);
	if (!GlobalMemoryStatusEx(&memsStat))//�����ȡϵͳ�ڴ���Ϣ���ɹ�����ֱ�ӷ���  
	{
		nMemTotal = -1;
		nMemUsed = -1;
		return false;
	}
	int nMemFree = memsStat.ullAvailPhys / (1024.0*1024.0);
	nMemTotal = memsStat.ullTotalPhys / (1024.0*1024.0);
	nMemUsed = nMemTotal - nMemFree;
	return true;
}

bool GetSysDisk(int& nDiskTotal, int& nDiskUsed)
{
	static char path[_MAX_PATH];//�洢��ǰϵͳ���ڵ��̷�  
	int curdrive = _getdrive();
	unsigned long lFreeAll = 0UL;
	unsigned long lTotalAll = 0UL;
	for (int drive = 1; drive <= 26; drive++)//���������̷�  
	{
		if (!_chdrive(drive))
		{
			sprintf(path, "%c:\\", drive + 'A' - 1);
			ULARGE_INTEGER caller, total, free;
			WCHAR wszClassName[_MAX_PATH];
			memset(wszClassName, 0, sizeof(wszClassName));
			MultiByteToWideChar(CP_ACP, 0, path, strlen(path) + 1, wszClassName,
				sizeof(wszClassName) / sizeof(wszClassName[0]));
			if (GetDiskFreeSpaceEx(wszClassName, &caller, &total, &free) == 0){ return false; }

			double dTepFree = free.QuadPart / (1024.0*1024.0);
			double dTepTotal = total.QuadPart / (1024.0*1024.0);

			lFreeAll += (unsigned long)ceil(dTepFree);
			lTotalAll += (unsigned long)ceil(dTepTotal);
		}
	}
	nDiskUsed = lFreeAll;
	nDiskTotal = lTotalAll;
	return true;
}

list<vector<string>> GetAllProcess()
{
	HANDLE hProcessSnap;    //���̿��յľ��  
	HANDLE hProcess;    //���ڻ�ȡ���̵ľ��  
	PROCESSENTRY32 pe32;//������Ϣ�Ľṹ��  
	list<vector<string>> pidMap;//�������н��̵�PID ,���ֺ�ռ���ڴ� 
							 
    // ��ȡϵͳ������Ϣ�Ŀ���  
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		if (NULL != hProcessSnap)
		{
			CloseHandle(hProcessSnap);          // �رս��̿�����Ϣ  
			hProcessSnap = NULL;
		}
		return pidMap;
	}

	// ��ʹ��֮ǰ����PROCESSENTRY32�ṹ��ĳ�ʼ��Сֵ,�������ʼ��dwSize, Process32First ���û�ʧ��.  
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))// ��ʼ��ȡ��һ�����̵���Ϣ�������ȡʧ�ܾͷ���  
	{
		if (NULL != hProcessSnap)
		{
			CloseHandle(hProcessSnap);          // �رս��̿�����Ϣ  
			hProcessSnap = NULL;
		}
		return pidMap;
	}

	//��ʼ�������н���  
	do
	{
		vector<string> oneProcess;

		//�������֣�PID  ��ռ���ڴ�
		string strProcessName = UnicodeToAnsi(pe32.szExeFile);
		oneProcess.push_back(strProcessName);

		int pid = static_cast<int>(pe32.th32ProcessID);
		stringstream ss; string strpid;
		ss << pid; ss >> strpid; ss.clear();
		oneProcess.push_back(strpid);
		
		int mem = -1;
		GetProcessMemory(pid,mem);
		string strmem;
		ss << mem; ss >> strmem; ss.clear();
		if (strmem == "-1") { strmem = "0"; }
		strmem += "%";
		oneProcess.push_back(strmem);

		pidMap.push_back(oneProcess);
	} while (Process32Next(hProcessSnap, &pe32));// ��ȡ��һ�����̵���Ϣ  

	if (NULL != hProcessSnap)//���رտ��վ��  
	{
		CloseHandle(hProcessSnap);
		hProcessSnap = NULL;
	}
	return pidMap;
}

bool GetProcessMemory(int nPid, int &nProcessMemRate)
{
	HANDLE hProcess;//���̵߳ľ��  
	PROCESS_MEMORY_COUNTERS pmc;//���̵߳��ڴ���Ϣ�ṹ��  
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);//�������Ȩ�޴򿪸��̲߳���þ��  
	if (nullptr == hProcess){ return false; }
	if (!GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))){ return false; }

	int nMemTotal = 0;
	int nMemUsed = 0;
	GetSysMemory(nMemTotal, nMemUsed);
	double tep = ((int)pmc.WorkingSetSize*1.0);
	nProcessMemRate = (int)ceil(100 * (tep / (1024.0*1024.0)) / nMemTotal);

	CloseHandle(hProcess);
	return true;
}