

class File{
	
// This header file defines the class Elf_file which can be used to
// read useful data from an ELF file.  The functions here are all
// templates which take a file interface object as a parameter.  This
// type must have a subtype View.  This type must support two methods:
//     View view(off_t file_offset, off_t data_size)
// returns a View for the specified part of the file.
//     void error(const char* printf_format, ...)
// prints an error message and does not return.  The subtype View must
// support a method
//     const unsigned char* data()
// which returns a pointer to a buffer containing the requested data.
// This general interface is used to read data from the file.  Objects
// of type View will never survive longer than the elfcpp function.

// Some of these functions must return a reference to part of the
// file.  To use these, the file interface must support a subtype
// Location:
//    Location(off_t file_offset, off_t data_size)
// To use this in conjunction with the accessors types Shdr, etc., the
// file interface should support an overload of view:
//    View view(Location)
// This permits writing
public:
	PBYTE data;
	off_t data_size;

	File() {};
	File(PBYTE a_data, off_t a_data_size) : data(a_data), data_size(a_data_size){};

	class View{		
	public:
		File *file;
		off_t file_offset;
		off_t data_size;
		const unsigned char* data(){
			return reinterpret_cast<const unsigned char*>(file->data)+file_offset;
		}
	};
	class Location{
	public:
		off_t file_offset;
		off_t data_size;

		Location(off_t file_offset, off_t data_size){
			this->file_offset=file_offset;
			this->data_size=data_size;
		}
	};

	View view(off_t file_offset, off_t data_size){
		View ret;
		ret.file_offset=file_offset;
		ret.file=this;
		ret.data_size=data_size;
		return ret;
	}
	View view(Location loc){
		View ret;
		ret.file_offset=loc.file_offset;
		ret.file=this;
		ret.data_size=loc.data_size;
		return ret;
	}
	void error(const char* printf_format, ...){
		va_list p;
		va_start(p,printf_format);
		vprintf(printf_format,p);
	}
};


class MappingFile:public File{	
	HANDLE hFileMapping;
public :
	HANDLE hFile;
	MappingFile(const char* fname){
		// Open the file that we want to map.
		hFile = ::CreateFile(fname,
			GENERIC_READ|GENERIC_EXECUTE ,
			0,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		data_size=GetFileSize(hFile,0);
		if(hFile==INVALID_HANDLE_VALUE || data_size<=0)throw "File Not Exist";

		// Create a file-mapping object for the file.
		hFileMapping = ::CreateFileMapping(hFile,
			NULL,
			PAGE_WRITECOPY,
			0, 0,
			NULL);

		data = (PBYTE)::MapViewOfFile(hFileMapping, FILE_MAP_COPY, 0, 0, 0);

	}
	void Close(){
		UnmapViewOfFile(data);
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
	}
};

struct MM{
	DWORD mmin,mmax;
	MM(){
		mmin=(DWORD)-1;
		mmax=0;
	}
	void Put(DWORD start,DWORD size){
		if(start==0 || size==0)return;

		if(mmin>start){
			mmin=start;
		}
		if(mmax<start+size){
			mmax=start+size;
		}
	}
	PBYTE ptr;
	void Alloc(){
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		DWORD rmmin=(DWORD)mmin/systemInfo.dwAllocationGranularity*systemInfo.dwAllocationGranularity;
		DWORD rmmax=((DWORD)mmax+systemInfo.dwAllocationGranularity-1)/systemInfo.dwAllocationGranularity*systemInfo.dwAllocationGranularity;

		off_t realsize=rmmax-rmmin;
		ptr=(PBYTE)VirtualAlloc((LPVOID)rmmin,realsize, MEM_COMMIT | MEM_RESERVE,PAGE_READWRITE);		
	}
	PBYTE Ptr(){
		return ptr;
	}
	BOOL Protect(LPBYTE ptr,DWORD size,DWORD pro){
		DWORD old;
		return VirtualProtect(ptr,size,pro,&old);
	}
};