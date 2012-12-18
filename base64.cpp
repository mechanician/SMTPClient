#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
/*
** Translation Table as described in RFC1113
*/
static const char cb64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* 
 * Function 'encodeblock' is taken from http://base64.sourceforge.net
 */
static void encodeblock( unsigned char *in, unsigned char *out, int len )
{
    out[0] = (unsigned char) cb64[ (int)(in[0] >> 2) ];
    out[1] = (unsigned char) cb64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ (int)(in[2] & 0x3f) ] : '=');
}

char* toBase64(char const * const instr){
	int i = 0;
	int j = 0;
	int k = 0;
	unsigned char in[3];
	unsigned char out[4];
	int slen = strlen(instr);
	int len = slen % 3;
	int res = (len > 0) ? 1 : 0;
	char *outstr = (char*)malloc(((slen - len)*4)/3 + 4*res); 	// sizeof(char) = 1 byte -- always! (by defenition)
	while (i < slen - len){
		for(j = 0; j < 3; j++){
			in[j] = instr[i++];
		}
		encodeblock(in, out, 3);
		for(j = 0; j < 4; j++)
			outstr[k++] = out[j];
	}
	for(j = 0; j < len; j++)
		in[j] = instr[i++];
	encodeblock(in, out, len);
	for(j = 0; j < 3; j++)
		outstr[k++] = out[j];

	return outstr;
}
