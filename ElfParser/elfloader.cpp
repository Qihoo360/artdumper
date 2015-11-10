// elfloader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iomanip>  
#include "elfcpp.h"
#include "elfcpp_file.h"
#include "map_file.h"
#include "util.h"
#include "exec_buffer.h"
#include <assert.h>
#include <map>
#include <iostream>

using namespace std;

typedef elfcpp::Elf_file<32,false,File> ElfFile;
static File::Location s_oat(0, 0);

extern "C" __declspec(dllexport) bool ElfInit(char* elf, unsigned int len) {
	bool ret = false;

	File file(reinterpret_cast<PBYTE>(elf), len);
	ElfFile::Ef_ehdr hdr(file.view(elfcpp::file_header_offset, ElfFile::ehdr_size).data());
	ElfFile efile(&file, hdr);

	//reset s_oat
	s_oat.file_offset = 0;
	s_oat.data_size = 0;

	//找到rodata段，将offset和size找到，offset将是ota文件的头
	for (unsigned i = 0; i < efile.shnum(); ++i) {
		if (efile.section_name(i).compare(".rodata") == 0) {
			s_oat.file_offset = efile.section_contents(i).file_offset;
			s_oat.data_size = efile.section_contents(i).data_size;

			ret = true;
			break;
		}
	}

	return ret;
}

extern "C" __declspec(dllexport) bool GetOatInfo(unsigned int& offset, unsigned int& size) {
	if (s_oat.file_offset == 0 || s_oat.data_size == 0) {
		return false;
	}

	offset = s_oat.file_offset;
	size = s_oat.data_size;

	return true;
}

extern "C" __declspec(dllexport) bool ElfUnInit() {
	//reset s_oat
	s_oat.file_offset = 0;
	s_oat.data_size = 0;

	return true;
}