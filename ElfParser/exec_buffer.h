
struct ExecBuffer{
	ExecBuffer(){
	}
	LPBYTE ptr;
	DWORD size;
	LPBYTE cur;
	bool Init(DWORD size,LPVOID addr=0){		
		cur=ptr=(LPBYTE)VirtualAlloc(addr,size, MEM_COMMIT | MEM_RESERVE,PAGE_EXECUTE_READWRITE);
		if(!ptr){
			PrintError();
			return false;
		}		
		this->size=size;
		return true; 
	}
	LPBYTE Ptr(){
		return ptr;
	}
	LPBYTE &Cur(){
		return cur;
	}
	ExecBuffer &PushByte(DWORD b){
		return Push((BYTE)b);
	}
	ExecBuffer &Push(BYTE b){
		*Cur()++=b;
		return *this;
	}
	ExecBuffer &Push(void *pb,DWORD size){
		return Push((BYTE*)pb,size);
	}
	ExecBuffer &Push(BYTE *pb,DWORD size){
		while(size--)
		*Cur()++=*pb++;
		return *this;
	}
	LPBYTE Align4(){
		while((DWORD)Cur()&0x3)*Cur()++=0x90;
		return Cur();
	}
	ExecBuffer _Jmp(DWORD tgt){
		DWORD pc=(DWORD)Cur()+5;
		DWORD offset=tgt-pc;
		Push(0xE9).Push((LPBYTE)&offset,4);
		return *this;
	}
	ExecBuffer _Call(DWORD tgt){
		DWORD pc=(DWORD)Cur()+5;
		DWORD offset=tgt-pc;
		Push(0xE8).Push((LPBYTE)&offset,4);
		return *this;
	}
	ExecBuffer _Push(DWORD val){
		Push(0x68).Push((LPBYTE)&val,4);
		return *this;
	}
	
	void Flush(){
		FlushInstructionCache(GetCurrentProcess(),(LPBYTE)Ptr(),size);
	}
};