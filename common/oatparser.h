#pragma once

// 初始化， oat_file为原始的oat文件名 out_dex_path为输出的dex路径
extern "C" __declspec(dllimport) bool InitOatParser(const char* oat_file, const char* out_dex_path);

// 将Ota文件dump成dex文件
extern "C" __declspec(dllimport) bool DoDumpToDex();

