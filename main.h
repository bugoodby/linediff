#ifndef __MAIN_H__
#define __MAIN_H__

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

#define GET_ERROR_VAL( val )	((val) = -1 * __LINE__)


/*----------------------------------------*/
/* �v���O�������A�o�[�W����               */
/*----------------------------------------*/
#define MODULENAME_STRING	L"linediff"
#define VERSION_STRING		L"0.02"


/*----------------------------------------*/
/* �R�}���h���C����͌��ʂ��i�[����\���� */
/*----------------------------------------*/
typedef enum {
	TYPE_DEFAULT = 0,
	TYPE_CSV
} OUTPUT_TYPE;


typedef struct tagOPTION {
	int			num;					/* number of input files */
	_TCHAR**	argv;					/* pointer to a file name array */
	int			buffer_size;			/* read buffer size */
	bool		wait_key_input;			/* if true, wait key input */
	bool		sort;					/* if true, sort lines */
	OUTPUT_TYPE out_type;				/* output format type */
} OPTION;

/*----------------------------------------*/
/* �֐��̃v���g�^�C�v�錾                 */
/*----------------------------------------*/
int linediff( FILE *infp1, FILE *infp2, FILE *outfp, OPTION &opt );

#endif /* __MAIN_H__ */
