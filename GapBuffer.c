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

#include "GapBuffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#define BUFFER_INITIAL_SIZE             65536
//#define BUFFER_EXPAND_INCREMENT         1024
#define BUFFER_INITIAL_SIZE             20
#define BUFFER_EXPAND_INCREMENT         5

struct gap_buffer_t
{
	char *buffer;
    char *gap_start;
    char *gap_end;
    char *buffer_end; 
    int buffer_size;

    char *cursor;
};

/* "Private" members */
void gap_buffer_Resize_Buffer(gap_buffer_handle hbuf);
void gap_buffer_Move_Gap(gap_buffer_handle hbuf);

gap_buffer_handle gap_buffer_Create()
{
    gap_buffer_handle hbuf = malloc(sizeof(struct gap_buffer_t));

    hbuf->buffer = malloc(sizeof(char) * BUFFER_INITIAL_SIZE);
    hbuf->gap_start = hbuf->buffer;
    hbuf->gap_end = hbuf->buffer + BUFFER_INITIAL_SIZE;
    hbuf->buffer_end = hbuf->buffer + BUFFER_INITIAL_SIZE;
    hbuf->buffer_size = BUFFER_INITIAL_SIZE;
    hbuf->cursor = hbuf->gap_start;

    return hbuf;
}

void gap_buffer_Print(gap_buffer_handle hbuf)
{
    char *buffer = hbuf->buffer;

    printf("buffer:    %p  buffer_end: %p  buffer_size: %d bytes\n", hbuf->buffer, hbuf->buffer_end, hbuf->buffer_size);
    printf("gap_start: %p  gap_end:    %p\n", hbuf->gap_start, hbuf->gap_end);
    printf("cursor:    %p\n\n", hbuf->cursor);

    printf("[ ");
    while (buffer < hbuf->buffer_end)
    {
        if (buffer >= hbuf->gap_start && buffer < hbuf->gap_end)
        {
            printf("_ ");
        }
        else
        {
            printf("%c ", *buffer);
        }
        buffer++;
    }
    printf(" ]\n");

    /* Gap line */
    buffer = hbuf->buffer;
    printf("  ");
    while (buffer < hbuf->buffer_end)
    {
        if (buffer == hbuf->gap_start && buffer == hbuf->gap_end)
            printf("X ");
        else if (buffer == hbuf->gap_start)
            printf("> ");
        else if (buffer == hbuf->gap_end)
            printf("< ");
        else
            printf("  ");

        buffer++;
    }
    printf("\n");

    /* Cursor line */
    buffer = hbuf->buffer;
    printf("  ");
    while (buffer < hbuf->cursor)
    {
        printf("  ");

        buffer++;
    }
    printf("^\n");
}

int gap_buffer_Size(gap_buffer_handle hbuf)
{
    return (hbuf->buffer_end - hbuf->buffer) - (hbuf->gap_end - hbuf->gap_start);
}

int gap_buffer_Current_Pos(gap_buffer_handle hbuf)
{
    if (hbuf->cursor > hbuf->buffer_end)
    {
        return (hbuf->cursor - hbuf->buffer) - (hbuf->gap_end - hbuf->gap_start);
    }
    else
    {
        return hbuf->cursor - hbuf->buffer;
    }
}

void gap_buffer_Set_Cursor(gap_buffer_handle hbuf, int position)
{
    hbuf->cursor = hbuf->buffer + position;

    if (hbuf->cursor > hbuf->gap_start)
    {
        hbuf->cursor += hbuf->gap_end - hbuf->gap_start;
    }
}

char gap_buffer_Next_Char(gap_buffer_handle hbuf)
{
    if (hbuf->cursor == hbuf->gap_start)
    {
        hbuf->cursor = hbuf->gap_end;
    }
    
    return *(++hbuf->cursor);
}

char gap_buffer_Prev_Char(gap_buffer_handle hbuf)
{
    if (hbuf->cursor == hbuf->gap_end)
    {
        hbuf->cursor = hbuf->gap_start;
    }
    
    return *(--hbuf->cursor);
}

void gap_buffer_Put_Char(gap_buffer_handle hbuf, char ch)
{
    if (hbuf->gap_end == hbuf->gap_start)
    {
        gap_buffer_Resize_Buffer(hbuf);
    }

    gap_buffer_Move_Gap(hbuf);

    *hbuf->cursor = ch;
    hbuf->cursor++;
    hbuf->gap_start++;
}

void gap_buffer_Delete_Chars(gap_buffer_handle hbuf, int num_chars)
{
    gap_buffer_Move_Gap(hbuf);
    
    hbuf->cursor = (hbuf->gap_start -= num_chars);
}

void gap_buffer_Replace_Char(gap_buffer_handle hbuf, char ch)
{
    if (hbuf->cursor == hbuf->gap_start)
    {
        hbuf->cursor = hbuf->gap_end;
    }

    if (hbuf->cursor == hbuf->buffer_end)
    {
        gap_buffer_Resize_Buffer(hbuf);
    }

    *hbuf->cursor = ch;
}

void gap_buffer_Insert_String(gap_buffer_handle hbuf, char *s, int s_length)
{
    gap_buffer_Move_Gap(hbuf);

    while (s_length > (hbuf->gap_end - hbuf->gap_start))
    {
        gap_buffer_Resize_Buffer(hbuf);
    }

    do
    {
        gap_buffer_Put_Char(hbuf, *(s++));
    } 
    while (s_length-- > 0);
}

/*
 * gap_buffer_Resize_Buffer
 *
 * Expand the total size of the buffer, usually because we've filled
 * up the gap.
 *
 */
void gap_buffer_Resize_Buffer(gap_buffer_handle hbuf)
{
    int gap_size = hbuf->gap_end - hbuf->gap_start;
    int end_size = hbuf->buffer_end - hbuf->gap_end;
    int start_size = hbuf->gap_start - hbuf->buffer;
    int cursor_index = hbuf->cursor <= hbuf->gap_start 
        ? hbuf->cursor - hbuf->buffer 
        : hbuf->cursor - gap_size - hbuf->buffer;

    char *buffer = realloc(hbuf->buffer, sizeof(char) * (hbuf->buffer_size + BUFFER_EXPAND_INCREMENT));

    /* Our old gap pointers could be bad after the realloc, so reset them */
    hbuf->buffer = buffer;
    hbuf->buffer_size += BUFFER_EXPAND_INCREMENT;
    hbuf->buffer_end = hbuf->buffer + hbuf->buffer_size;
    hbuf->gap_start = hbuf->buffer + start_size;
    hbuf->gap_end = hbuf->buffer + start_size + gap_size;
    hbuf->cursor = (hbuf->buffer + cursor_index) <= hbuf->gap_start 
        ? hbuf->buffer + cursor_index
        : hbuf->gap_end + (cursor_index - start_size);

    /* Move the end down to make room for the new gap */
    memmove(hbuf->gap_end + BUFFER_EXPAND_INCREMENT, hbuf->gap_end, sizeof(char) * end_size); 

    hbuf->gap_end += BUFFER_EXPAND_INCREMENT;
}

/*
 * gap_buffer_Move_Gap
 *
 * Moves the current insert point for the buffer to the cursor
 *
 */
void gap_buffer_Move_Gap(gap_buffer_handle hbuf)
{
    /* Determine if we are moving backwards in the buffer (i.e. before the gap)
     * or forward (i.e. after the gap)
     */
    if (hbuf->cursor == hbuf->gap_start)
    {
        return;
    }

    if (hbuf->cursor == hbuf->gap_end)
    {
        hbuf->cursor = hbuf->gap_start;
        return;
    }

    if (hbuf->cursor < hbuf->gap_start)
    {
        int chunk_size = hbuf->gap_start - hbuf->cursor;

        hbuf->gap_start -= chunk_size;
        hbuf->gap_end -= chunk_size;

        memmove(hbuf->gap_end, hbuf->gap_start, sizeof(char) * chunk_size); 

    }
    else
    {
        int chunk_size = hbuf->cursor - hbuf->gap_end;

        memmove(hbuf->gap_start, hbuf->gap_end, sizeof(char) * chunk_size);

        hbuf->gap_start += chunk_size;
        hbuf->gap_end = hbuf->cursor;
        hbuf->cursor = hbuf->gap_start;
    }
}