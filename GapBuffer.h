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

#ifndef __GAPBUFFER_H__HEADER__
#define __GAPBUFFER_H__HEADER__

typedef struct gap_buffer_t *gap_buffer_handle;

void gap_buffer_Print(gap_buffer_handle hbuf);

gap_buffer_handle gap_buffer_Create();
int gap_buffer_Size(gap_buffer_handle hbuf);
int gap_buffer_Current_Pos(gap_buffer_handle hbuf);
void gap_buffer_Set_Cursor(gap_buffer_handle hbuf, int position);
char gap_buffer_Next_Char(gap_buffer_handle hbuf);
char gap_buffer_Prev_Char(gap_buffer_handle hbuf);
void gap_buffer_Put_Char(gap_buffer_handle hbuf, char ch);
void gap_buffer_Delete_Chars(gap_buffer_handle hbuf, int num_chars);
void gap_buffer_Replace_Char(gap_buffer_handle hbuf, char ch);
void gap_buffer_Insert_String(gap_buffer_handle hbuf, char *s, int s_length);


#endif