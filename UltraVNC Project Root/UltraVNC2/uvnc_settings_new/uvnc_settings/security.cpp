//  Copyright (C) 2010 Ultr@VNC Team Members. All Rights Reserved.
// 
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
//  USA.
//
// If the source code for the program is not available from the place from
// which you received this file, check 
// http://www.uvnc.com/
//
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include <comutil.h>
#include "vncsetauth.h"
bool initdone2=false;

extern LONG MSLogonRequired;
extern LONG NewMSLogon;
extern LONG AuthRequired;
extern LONG AllowShutdown;
extern LONG AllowProperties;
extern LONG AllowEditClients;
extern HINSTANCE hInst;
#define MAXPWLEN 8
extern char passwd[9];
extern char passwd2[9];
char *plaintext;
char *plaintext3;
vncSetAuth m_vncauth;
extern HWND m_hParent;


unsigned char fixedkey[8] = {23,82,107,6,35,78,88,7};
static unsigned long KnL[32] = { 0L };
static unsigned short bytebit[8]	= {
	01, 02, 04, 010, 020, 040, 0100, 0200 };
static unsigned long bigbyte[24] = {
	0x800000L,	0x400000L,	0x200000L,	0x100000L,
	0x80000L,	0x40000L,	0x20000L,	0x10000L,
	0x8000L,	0x4000L,	0x2000L,	0x1000L,
	0x800L, 	0x400L, 	0x200L, 	0x100L,
	0x80L,		0x40L,		0x20L,		0x10L,
	0x8L,		0x4L,		0x2L,		0x1L	};
static unsigned char pc1[56] = {
	56, 48, 40, 32, 24, 16,  8,	 0, 57, 49, 41, 33, 25, 17,
	 9,  1, 58, 50, 42, 34, 26,	18, 10,  2, 59, 51, 43, 35,
	62, 54, 46, 38, 30, 22, 14,	 6, 61, 53, 45, 37, 29, 21,
	13,  5, 60, 52, 44, 36, 28,	20, 12,  4, 27, 19, 11,  3 };
static unsigned char totrot[16] = {
	1,2,4,6,8,10,12,14,15,17,19,21,23,25,27,28 };
static unsigned char pc2[48] = {
	13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9,
	22, 18, 11,  3, 25,  7, 15,  6, 26, 19, 12,  1,
	40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
	43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31 };

#define DE1     1
#define EN0     0 

static unsigned long SP1[64] = {
	0x01010400L, 0x00000000L, 0x00010000L, 0x01010404L,
	0x01010004L, 0x00010404L, 0x00000004L, 0x00010000L,
	0x00000400L, 0x01010400L, 0x01010404L, 0x00000400L,
	0x01000404L, 0x01010004L, 0x01000000L, 0x00000004L,
	0x00000404L, 0x01000400L, 0x01000400L, 0x00010400L,
	0x00010400L, 0x01010000L, 0x01010000L, 0x01000404L,
	0x00010004L, 0x01000004L, 0x01000004L, 0x00010004L,
	0x00000000L, 0x00000404L, 0x00010404L, 0x01000000L,
	0x00010000L, 0x01010404L, 0x00000004L, 0x01010000L,
	0x01010400L, 0x01000000L, 0x01000000L, 0x00000400L,
	0x01010004L, 0x00010000L, 0x00010400L, 0x01000004L,
	0x00000400L, 0x00000004L, 0x01000404L, 0x00010404L,
	0x01010404L, 0x00010004L, 0x01010000L, 0x01000404L,
	0x01000004L, 0x00000404L, 0x00010404L, 0x01010400L,
	0x00000404L, 0x01000400L, 0x01000400L, 0x00000000L,
	0x00010004L, 0x00010400L, 0x00000000L, 0x01010004L };

static unsigned long SP2[64] = {
	0x80108020L, 0x80008000L, 0x00008000L, 0x00108020L,
	0x00100000L, 0x00000020L, 0x80100020L, 0x80008020L,
	0x80000020L, 0x80108020L, 0x80108000L, 0x80000000L,
	0x80008000L, 0x00100000L, 0x00000020L, 0x80100020L,
	0x00108000L, 0x00100020L, 0x80008020L, 0x00000000L,
	0x80000000L, 0x00008000L, 0x00108020L, 0x80100000L,
	0x00100020L, 0x80000020L, 0x00000000L, 0x00108000L,
	0x00008020L, 0x80108000L, 0x80100000L, 0x00008020L,
	0x00000000L, 0x00108020L, 0x80100020L, 0x00100000L,
	0x80008020L, 0x80100000L, 0x80108000L, 0x00008000L,
	0x80100000L, 0x80008000L, 0x00000020L, 0x80108020L,
	0x00108020L, 0x00000020L, 0x00008000L, 0x80000000L,
	0x00008020L, 0x80108000L, 0x00100000L, 0x80000020L,
	0x00100020L, 0x80008020L, 0x80000020L, 0x00100020L,
	0x00108000L, 0x00000000L, 0x80008000L, 0x00008020L,
	0x80000000L, 0x80100020L, 0x80108020L, 0x00108000L };

static unsigned long SP3[64] = {
	0x00000208L, 0x08020200L, 0x00000000L, 0x08020008L,
	0x08000200L, 0x00000000L, 0x00020208L, 0x08000200L,
	0x00020008L, 0x08000008L, 0x08000008L, 0x00020000L,
	0x08020208L, 0x00020008L, 0x08020000L, 0x00000208L,
	0x08000000L, 0x00000008L, 0x08020200L, 0x00000200L,
	0x00020200L, 0x08020000L, 0x08020008L, 0x00020208L,
	0x08000208L, 0x00020200L, 0x00020000L, 0x08000208L,
	0x00000008L, 0x08020208L, 0x00000200L, 0x08000000L,
	0x08020200L, 0x08000000L, 0x00020008L, 0x00000208L,
	0x00020000L, 0x08020200L, 0x08000200L, 0x00000000L,
	0x00000200L, 0x00020008L, 0x08020208L, 0x08000200L,
	0x08000008L, 0x00000200L, 0x00000000L, 0x08020008L,
	0x08000208L, 0x00020000L, 0x08000000L, 0x08020208L,
	0x00000008L, 0x00020208L, 0x00020200L, 0x08000008L,
	0x08020000L, 0x08000208L, 0x00000208L, 0x08020000L,
	0x00020208L, 0x00000008L, 0x08020008L, 0x00020200L };

static unsigned long SP4[64] = {
	0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
	0x00802080L, 0x00800081L, 0x00800001L, 0x00002001L,
	0x00000000L, 0x00802000L, 0x00802000L, 0x00802081L,
	0x00000081L, 0x00000000L, 0x00800080L, 0x00800001L,
	0x00000001L, 0x00002000L, 0x00800000L, 0x00802001L,
	0x00000080L, 0x00800000L, 0x00002001L, 0x00002080L,
	0x00800081L, 0x00000001L, 0x00002080L, 0x00800080L,
	0x00002000L, 0x00802080L, 0x00802081L, 0x00000081L,
	0x00800080L, 0x00800001L, 0x00802000L, 0x00802081L,
	0x00000081L, 0x00000000L, 0x00000000L, 0x00802000L,
	0x00002080L, 0x00800080L, 0x00800081L, 0x00000001L,
	0x00802001L, 0x00002081L, 0x00002081L, 0x00000080L,
	0x00802081L, 0x00000081L, 0x00000001L, 0x00002000L,
	0x00800001L, 0x00002001L, 0x00802080L, 0x00800081L,
	0x00002001L, 0x00002080L, 0x00800000L, 0x00802001L,
	0x00000080L, 0x00800000L, 0x00002000L, 0x00802080L };

static unsigned long SP5[64] = {
	0x00000100L, 0x02080100L, 0x02080000L, 0x42000100L,
	0x00080000L, 0x00000100L, 0x40000000L, 0x02080000L,
	0x40080100L, 0x00080000L, 0x02000100L, 0x40080100L,
	0x42000100L, 0x42080000L, 0x00080100L, 0x40000000L,
	0x02000000L, 0x40080000L, 0x40080000L, 0x00000000L,
	0x40000100L, 0x42080100L, 0x42080100L, 0x02000100L,
	0x42080000L, 0x40000100L, 0x00000000L, 0x42000000L,
	0x02080100L, 0x02000000L, 0x42000000L, 0x00080100L,
	0x00080000L, 0x42000100L, 0x00000100L, 0x02000000L,
	0x40000000L, 0x02080000L, 0x42000100L, 0x40080100L,
	0x02000100L, 0x40000000L, 0x42080000L, 0x02080100L,
	0x40080100L, 0x00000100L, 0x02000000L, 0x42080000L,
	0x42080100L, 0x00080100L, 0x42000000L, 0x42080100L,
	0x02080000L, 0x00000000L, 0x40080000L, 0x42000000L,
	0x00080100L, 0x02000100L, 0x40000100L, 0x00080000L,
	0x00000000L, 0x40080000L, 0x02080100L, 0x40000100L };

static unsigned long SP6[64] = {
	0x20000010L, 0x20400000L, 0x00004000L, 0x20404010L,
	0x20400000L, 0x00000010L, 0x20404010L, 0x00400000L,
	0x20004000L, 0x00404010L, 0x00400000L, 0x20000010L,
	0x00400010L, 0x20004000L, 0x20000000L, 0x00004010L,
	0x00000000L, 0x00400010L, 0x20004010L, 0x00004000L,
	0x00404000L, 0x20004010L, 0x00000010L, 0x20400010L,
	0x20400010L, 0x00000000L, 0x00404010L, 0x20404000L,
	0x00004010L, 0x00404000L, 0x20404000L, 0x20000000L,
	0x20004000L, 0x00000010L, 0x20400010L, 0x00404000L,
	0x20404010L, 0x00400000L, 0x00004010L, 0x20000010L,
	0x00400000L, 0x20004000L, 0x20000000L, 0x00004010L,
	0x20000010L, 0x20404010L, 0x00404000L, 0x20400000L,
	0x00404010L, 0x20404000L, 0x00000000L, 0x20400010L,
	0x00000010L, 0x00004000L, 0x20400000L, 0x00404010L,
	0x00004000L, 0x00400010L, 0x20004010L, 0x00000000L,
	0x20404000L, 0x20000000L, 0x00400010L, 0x20004010L };

static unsigned long SP7[64] = {
	0x00200000L, 0x04200002L, 0x04000802L, 0x00000000L,
	0x00000800L, 0x04000802L, 0x00200802L, 0x04200800L,
	0x04200802L, 0x00200000L, 0x00000000L, 0x04000002L,
	0x00000002L, 0x04000000L, 0x04200002L, 0x00000802L,
	0x04000800L, 0x00200802L, 0x00200002L, 0x04000800L,
	0x04000002L, 0x04200000L, 0x04200800L, 0x00200002L,
	0x04200000L, 0x00000800L, 0x00000802L, 0x04200802L,
	0x00200800L, 0x00000002L, 0x04000000L, 0x00200800L,
	0x04000000L, 0x00200800L, 0x00200000L, 0x04000802L,
	0x04000802L, 0x04200002L, 0x04200002L, 0x00000002L,
	0x00200002L, 0x04000000L, 0x04000800L, 0x00200000L,
	0x04200800L, 0x00000802L, 0x00200802L, 0x04200800L,
	0x00000802L, 0x04000002L, 0x04200802L, 0x04200000L,
	0x00200800L, 0x00000000L, 0x00000002L, 0x04200802L,
	0x00000000L, 0x00200802L, 0x04200000L, 0x00000800L,
	0x04000002L, 0x04000800L, 0x00000800L, 0x00200002L };

static unsigned long SP8[64] = {
	0x10001040L, 0x00001000L, 0x00040000L, 0x10041040L,
	0x10000000L, 0x10001040L, 0x00000040L, 0x10000000L,
	0x00040040L, 0x10040000L, 0x10041040L, 0x00041000L,
	0x10041000L, 0x00041040L, 0x00001000L, 0x00000040L,
	0x10040000L, 0x10000040L, 0x10001000L, 0x00001040L,
	0x00041000L, 0x00040040L, 0x10040040L, 0x10041000L,
	0x00001040L, 0x00000000L, 0x00000000L, 0x10040040L,
	0x10000040L, 0x10001000L, 0x00041040L, 0x00040000L,
	0x00041040L, 0x00040000L, 0x10041000L, 0x00001000L,
	0x00000040L, 0x10040040L, 0x00001000L, 0x00041040L,
	0x10001000L, 0x00000040L, 0x10000040L, 0x10040000L,
	0x10040040L, 0x10000000L, 0x00040000L, 0x10001040L,
	0x00000000L, 0x10041040L, 0x00040040L, 0x10000040L,
	0x10040000L, 0x10001000L, 0x10001040L, 0x00000000L,
	0x10041040L, 0x00041000L, 0x00041000L, 0x00001040L,
	0x00001040L, 0x00040040L, 0x10000000L, 0x10041000L };

void usekey(register unsigned long *from)
{
	register unsigned long *to, *endp;

	to = KnL, endp = &KnL[32];
	while( to < endp ) *to++ = *from++;
	return;
}

static void cookey(register unsigned long *raw1)
{
	register unsigned long *cook, *raw0;
	unsigned long dough[32];
	register int i;

	cook = dough;
	for( i = 0; i < 16; i++, raw1++ ) {
		raw0 = raw1++;
		*cook	 = (*raw0 & 0x00fc0000L) << 6;
		*cook	|= (*raw0 & 0x00000fc0L) << 10;
		*cook	|= (*raw1 & 0x00fc0000L) >> 10;
		*cook++ |= (*raw1 & 0x00000fc0L) >> 6;
		*cook	 = (*raw0 & 0x0003f000L) << 12;
		*cook	|= (*raw0 & 0x0000003fL) << 16;
		*cook	|= (*raw1 & 0x0003f000L) >> 4;
		*cook++ |= (*raw1 & 0x0000003fL);
		}
	usekey(dough);
	return;
}

void deskey(unsigned char *key, int edf)	/* Thanks to James Gillogly & Phil Karn! */
{
	register int i, j, l, m, n;
	unsigned char pc1m[56], pcr[56];
	unsigned long kn[32];

	for ( j = 0; j < 56; j++ ) {
		l = pc1[j];
		m = l & 07;
		pc1m[j] = (key[l >> 3] & bytebit[m]) ? 1 : 0;
		}
	for( i = 0; i < 16; i++ ) {
		if( edf == DE1 ) m = (15 - i) << 1;
		else m = i << 1;
		n = m + 1;
		kn[m] = kn[n] = 0L;
		for( j = 0; j < 28; j++ ) {
			l = j + totrot[i];
			if( l < 28 ) pcr[j] = pc1m[l];
			else pcr[j] = pc1m[l - 28];
			}
		for( j = 28; j < 56; j++ ) {
		    l = j + totrot[i];
		    if( l < 56 ) pcr[j] = pc1m[l];
		    else pcr[j] = pc1m[l - 28];
		    }
		for( j = 0; j < 24; j++ ) {
			if( pcr[pc2[j]] ) kn[m] |= bigbyte[j];
			if( pcr[pc2[j+24]] ) kn[n] |= bigbyte[j];
			}
		}
	cookey(kn);
	return;
	}
static void scrunch(register unsigned char *outof, register unsigned long *into)
{
	*into	 = (*outof++ & 0xffL) << 24;
	*into	|= (*outof++ & 0xffL) << 16;
	*into	|= (*outof++ & 0xffL) << 8;
	*into++ |= (*outof++ & 0xffL);
	*into	 = (*outof++ & 0xffL) << 24;
	*into	|= (*outof++ & 0xffL) << 16;
	*into	|= (*outof++ & 0xffL) << 8;
	*into	|= (*outof   & 0xffL);
	return;
	}

static void unscrun(register unsigned long *outof, register unsigned char *into)
{
	*into++ = (unsigned char)((*outof >> 24) & 0xffL);
	*into++ = (unsigned char)((*outof >> 16) & 0xffL);
	*into++ = (unsigned char)((*outof >>  8) & 0xffL);
	*into++ = (unsigned char)( *outof++	 & 0xffL);
	*into++ = (unsigned char)((*outof >> 24) & 0xffL);
	*into++ = (unsigned char)((*outof >> 16) & 0xffL);
	*into++ = (unsigned char)((*outof >>  8) & 0xffL);
	*into	=  (unsigned char)(*outof	 & 0xffL);
	return;
	}

static void desfunc(register unsigned long *block, register unsigned long *keys)
{
	register unsigned long fval, work, right, leftt;
	register int round;

	leftt = block[0];
	right = block[1];
	work = ((leftt >> 4) ^ right) & 0x0f0f0f0fL;
	right ^= work;
	leftt ^= (work << 4);
	work = ((leftt >> 16) ^ right) & 0x0000ffffL;
	right ^= work;
	leftt ^= (work << 16);
	work = ((right >> 2) ^ leftt) & 0x33333333L;
	leftt ^= work;
	right ^= (work << 2);
	work = ((right >> 8) ^ leftt) & 0x00ff00ffL;
	leftt ^= work;
	right ^= (work << 8);
	right = ((right << 1) | ((right >> 31) & 1L)) & 0xffffffffL;
	work = (leftt ^ right) & 0xaaaaaaaaL;
	leftt ^= work;
	right ^= work;
	leftt = ((leftt << 1) | ((leftt >> 31) & 1L)) & 0xffffffffL;

	for( round = 0; round < 8; round++ ) {
		work  = (right << 28) | (right >> 4);
		work ^= *keys++;
		fval  = SP7[ work		 & 0x3fL];
		fval |= SP5[(work >>  8) & 0x3fL];
		fval |= SP3[(work >> 16) & 0x3fL];
		fval |= SP1[(work >> 24) & 0x3fL];
		work  = right ^ *keys++;
		fval |= SP8[ work		 & 0x3fL];
		fval |= SP6[(work >>  8) & 0x3fL];
		fval |= SP4[(work >> 16) & 0x3fL];
		fval |= SP2[(work >> 24) & 0x3fL];
		leftt ^= fval;
		work  = (leftt << 28) | (leftt >> 4);
		work ^= *keys++;
		fval  = SP7[ work		 & 0x3fL];
		fval |= SP5[(work >>  8) & 0x3fL];
		fval |= SP3[(work >> 16) & 0x3fL];
		fval |= SP1[(work >> 24) & 0x3fL];
		work  = leftt ^ *keys++;
		fval |= SP8[ work		 & 0x3fL];
		fval |= SP6[(work >>  8) & 0x3fL];
		fval |= SP4[(work >> 16) & 0x3fL];
		fval |= SP2[(work >> 24) & 0x3fL];
		right ^= fval;
		}

	right = (right << 31) | (right >> 1);
	work = (leftt ^ right) & 0xaaaaaaaaL;
	leftt ^= work;
	right ^= work;
	leftt = (leftt << 31) | (leftt >> 1);
	work = ((leftt >> 8) ^ right) & 0x00ff00ffL;
	right ^= work;
	leftt ^= (work << 8);
	work = ((leftt >> 2) ^ right) & 0x33333333L;
	right ^= work;
	leftt ^= (work << 2);
	work = ((right >> 16) ^ leftt) & 0x0000ffffL;
	leftt ^= work;
	right ^= (work << 16);
	work = ((right >> 4) ^ leftt) & 0x0f0f0f0fL;
	leftt ^= work;
	right ^= (work << 4);
	*block++ = right;
	*block = leftt;
	return;
}

void des(unsigned char *inblock, unsigned char *outblock)
{
	unsigned long work[2];

	scrunch(inblock, work);
	desfunc(work, KnL);
	unscrun(work, outblock);
	return;
}
int
vncEncryptPasswd(char *passwd, char *encryptedPasswd)
{
    size_t i;

    /* pad password with nulls */

    for (i = 0; i < MAXPWLEN; i++) {
	if (i < strlen(passwd)) {
	    encryptedPasswd[i] = passwd[i];
	} else {
	    encryptedPasswd[i] = 0;
	}
    }

    /* Do encryption in-place - this way we overwrite our copy of the plaintext
       password */

    deskey(fixedkey, EN0);
    des((unsigned char *)encryptedPasswd, (unsigned char *)encryptedPasswd);

    return 8;
}

char *
vncDecryptPasswd(char *inouttext)
{
    unsigned char *passwd = (unsigned char *)malloc(9);

    deskey(fixedkey, DE1);
    des((unsigned char *)inouttext, passwd);

    passwd[8] = 0;

    return (char *)passwd;
}

bool error_passwd1=false;
bool error_passwd2=false;
bool error_auth=true;


BOOL CALLBACK security(HWND hwnd, UINT uMsg,WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (uMsg)
	{
	case WM_CTLCOLORSTATIC:
		{
			if((HWND) lParam == GetDlgItem(hwnd, IDC_PASSWORD_LABEL1) && error_passwd1 )
			{
			SetBkColor((HDC) wParam, (COLORREF) GetSysColor(COLOR_BTNFACE));
			SetTextColor((HDC) wParam, RGB(255, 0, 0));
			SetBkMode((HDC) wParam, TRANSPARENT);
			return true;
			}
			if((HWND) lParam == GetDlgItem(hwnd, IDC_PASSWORD_LABEL2) && error_passwd2 )
			{
			SetBkColor((HDC) wParam, (COLORREF) GetSysColor(COLOR_BTNFACE));
			SetTextColor((HDC) wParam, RGB(255, 0, 0));
			SetBkMode((HDC) wParam, TRANSPARENT);
			return true;
			}
			if((HWND) lParam == GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX))
			{
			SetBkColor((HDC) wParam, (COLORREF) GetSysColor(COLOR_MENU));
			SetTextColor((HDC) wParam, RGB(255, 0, 0));
			SetBkMode((HDC) wParam, TRANSPARENT);
			return false;
			}

			return false;
		}
		break;

	case WM_INITDIALOG: 
		{	
			initdone2=false;
			SendMessage(GetDlgItem(hwnd, IDC_MSLOGON_CHECKD), BM_SETCHECK, MSLogonRequired, 0);
			SendMessage(GetDlgItem(hwnd, IDC_NEW_MSLOGON), BM_SETCHECK, NewMSLogon, 0);
			SendMessage(GetDlgItem(hwnd, IDC_PASSRECK), BM_SETCHECK, !AuthRequired, 0);
			SendMessage(GetDlgItem(hwnd, IDC_ALLOWEDIT), BM_SETCHECK, AllowEditClients, 0);
			SetDlgItemText(hwnd, IDC_PASSWORD, "~~~~~~~~");
			SetDlgItemText(hwnd, IDC_PASSWORD2, "~~~~~~~~");
			BOOL bMSLogonChecked =
				(SendDlgItemMessage(hwnd, IDC_MSLOGON_CHECKD,
										BM_GETCHECK, 0, 0) == BST_CHECKED);

			EnableWindow(GetDlgItem(hwnd, IDC_NEW_MSLOGON), bMSLogonChecked);
			EnableWindow(GetDlgItem(hwnd, IDC_MSLOGON), bMSLogonChecked);

			if (!AuthRequired)
			{
			EnableWindow(GetDlgItem(hwnd, IDC_MSLOGON_CHECKD), AuthRequired);
			EnableWindow(GetDlgItem(hwnd, IDC_PASSWORD), AuthRequired);
			EnableWindow(GetDlgItem(hwnd, IDC_PASSWORD2), AuthRequired);
			EnableWindow(GetDlgItem(hwnd, IDC_NEW_MSLOGON), AuthRequired);
			EnableWindow(GetDlgItem(hwnd, IDC_MSLOGON), AuthRequired);
			}

			error_passwd1=false;error_passwd2=false;
			int len = strlen(passwd);
			if (strcmp(passwd,passwd2) == 0 && len!=0)
			{
				error_passwd1=true;error_passwd2=true;
			}
			if (AuthRequired==1 && strlen(passwd)==0 && strlen(passwd2)==0)
			{
				error_passwd1=true;error_passwd2=true;
			}
			else if (AuthRequired==1 && strlen(passwd)==0 )
			{
				error_passwd1=true;
			}
			else if (AuthRequired==1 && strlen(passwd2)==0)
			{
				error_passwd2=true;
			}
			if (strcmp(passwd,passwd2) == 0 && len!=0)
				{
					SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"The <view only> and the <full> password are identical. \n You always have <full> access.");
				}
				else if (AuthRequired==1 && strlen(passwd)==0 && strlen(passwd2)==0)
				{
					SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"Authentication error: You enabled authentication but no password have been set for <full> and <view only>");
				}
				else if (AuthRequired==1 && strlen(passwd)==0 )
				{
					SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"Authentication error: You enabled authentication but no password have been set for <full>");
				}
				else if (AuthRequired==1 && strlen(passwd2)==0)
				{
					SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"Authentication error: You enabled authentication but no password have been set for <view only>");
				}
				else SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"");

			initdone2=true;
			InvalidateRect(hwnd,NULL,false);
			return TRUE;
		}

	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{
			case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			return TRUE;

			case IDC_MSLOGON_CHECKD:
			{
				BOOL bMSLogonChecked =
				(SendDlgItemMessage(hwnd, IDC_MSLOGON_CHECKD,
										BM_GETCHECK, 0, 0) == BST_CHECKED);

				EnableWindow(GetDlgItem(hwnd, IDC_NEW_MSLOGON), bMSLogonChecked);
				EnableWindow(GetDlgItem(hwnd, IDC_MSLOGON), bMSLogonChecked);

			}
			return TRUE;

			case IDC_PASSRECK:
				{
				AuthRequired=!SendDlgItemMessage(hwnd, IDC_PASSRECK, BM_GETCHECK, 0, 0);
				if (!AuthRequired)
				{
				EnableWindow(GetDlgItem(hwnd, IDC_MSLOGON_CHECKD), AuthRequired);
				EnableWindow(GetDlgItem(hwnd, IDC_PASSWORD), AuthRequired);
				EnableWindow(GetDlgItem(hwnd, IDC_PASSWORD2), AuthRequired);
				EnableWindow(GetDlgItem(hwnd, IDC_NEW_MSLOGON), AuthRequired);
				EnableWindow(GetDlgItem(hwnd, IDC_MSLOGON), AuthRequired);
				}
				else
				{
					EnableWindow(GetDlgItem(hwnd, IDC_MSLOGON_CHECKD), AuthRequired);
					EnableWindow(GetDlgItem(hwnd, IDC_PASSWORD), AuthRequired);
					EnableWindow(GetDlgItem(hwnd, IDC_PASSWORD2), AuthRequired);
					EnableWindow(GetDlgItem(hwnd, IDC_NEW_MSLOGON), AuthRequired);
					EnableWindow(GetDlgItem(hwnd, IDC_MSLOGON), AuthRequired);
					BOOL bMSLogonChecked =
						(SendDlgItemMessage(hwnd, IDC_MSLOGON_CHECKD,
										BM_GETCHECK, 0, 0) == BST_CHECKED);

					EnableWindow(GetDlgItem(hwnd, IDC_NEW_MSLOGON), bMSLogonChecked);
					EnableWindow(GetDlgItem(hwnd, IDC_MSLOGON), bMSLogonChecked);
				}

				char plaintext2[MAXPWLEN+1];
				int len=strlen(passwd);
				GetDlgItemText(hwnd, IDC_PASSWORD, (LPSTR) &plaintext2, MAXPWLEN+1);
				if (strcmp(plaintext2, "~~~~~~~~") != 0) 
				{
				len = GetDlgItemText(hwnd, IDC_PASSWORD, (LPSTR) &plaintext2, MAXPWLEN+1);
				if (len == 0)
						{
							memset(passwd,0,MAXPWLEN);
						}
				else
						{
							vncEncryptPasswd(plaintext2,passwd);
						}
				}
				char plaintext4[MAXPWLEN+1];
				int len2=strlen(passwd2);
				GetDlgItemText(hwnd, IDC_PASSWORD2, (LPSTR) &plaintext4, MAXPWLEN+1);
				if (strcmp(plaintext4, "~~~~~~~~") != 0) 
				{
				len2 = GetDlgItemText(hwnd, IDC_PASSWORD2, (LPSTR) &plaintext4, MAXPWLEN+1);
				if (len2 == 0)
						{
							memset(passwd2,0,MAXPWLEN);
						}
				else
						{
							vncEncryptPasswd(plaintext4,passwd2);
						}
				}

				error_passwd1=false;error_passwd2=false;
				if (strcmp(passwd,passwd2) == 0 && len!=0)
				{
					error_passwd1=true;error_passwd2=true;
				}
				if (AuthRequired==1 && strlen(passwd)==0 && strlen(passwd2)==0)
				{
					error_passwd1=true;error_passwd2=true;
				}
				else if (AuthRequired==1 && strlen(passwd)==0 )
				{
					error_passwd1=true;
				}
				else if (AuthRequired==1 && strlen(passwd2)==0)
				{
					error_passwd2=true;
				}

				if (strcmp(passwd,passwd2) == 0 && len!=0)
				{
					SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"The <view only> and the <full> password are identical. \n You always have <full> access.");
				}
				else if (AuthRequired==1 && strlen(passwd)==0 && strlen(passwd2)==0)
				{
					SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"Authentication error: You enabled authentication but no password have been set for <full> and <view only>");
				}
				else if (AuthRequired==1 && strlen(passwd)==0 )
				{
					SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"Authentication error: You enabled authentication but no password have been set for <full>");
				}
				else if (AuthRequired==1 && strlen(passwd2)==0)
				{
					SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"Authentication error: You enabled authentication but no password have been set for <view only>");
				}
				else SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"");

				InvalidateRect(hwnd,NULL,false);
				SendMessage(m_hParent,WM_TIMER,0,0);
				}
				break;
			case IDC_MSLOGON:
			{
				// Marscha@2004 - authSSP: if "New MS-Logon" is checked,
				// call vncEditSecurity from SecurityEditor.dll,
				// else call "old" dialog.
				BOOL bNewMSLogonChecked =
				(SendDlgItemMessage(hwnd, IDC_NEW_MSLOGON,
										BM_GETCHECK, 0, 0) == BST_CHECKED);
				if (bNewMSLogonChecked) 
					{
						typedef void (*vncEditSecurityFn) (HWND hwnd, HINSTANCE hInstance);
						vncEditSecurityFn vncEditSecurity = 0;
						char szCurrentDir[MAX_PATH];
							if (GetModuleFileName(NULL, szCurrentDir, MAX_PATH)) {
								char* p = strrchr(szCurrentDir, '\\');
								*p = '\0';
								strcat (szCurrentDir,"\\authSSP.dll");
							}
						HMODULE hModule = LoadLibrary(szCurrentDir);
						if (hModule) {
							vncEditSecurity = (vncEditSecurityFn) GetProcAddress(hModule, "vncEditSecurity");
							HRESULT hr = CoInitialize(NULL);
							vncEditSecurity(NULL, hInst);
							CoUninitialize();
							FreeLibrary(hModule);
						}
					}
				 else { 
					// Marscha@2004 - authSSP: end of change
					m_vncauth.Show(TRUE);
				}
			}
			return TRUE;

		case IDOK:
			MSLogonRequired=SendDlgItemMessage(hwnd, IDC_MSLOGON_CHECKD, BM_GETCHECK, 0, 0);
			NewMSLogon=SendDlgItemMessage(hwnd, IDC_NEW_MSLOGON, BM_GETCHECK, 0, 0);
			AuthRequired=!SendDlgItemMessage(hwnd, IDC_PASSRECK, BM_GETCHECK, 0, 0);
			//AllowShutdown=SendDlgItemMessage(hwnd, IDC_ALLOWSHUTDOWN, BM_GETCHECK, 0, 0);
			AllowEditClients=SendDlgItemMessage(hwnd, IDC_ALLOWEDIT, BM_GETCHECK, 0, 0);
			//AllowProperties=SendDlgItemMessage(hwnd, IDC_ALLOWPROP, BM_GETCHECK, 0, 0);

			char plaintext2[MAXPWLEN+1];
			int len=strlen(passwd);
			GetDlgItemText(hwnd, IDC_PASSWORD, (LPSTR) &plaintext2, MAXPWLEN+1);
			if (strcmp(plaintext2, "~~~~~~~~") != 0) 
			{
			len = GetDlgItemText(hwnd, IDC_PASSWORD, (LPSTR) &plaintext2, MAXPWLEN+1);
			if (len == 0)
					{
						memset(passwd,0,MAXPWLEN);
					}
			else
					{
						vncEncryptPasswd(plaintext2,passwd);
					}
			}
			char plaintext4[MAXPWLEN+1];
			int len2=strlen(passwd2);
			GetDlgItemText(hwnd, IDC_PASSWORD2, (LPSTR) &plaintext4, MAXPWLEN+1);
			if (strcmp(plaintext4, "~~~~~~~~") != 0) 
			{
			len2 = GetDlgItemText(hwnd, IDC_PASSWORD2, (LPSTR) &plaintext4, MAXPWLEN+1);
			if (len2 == 0)
					{
						memset(passwd2,0,MAXPWLEN);
					}
			else
					{
						vncEncryptPasswd(plaintext4,passwd2);
					}
			}

			if (!AuthRequired && (strlen(passwd)!=0 || strlen(passwd2)!=0))
			{
				int result=MessageBox(NULL,"Warning: You Disabled authentication, but still have a password set\nUnless you blank the password authentication will still be required\nBLANK PASSWORD ?","Error",MB_YESNO);
				if (result==IDYES)
				{
					memset(passwd,0,8);
					memset(passwd2,0,8);
				}

			}

			error_passwd1=false;error_passwd2=false;
			if (strcmp(passwd,passwd2) == 0 && len!=0)
			{
				error_passwd1=true;error_passwd2=true;
			}
			if (AuthRequired==1 && strlen(passwd)==0 && strlen(passwd2)==0)
			{
				error_passwd1=true;error_passwd2=true;
			}
			else if (AuthRequired==1 && strlen(passwd)==0 )
			{
				error_passwd1=true;
			}
			else if (AuthRequired==1 && strlen(passwd2)==0)
			{
				error_passwd2=true;
			}
		

			if (strcmp(passwd,passwd2) == 0 && len!=0)
			{
				SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"The <view only> and the <full> password are identical. \n You always have <full> access.");
			}
			else if (AuthRequired==1 && strlen(passwd)==0 && strlen(passwd2)==0)
			{
				SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"Authentication error: You enabled authentication but no password have been set for <full> and <view only>");
			}
			else if (AuthRequired==1 && strlen(passwd)==0 )
			{
				SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"Authentication error: You enabled authentication but no password have been set for <full>");
			}
			else if (AuthRequired==1 && strlen(passwd2)==0)
			{
				SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"Authentication error: You enabled authentication but no password have been set for <view only>");
			}
			else SetWindowText(GetDlgItem(hwnd, IDC_STATIC_WARNINGBOX),"");

			InvalidateRect(hwnd,NULL,false);

			return TRUE;
			}
	}
	return (INT_PTR)FALSE;
}