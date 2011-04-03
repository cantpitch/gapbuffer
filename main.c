/*
 * Copyright 2009 by Hans Van Slooten
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>
#include "GapBuffer.h"

int main(int argc, char *argv[])
{
    int ch;

	printf("=== Gap Buffer Demonstration ===\n");
	printf("<delete prev char> '['\n");
	printf("<move left>        '<'\n");
	printf("<move right>       '>'\n\n");

    gap_buffer_handle hbuf = gap_buffer_Create();
    gap_buffer_Print(hbuf);

    do
    {
        ch = getc(stdin);
		
        if (ch >= 32 && ch < 127 && strchr("<>[", ch) == NULL)
        {
            gap_buffer_Put_Char(hbuf, ch);
        }

        // Ctrl+d
        if (ch == '[')
        {
            gap_buffer_Delete_Chars(hbuf, 1);
        }

        if (ch == '<')
        {
            gap_buffer_Prev_Char(hbuf);
        }

        if (ch == '>')
        {
            gap_buffer_Next_Char(hbuf);
        }
        
        gap_buffer_Print(hbuf);
    }
    while (ch != 'q');

}