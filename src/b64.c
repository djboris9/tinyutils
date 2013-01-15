/*
    This file is part of tinyutils.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

    Tinyutils Copyright (C) 2013 Boris Djurdjevic <dev@djurdjevic.ch>
*/

#include <stdio.h>
#include <stdlib.h>

// #define DEBUG
#define STDIN_BUFFER 6



/**
  MASK 1: 11111100 00000000 00000000
  MASK 2: 00000011 11110000 00000000
  MASK 3: 00000000 00001111 11000000
  MASK 4: 00000000 00000000 00111111
*/

/******************************************************************************/

#define B8_B6_1(begin)  ((*begin     & 0xFC) >> 2)
#define B8_B6_2(begin) (((*begin     & 0x3)  << 4) | ((*(begin+1) & 0xF0) >> 4 ))
#define B8_B6_3(begin) (((*(begin+1) & 0xF)  << 2) | ((*(begin+2) & 0xC0) >> 6 ))
#define B8_B6_4(begin)   (*(begin+2) & 0x3F)
#define PAD  0x40
#define ENDL 0x0

/*
 * Debugging functions
 */
#ifdef DEBUG

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 

void b2bin(char x) {
	printf (""BYTETOBINARYPATTERN, BYTETOBINARY(x));
}
#endif


/*
 * Base64 Chars
 */
const char b64c[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";


/*
 * Buffering
 */
char *buf, *bufptr;
void flush(char newline) {
	*bufptr = ENDL;
	if (newline)
		puts(buf);
	else
		printf(buf);
	bufptr = buf;
}

void out(const char *c0, const char *c1, const char *c2, const char *c3) {
	// Encoding out
	*(bufptr+0)  = *c0;
	*(bufptr+1)  = *c1;
	*(bufptr+2)  = *c2;
	*(bufptr+3)  = *c3;
	bufptr      += 4;

	if (bufptr-buf >= 76)
		flush(1);
}


/*
 * Convert functions
 */
void enc(unsigned char* input, int* readlen) {
	int offset;
	unsigned char* begin = input - 3;

	/*
	 * Process each 6 Bit group
	 */
	for (offset = 0; offset <= *readlen-3; offset+=3) {
		begin = input+offset;
		out(&b64c[B8_B6_1(begin)], &b64c[B8_B6_2(begin)], &b64c[B8_B6_3(begin)], &b64c[B8_B6_4(begin)]);
	}

	/*
	 * Do padding
	 */
	short pad = 3 - (*readlen % 3);
	if (pad == 1) { 
		begin += 3;
		out(&b64c[B8_B6_1(begin)], &b64c[B8_B6_2(begin)], &b64c[((*(begin+1) & 0xF) << 2)], &b64c[PAD]);
	}
	else if (pad == 2) {
		begin += 3;
		out(&b64c[B8_B6_1(begin)], &b64c[((*begin & 0x3 ) << 4)], &b64c[PAD], &b64c[PAD]);
	}
}

void dec(unsigned char* input, int* readlen) {
	puts("Not implemented yet");
}

/*
 * Main
 */
int main(int argc, char **argv) {
	/*
	 * Outbuffer initialization, we will also use a 76 Buffer for decode
	 */
	buf    = (char*) malloc(77); //76 width + \0
	bufptr = buf;

	short decode = 0;
/*	if (argc >= 1 && **(argv+1) == '-' && *(*(argv+1)+1) == 'd')
		decode = 1;
*/
	/*
	 * Variables for file_handling
	 */
	unsigned char buffer[STDIN_BUFFER];
	FILE *in;
	int   bytes_read = 0;
 
	//in=fopen("/dev/stdin","r");
	in=fopen("testfile.dat","r");
 
	if(in != NULL){
		while((bytes_read = fread(&buffer, 1, STDIN_BUFFER, in))) {
			if (decode)
				dec(buffer, &bytes_read);
			else
				enc(buffer, &bytes_read);
		}

		// Flush with newline
		flush(1);

		fclose(in);
	} else {
		fprintf(stderr, "ERROR opening STDIN!\n");
		exit(1);
	}

	// Cleanup
	free(buf);

	return 0;
}
