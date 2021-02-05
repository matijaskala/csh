/*	$OpenBSD: char.c,v 1.6 2015/10/26 16:27:04 naddy Exp $	*/
/*	$NetBSD: char.c,v 1.6 1995/03/21 09:02:26 cgd Exp $	*/

/*-
 * Copyright (c) 1980, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "char.h"

/* on default same as original map */
unsigned short _cmap[256] = {
/*	  0 nul		  1 soh		  2 stx		  3 etx	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	  4 eot		  5 enq		  6 ack		  7 bel	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	  8 bs		  9 ht		 10 nl		 11 vt	*/
	_CTR,		_CTR|_SP|_META,	_CTR|_NL|_META,	_CTR,

/*	 12 np		 13 cr		 14 so		 15 si	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	 16 dle		 17 dc1		 18 dc2		 19 dc3	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	 20 dc4		 21 nak		 22 syn		 23 etb	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	 24 can		 25 em		 26 sub		 27 esc	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	 28 fs		 29 gs		 30 rs		 31 us	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	 32 sp		 33 !		 34 "		 35 #	*/
	_SP|_META,	_PUN,		_QF|_PUN,	_META|_PUN,

/*	 36 $		 37 %		 38 &		 39 '	*/
	_DOL|_PUN,	_PUN,		_META|_CMD|_PUN,_QF|_PUN,

/*	 40 (		 41 )		 42 *		 43 +	*/
	_META|_CMD|_PUN,_META|_PUN,	_GLOB|_PUN,	_PUN,

/*	 44 ,		 45 -		 46 .		 47 /	*/
	_PUN,		_PUN,		_PUN,		_PUN,

/*	 48 0		 49 1		 50 2		 51 3	*/
	_DIG|_XD,	_DIG|_XD,	_DIG|_XD,	_DIG|_XD,

/*	 52 4		 53 5		 54 6		 55 7	*/
	_DIG|_XD,	_DIG|_XD,	_DIG|_XD,	_DIG|_XD,

/*	 56 8		 57 9		 58 :		 59 ;	*/
	_DIG|_XD,	_DIG|_XD,	_PUN,		_META|_CMD|_PUN,

/*	 60 <		 61 =		 62 >		 63 ?	*/
	_META|_PUN,	_PUN,		_META|_PUN,	_GLOB|_PUN,

/*	 64 @		 65 A		 66 B		 67 C	*/
	_PUN,		_LET|_UP|_XD,	_LET|_UP|_XD,	_LET|_UP|_XD,

/*	 68 D		 69 E		 70 F		 71 G	*/
	_LET|_UP|_XD,	_LET|_UP|_XD,	_LET|_UP|_XD,	_LET|_UP,

/*	 72 H		 73 I		 74 J		 75 K	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_UP,

/*	 76 L		 77 M		 78 N		 79 O	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_UP,

/*	 80 P		 81 Q		 82 R		 83 S	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_UP,

/*	 84 T		 85 U		 86 V		 87 W	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_UP,

/*	 88 X		 89 Y		 90 Z		 91 [	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_GLOB|_PUN,

/*	 92 \		 93 ]		 94 ^		 95 _	*/
	_ESC|_PUN,	_PUN,		_PUN,		_PUN,

/*	 96 `		 97 a		 98 b		 99 c	*/
  _QB|_GLOB|_META|_PUN,	_LET|_LOW|_XD,	_LET|_LOW|_XD,	_LET|_LOW|_XD,

/*	100 d		101 e		102 f		103 g	*/
	_LET|_LOW|_XD,	_LET|_LOW|_XD,	_LET|_LOW|_XD,	_LET|_LOW,

/*	104 h		105 i		106 j		107 k	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,

/*	108 l		109 m		110 n		111 o	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,

/*	112 p		113 q		114 r		115 s	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,

/*	116 t		117 u		118 v		119 w	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,

/*	120 x		121 y		122 z		123 {	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_GLOB|_PUN,

/*	124 |		125 }		126 ~		127 del	*/
	_META|_CMD|_PUN,_PUN,		_PUN,		_CTR,

#ifdef SHORT_STRINGS
/****************************************************************/
/* 128 - 255 The below is supposedly ISO 8859/1			*/
/****************************************************************/
/*	128 (undef)	129 (undef)	130 (undef)	131 (undef)	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	132 (undef)	133 (undef)	134 (undef)	135 (undef)	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	136 (undef)	137 (undef)	138 (undef)	139 (undef)	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	140 (undef)	141 (undef)	142 (undef)	143 (undef)	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	144 (undef)	145 (undef)	146 (undef)	147 (undef)	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	148 (undef)	149 (undef)	150 (undef)	151 (undef)	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	152 (undef)	153 (undef)	154 (undef)	155 (undef)	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	156 (undef)	157 (undef)	158 (undef)	159 (undef)	*/
	_CTR,		_CTR,		_CTR,		_CTR,

/*	160 nobreakspace 161 exclamdown	162 cent	163 sterling	*/
	_PUN, /* XXX */	_PUN,		_PUN,		_PUN,

/*	164 currency	165 yen		166 brokenbar	167 section	*/
	_PUN,		_PUN,		_PUN,		_PUN,

/*	168 diaeresis	169 copyright	170 ordfeminine	171 guillemotleft*/
	_PUN,		_PUN,		_PUN,		_PUN,

/*	172 notsign	173 hyphen	174 registered	175 macron	*/
	_PUN,		_PUN,		_PUN,		_PUN,

/*	176 degree	177 plusminus	178 twosuperior	179 threesuperior*/
	_PUN,		_PUN,		_PUN,		_PUN,

/*	180 acute	181 mu 		182 paragraph	183 periodcentered*/
	_PUN,		_PUN, /*XXX*/	_PUN,		_PUN,

/*	184 cedilla	185 onesuperior	186 masculine	187 guillemotright*/
	_PUN,		_PUN,		_PUN,		_PUN,

/*	188 onequarter	189 onehalf	190 threequarters 191 questiondown*/
	_PUN,		_PUN,		_PUN,		_PUN,

/*	192 Agrave	193 Aacute	194 Acircumflex	195 Atilde	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_UP,

/*	196 Adiaeresis	197 Aring	198 AE		199 Ccedilla	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_UP,

/*	200 Egrave	201 Eacute	202 Ecircumflex	203 Ediaeresis	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_UP,

/*	204 Igrave	205 Iacute	206 Icircumflex	207 Idiaeresis	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_UP,

/*	208 ETH		209 Ntilde	210 Ograve	211 Oacute	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_UP,

/*	212 Ocircumflex	213 Otilde	214 Odiaeresis	215 multiply	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_PUN,

/*	216 Ooblique	217 Ugrave	218 Uacute	219 Ucircumflex	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_UP,

/*	220 Udiaeresis	221 Yacute	222 THORN	223 ssharp	*/
	_LET|_UP,	_LET|_UP,	_LET|_UP,	_LET|_LOW,

/*	224 agrave	225 aacute	226 acircumflex	227 atilde	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,

/*	228 adiaeresis	229 aring	230 ae		231 ccedilla	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,

/*	232 egrave	233 eacute	234 ecircumflex	235 ediaeresis	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,

/*	236 igrave	237 iacute	238 icircumflex	239 idiaeresis	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,

/*	240 eth		241 ntilde	242 ograve	243 oacute	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,

/*	244 ocircumflex	245 otilde	246 odiaeresis	247 division	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_PUN,

/*	248 oslash	249 ugrave	250 uacute	251 ucircumflex	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,

/*	252 udiaeresis	253 yacute	254 thorn	255 ydiaeresis	*/
	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,	_LET|_LOW,
#endif /* SHORT_STRINGS */
};
