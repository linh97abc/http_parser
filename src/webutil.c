/*
 * Copyright (C) 2017 - 2020 Xilinx, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <webutil.h>
// #include "lwip_example_webserver.h"

#define HTTP_ARG_ARRAY_SIZE 1000

// static http_arg httpArgArray[HTTP_ARG_ARRAY_SIZE];
static int p_arg_count;
static int httpArgArrayIndex;

int is_cmd_print(char *buf)
{
	/* skip past 'POST /' */
	buf += 6;

	/* then check for cmd/printxhr */
	return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "printxhr", 8));
}

void extract_file_name(char *filename, char *req, int rlen, int maxlen)
{
	char *fstart, *fend;
	int offset;

	/* first locate the file name in the request */
	/* requests are of the form GET /path/to/filename HTTP... */

	offset = strlen("GET ");

	if (req[offset] == '/')
		offset++;

	fstart = req + offset; /* start marker */

	/* file name finally ends in a space */
	while (req[offset] != ' ')
		offset++;

	fend = req + offset - 1; /* end marker */

	if (fend < fstart)
	{
		strcpy(filename, "index.htm");
		return;
	}

	/* if there is something wrong with the URL & we ran for for more than
	 * the HTTP buffer length (offset > rlen) or the filename is too long,
	 * throw 404 error */
	if (offset > rlen || fend - fstart > maxlen)
	{
		*fend = 0;
		strcpy(filename, "404.htm");
		printf("Request filename is too long, length = %d, file = %s (truncated), max = %d\r\n",
			   (int)(fend - fstart), fstart, maxlen);
		return;
	}

	/* copy over the filename */
	strncpy(filename, fstart, fend - fstart + 1);
	filename[fend - fstart + 1] = 0;

	/* if last character is a '/', append index.htm */
	if (*fend == '/')
		strcat(filename, "index.htm");
}

char *get_file_extension(char *fname)
{
	char *fext = fname + strlen(fname) - 1;

	while (fext > fname)
	{
		if (*fext == '.')
			return fext + 1;
		fext--;
	}

	return NULL;
}

static const char *DictExt[] =
	{
		"text/plain",
		"text/html",
		"text/css",
		"text/javascript",
		"text/csv",
		"text/xml",
		"image/jpeg ",
		"image/png ",
		"image/gif",
		"application/json",
		"application/zip",
		"application/pdf",
		"video/mp4",
		"video/webm",
};

int generate_http_header(char *buf, enum WebUtilFileExt fext, bool isCompress, int fsize)
{
	char lbuf[40];

	strcpy(buf, "HTTP/1.1 200 OK\r\nContent-Type: ");

	strcat(buf, DictExt[fext]);
	strcat(buf, "\r\n");

	if (isCompress)
	{
		strcat(buf, "content-encoding: gzip\r\n");
	}

	// sprintf(lbuf, "content-encoding: gzip");
	sprintf(lbuf, "Content-length: %d", fsize);
	strcat(buf, lbuf);
	strcat(buf, "\r\n");

	strcat(buf, "Connection: close\r\n");
	strcat(buf, "\r\n");

	return strlen(buf);
}
