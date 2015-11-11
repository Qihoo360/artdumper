// OATLoaderDumper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "StringPiece.h"
#include "oatparser.h"
#include <io.h>
#include <string>
#include <direct.h>

static void usage() {
	fprintf_s(stderr,
		"Usage: OATLoaderDumper [options] ...\n"
		"    Example: OATLoaderDumper --oat-file=F:\\androidL\\framework\\arm\\wm.odex --out-path=F:\\androidL\\outdex  \n"
		"\n");
	fprintf(stderr,
        "\n"
		"    --oat-file=<file.oat>: specifies an input oat filename.\n"
		"       Example: --oat-file=F:\\androidL\\framework\\arm\\wm.odex\n"
		"\n");
	fprintf(stderr,
		"    --out-path=<file.dex>: specifies an out dex path.\n"
		"       Example: --out-dex==F:\\androidL\\outdex \n"
		"\n");
}

static bool IsDirExist(const std::string& outpath) {
    bool ret = true;

    if (-1 == _access(outpath.c_str(), 0) && errno == ENOENT) {
        ret = false;
    }

    return ret;
}

static bool MakeDir(const std::string& outpath) {
    bool ret = false;
    if (0 == _mkdir(outpath.c_str())) {
        ret = true;
    }

    return ret;
}

int main(int argc, char* argv[])
{
	argv++;
	argc--;

	if (argc == 0) {
		fprintf(stderr, "No arguments specified.\n");
		usage();
		return false;
	}

	using namespace Art;
	std::string oat_file;
	std::string oat_todex_path;
	for (int i = 0; i < argc; i++) {
		const StringPiece option(argv[i]);
		if (option.starts_with("--oat-file=")) {
			oat_file = option.substr(strlen("--oat-file=")).data();
		}
		else if (option.starts_with("--out-path=")) {
			oat_todex_path = option.substr(strlen("--out-path=")).data();
		}
	}

	if (oat_file.length() == 0 || oat_todex_path.length() == 0) {
		fprintf(stderr, "--oat-file and --out-path must be specified\n");
		return false;
	}

    if (!IsDirExist(oat_todex_path)) {
        MakeDir(oat_todex_path);
    }

	if (InitOatParser(oat_file.c_str(), oat_todex_path.c_str())) {
		DoDumpToDex();
	}

	return 0;
}

