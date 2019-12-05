/*
 * The Clear BSD License
 * Copyright 2019 Damon Zhang
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include "platform_stdio.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define NUMBER_STRING_LENGTH            16          /* will handle all 32 bit integers + ending '\0' */

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/
/******************************************************
 *               Variable Definitions
 ******************************************************/
/******************************************************
 *               Function Definitions
 ******************************************************/

static char number_string[ NUMBER_STRING_LENGTH ];

static int number_print(int32_t number_in, int num_digits, int leading_zeros, int base)
{
    int         i;
    uint32_t    number;
    int         negative = 0;
    int         digits_printed = 0;

    memset( number_string, 0x00, NUMBER_STRING_LENGTH);

    /* use a 32 bit unsigned number internally */
    number = (uint32_t)number_in;

    /* if negative and decimal base, put a '-' sign later */
    if ((number_in < 0) && (base == 10))
    {
        negative = 1;
        number = -number_in;
    }

    i = NUMBER_STRING_LENGTH - 1;
    number_string[i--] = '\0';
    while( i > 0 )
    {
        /* make sure we print at least 1 '0' ! */
        if( (number != 0) || (digits_printed == 0))
        {
            /* write lowest digit, then back up 1 */
            number_string[i--] = (((number % base) < 0x0a) ? ('0' + (number % base)) : (('a' - 0x0a) + (number % base)));
            digits_printed++;
        }
        else if( num_digits > 0)
        {
            number_string[i--] = (leading_zeros != 0) ? '0' : ' ';
            digits_printed++;
        }
        else if (negative != 0)
        {
            number_string[i--] = '-';
            negative = 0;
            digits_printed++;
        }
        else
        {
            break;
        }

        /* reduce by 1 digit in the given base */
        number /= base;

        num_digits--;
    }

    if (i > 0)
    {
        /* move to the start of the string number_stringer */
        memmove( &number_string[0], &number_string[i + 1], (NUMBER_STRING_LENGTH - i));
    }


    return (digits_printed + 1);
}

/* small, minimum support, formatted print */
int mini_printf( const char *format, ...)
{
    const char* percent_ptr;
    const char* curr_ptr;
    int count, precision;
    int leading_zeros, num_digits;
    int out_count;
    va_list ap;

    va_start(ap, format);

    out_count = 0;
    curr_ptr = format;
    while( (curr_ptr != NULL) && (*curr_ptr != '\0') )
    {
        leading_zeros = 0;
        num_digits = 0;

        if (*curr_ptr == '%')
        {
            percent_ptr = curr_ptr;
        }
        else
        {
            percent_ptr = (char *)strchr(curr_ptr, '%');
        }
        if (percent_ptr != NULL)
        {
            count = percent_ptr - curr_ptr;

            if (count > 0)
            {
                /* print up to the % */
                platform_stdio_write( curr_ptr, count );
                out_count += count;
            }

            count = 0;
            precision = 0; /* for defining string len using format "%.*s"  */
            percent_ptr++;  /* skip the '%' */
            /* skip the % and up to the identifier - we only handle %d, %x, %p, %c, %s
             * if there are format params for leading 0's or long vars, skip them
             */
            while( (*percent_ptr != '\0') && (*percent_ptr != '%') &&
                   (*percent_ptr != 'x')  && (*percent_ptr != 'p') &&
                   (*percent_ptr != 'X')  && (*percent_ptr != 'P') &&
                   (*percent_ptr != 'd')  &&
                   (*percent_ptr != 'c')  && (*percent_ptr != 's'))
            {
                switch(*percent_ptr)
                {
                    case '.':
                        if (*(percent_ptr + 1) == '*')
                        {
                            precision = va_arg(ap, uint32_t);
                            break;
                        }
                        break;
                    case '0':
                        if ((leading_zeros == 0) && (num_digits == 0))
                        {
                            leading_zeros = 1;
                            break;
                        }
                        /* fall through */
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                        num_digits *= 10;
                        num_digits += *percent_ptr - '0';
                        if (num_digits > (NUMBER_STRING_LENGTH - 1))
                        {
                            num_digits = NUMBER_STRING_LENGTH - 1;
                        }
                        break;
                    case 'l':
                        /* long_data = 1; ignore - 8, 16 bit numbers are promoted to int through ... args */
                        /* fall through */
                    default:
                        break;
                }
                percent_ptr++;
            }

            if (*percent_ptr == '%')
            {
                count = 1;
                platform_stdio_write( "%", count );
            }
            else if (*percent_ptr == 'd')
            {
                int32_t my_num = va_arg(ap, int32_t);

                count = number_print(my_num, num_digits, leading_zeros, 10);
                platform_stdio_write( number_string, count );
            }
            else if ((*percent_ptr == 'x') || (*percent_ptr == 'X') || (*percent_ptr == 'p') || (*percent_ptr == 'P'))
            {
                uint32_t my_num = va_arg(ap, uint32_t);

                if ((*percent_ptr == 'p') || (*percent_ptr == 'P'))
                {
                    /* print leading "0x" for addresses */
                    platform_stdio_write( "0x", 2 );
                }
                count = number_print(my_num, num_digits, leading_zeros, 16);
                platform_stdio_write( number_string, count );
            }
            else if (*percent_ptr == 'c')
            {
                char my_char = va_arg(ap, int);

                count = 1;
                platform_stdio_write( &my_char, count );
            }
            else if (*percent_ptr == 's')
            {
                char* my_string = va_arg(ap, char *);
                if (precision != 0)
                {
                    count = precision;
                }
                else
                {
                    count = strlen(my_string);
                }
                if (count > 0)
                {
                    platform_stdio_write( my_string, count );
                }
            }

            percent_ptr ++;    /* skip the format character */
            curr_ptr = percent_ptr;

            out_count += count;
        }
        else
        {
            count = strlen(curr_ptr);
            if (count > 0)
            {
                platform_stdio_write( curr_ptr, count );
            }
            out_count += count;

            curr_ptr = NULL;
        }
    }

    va_end(ap);

    return out_count;
}

int hex_dump_print(const void* data_ptr, uint16_t length, int show_ascii)
{
    uint8_t*  data = (uint8_t*)data_ptr;
    uint8_t* char_ptr;
    int i, count;


    if ((data == NULL) || (length == 0))
    {
        return -1;
    }

    count = 0;
    char_ptr = data;
    while (length > 0)
    {

        i = 0;
        while ((length > 0) && (i < 16))
        {
            mini_printf(" %02x", *data);
            i++;
            data++;
            length--;
            count++;
        }

        if (show_ascii != 0)
        {
            int fill = 16 - i;
            /* fill in for < 16 */
            while(fill > 0)
            {
                mini_printf("   ");
                fill--;
            }

            /* space between numbers and chars */
            mini_printf("    ");

            while (i > 0)
            {
                mini_printf("%c",(isprint(*char_ptr) ? *char_ptr : '.'));
                char_ptr++;
                i--;
            }
        }

        mini_printf("\r\n");
    }

    return count;
}
