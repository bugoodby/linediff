// linediff.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "main.h"
#include <string>
#include <vector>
#include <list>
#include <algorithm>

#pragma warning(disable:4786)

using namespace std;

int remove_newline_code( wchar_t *string );
int compare_lines( list<wstring> &left, list<wstring> &right, FILE *outfp, OPTION &opt );
int output_line( wstring &left, wstring &right, FILE *outfp, OPTION &opt );

int linediff( FILE *infp1, FILE *infp2, FILE *outfp, OPTION &opt )
{
	int retval = 0;
	wchar_t *line_buffer = NULL;
	list<wstring> leftLines, rightLines;

	// allocate memory
	line_buffer = (wchar_t*)malloc(sizeof(wchar_t) * opt.buffer_size);
	if ( !line_buffer ) {
		GET_ERROR_VAL(retval);
		goto EXIT;
	}
	
	// get line (left file)
	while ( fgetws(line_buffer, opt.buffer_size, infp1) != NULL ) {
		remove_newline_code(line_buffer);
		leftLines.push_back(line_buffer);
	}
	if ( opt.sort ) {
		//sort( leftLines.begin(), leftLines.end() );	// listはランダムアクセスイテレータでないのでstd::sort不可
		leftLines.sort();
	}

	// get line (right file)
	while ( fgetws(line_buffer, opt.buffer_size, infp2) != NULL ) {
		remove_newline_code(line_buffer);
		rightLines.push_back(line_buffer);
	}
	if ( opt.sort ) {
		//sort( rightLines.begin(), rightLines.end() );
		rightLines.sort();
	}
	
	// compare
	retval = compare_lines(leftLines, rightLines, outfp, opt);

EXIT:
	// free memory
	if ( line_buffer ) {
		free(line_buffer);
	}

	return retval;
}

int remove_newline_code( wchar_t *string )
{
	if (!string) return -1;
	
	size_t len = wcslen(string);
	wchar_t *ch = NULL;

	while ( len > 0 ) {
		ch = string + len - 1;
		if ( *ch == L'\n' || *ch == L'\r' ) {
			*ch = L'\0';
			len--;
		}
		else {
			break;
		}
	}
	return 0;
}

int compare_lines( list<wstring> &left, list<wstring> &right, FILE *outfp, OPTION &opt )
{
	wstring nulstr;
	list<wstring>::iterator itL, itR;

	itL = left.begin();
	itR = right.begin();

	// 左右どちらかが終端に辿り着くまでループ
	while ( itL != left.end() && itR != right.end() )
	{
		int nCompRet = itL->compare( *itR );
		
		// 一致
		if ( nCompRet == 0 ) 
		{
			output_line( *itL, *itR, outfp, opt );
			itL++;
			itR++;
		}
		// 左文字列が右にない
		else if ( nCompRet < 0 ) 
		{
			output_line( *itL, nulstr, outfp, opt );
			itL++;
		}
		// 右文字列が左にない
		else 
		{
			output_line( nulstr, *itR, outfp, opt );
			itR++;
		}
	}

	// 残った文字列を追加
	while ( itL != left.end() )
	{
		output_line( *itL, nulstr, outfp, opt );
		itL++;
	}
	while ( itR != right.end() )
	{
		output_line( nulstr, *itR, outfp, opt );
		itR++;
	}

	return 0;
}

int output_line( wstring &left, wstring &right, FILE *outfp, OPTION &opt )
{
	switch ( opt.out_type )
	{
	case TYPE_CSV:
		fwprintf(outfp, L"\"%s\",\"%s\"\n", left.c_str(), right.c_str());
		break;
	case TYPE_DEFAULT:
	default:
		fwprintf(outfp, L"%s | %s \n", left.c_str(), right.c_str());
		break;
	}

	return 0;
}