// getverinfo.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "main.h"

void usage(void);
bool parse_cmdline( int argc, wchar_t **argv, OPTION &opt );

//--------------------------------------------------------------
// main
//--------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	OPTION opt = {0};
	int retval = 0;
	FILE *console = stderr;
	FILE *infp1 = stdin;
	FILE *infp2 = stdin;
	FILE *outfp = stdout;

	setlocale(LC_CTYPE, "japanese");

	/* parse */
	if ( !parse_cmdline(argc, argv, opt) ) {
		goto EXIT;
	}

	if ( opt.num < 2 ) {
		fwprintf(console, L"too few arguments.\n");
	}
	else {
		wchar_t *infile1 = opt.argv[0];
		wchar_t *infile2 = opt.argv[1];

		/* open files */
		infp1 = _wfopen(infile1, L"r");
		if ( !infp1 ) {
			fwprintf(console, L"[ERROR] cannot open %s.\n", infile1);
			GET_ERROR_VAL(retval);
		}
		infp2 = _wfopen(infile2, L"r");
		if ( !infp2 ) {
			fwprintf(console, L"[ERROR] cannot open %s.\n", infile2);
			GET_ERROR_VAL(retval);
		}

		/* main */
		if ( infp1 && infp2 && outfp ) {
			retval = linediff( infp1, infp2, outfp, opt );
			if ( retval < 0 ) {
				fwprintf(console, L"[ERROR] fail in main routine.\n");
			}
		}
	}
	
	if ( opt.wait_key_input ) {
		fputws(L"--- press any key ---\n", outfp);
		getwc(stdin);
	}

EXIT:
	/* close files */
	if ( infp1 ) {
		fclose(infp1);
	}
	if ( infp2 ) {
		fclose(infp2);
	}
	return retval;
}

//--------------------------------------------------------------
// show usage
//--------------------------------------------------------------
void usage(void)
{
	fwprintf(stderr, L"[ %s Version %s ]\n", MODULENAME_STRING, VERSION_STRING);
	fwprintf(stderr, L"compare 2 files.\n");
	fwprintf(stderr, L"\n");
	fwprintf(stderr, L"Usage : %s [option...] <input file1> <input file2>\n", MODULENAME_STRING);
	fwprintf(stderr, L"        -b <integer>    : set line buffer size (default: 1024)\n");
	fwprintf(stderr, L"        -c              : do not wait a key input\n");
	fwprintf(stderr, L"        -o <type>       : specify output format\n");
	fwprintf(stderr, L"                          <type> = \"csv\" \n");
	fwprintf(stderr, L"        -s              : sort lines\n");
	fwprintf(stderr, L"        <infput file>   : input file path\n");
}

//--------------------------------------------------------------
// parse command line
//--------------------------------------------------------------
bool parse_cmdline( int argc, wchar_t **argv, OPTION &opt )
{
	bool ret = true;
	wchar_t *s = NULL;
	int iSize = 0;

	/* initialize */
	opt.buffer_size = 1024;
	opt.wait_key_input = true;
	opt.out_type = TYPE_DEFAULT;
	opt.sort = false;

	/* parse */
	while ( --argc > 0 ) {
		s = *++argv;
		if ( *s == L'-' || *s == L'/' ) {
			switch ( *++s )
			{
			case L'h':
			case L'?':
				usage();
				ret = false;
				break;
			case L'b':
				if ( --argc > 0 ) {
					iSize = _wtol(*++argv);
					if ( iSize > 0 ) opt.buffer_size = iSize;
				}
				break;
			case L'c':
				opt.wait_key_input = false;
				break;
			case L'o':
				if ( --argc > 0 ) {
					++argv;
					if ( wcscmp(*argv, L"csv") == 0 ) {
						opt.out_type = TYPE_CSV;
					}
				}
				break;
			case L's':
				opt.sort = true;
				break;
			default:
				fwprintf(stderr, L"Unknown parameter : -%s\n", s);
				ret = false;
				break;
			}
		}
		else {
			opt.num = argc;
			opt.argv = argv;
			break;
		}
	}

	return ret;
}


