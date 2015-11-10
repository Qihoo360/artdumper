
inline void PrintError(){
	DWORD dwErr=GetLastError();
	HLOCAL hLocal=NULL;
    if(!FAILED(FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_ALLOCATE_BUFFER,
                            NULL,dwErr,0,(LPSTR)&hLocal,0,NULL))){
		
		//cout<<(LPCSTR)LocalLock(hLocal)<<endl;
		LocalFree(hLocal);
	}
}



PBYTE MapFile(HANDLE hFile,off_t start,off_t size,DWORD flag,LPVOID baseAddr){
	DWORD data_size=GetFileSize(hFile,0);
	if(size>data_size)size=data_size;
		// Create a file-mapping object for the file.
	HANDLE hFileMapping = ::CreateFileMapping(hFile,
			NULL,
			PAGE_EXECUTE_WRITECOPY,
			0, 0,
			NULL);
	if(!hFileMapping){
		PrintError();
	}
	PBYTE data = (PBYTE)::MapViewOfFileEx(hFileMapping, FILE_MAP_COPY|FILE_MAP_EXECUTE, 0,start, size,0);
	if(!data){
		PrintError();
		return 0;
	}
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	DWORD drealpos=(DWORD)baseAddr/systemInfo.dwAllocationGranularity*systemInfo.dwAllocationGranularity;
	off_t realsize=size;
	if(drealpos!=(DWORD)baseAddr){
		realsize+=systemInfo.dwAllocationGranularity;
	}
	PBYTE ptr=(PBYTE)VirtualAlloc((LPVOID)drealpos,realsize, MEM_COMMIT | MEM_RESERVE,PAGE_EXECUTE_READWRITE);
	if(!ptr){
		PrintError();
		return 0;
	}

	CopyMemory(baseAddr,data,size);
	/*DWORD oldProtect;
	if(!VirtualProtect(data,size,PAGE_EXECUTE_WRITECOPY,&oldProtect)){
		PrintError();
	}*/

	UnmapViewOfFile(data);
	CloseHandle(hFileMapping);

	//assert(ptr);
	FlushInstructionCache(GetCurrentProcess(),baseAddr,size);
	return (PBYTE) baseAddr;
}
