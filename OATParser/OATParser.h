#pragma once

#include <string>
#include <memory>

namespace Art {
	class DexHeader;
	class OATHeader;

	class OATParser {
	public:
		OATParser() {};
		OATParser(const char* a_oat, unsigned int a_size);
		~OATParser();

		static OATParser& GetInstance();
		void init(const char* a_oat_file, const char* a_out_dex_path);

		const char* Begin();
		const char* End();

		bool Parser();
	private:
		bool OpenOat(std::unique_ptr<char[]>& a_buf, unsigned int& a_len);

		bool OatReSet(std::unique_ptr<char[]>& a_buf);

		bool Dump(std::string& a_dex_name, const DexHeader* a_dex_header);

		const OATHeader* GetOatHeader() {
			return m_oatheader.get();
		};

		void MakeDexName(std::string& a_dex_name, std::string& a_out_dex_name);
	private:
		static OATParser m_oat_parser;

		std::unique_ptr<OATHeader> m_oatheader;
		std::string m_oat_file;
		std::string m_out_dex_path;

		const char* m_oat_begin;
		const char* m_oat_end;
	};
}