#pragma once

extern "C" __declspec(dllimport) bool ElfInit(char* elf, unsigned int len);

extern "C" __declspec(dllimport) bool GetOatInfo(unsigned int& offset, unsigned int& size);

extern "C" __declspec(dllimport) bool ElfUnInit();