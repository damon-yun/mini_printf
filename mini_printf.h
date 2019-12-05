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

/*
 * Small footprint formatted print directly to stdout
 *
 * NOTES:
 *  These values are upgraded to 32-bit values when variable args are used.
 *      char        unsigned char
 *      int8_t      uint8_t
 *      int16_t     uint16_t
 *
 * Uses:
 *    platform_stdio_write()
 *    va_start(), va_arg(), va_stop()
 *    strlen()
 *    memset(), memmove().
 *
 * Supports:
 *
 *  %%      - Prints the '%' character
 *
 *  %d      - decimal number
 *              supports negative
 *              ignores 'l' modifier (all values upgraded to longs)
 *              supports leading spaces and zeros
 *
 *                  mini_printf("%d %2d %04d %ld\r\n", num1, num2, num3, num4);
 *
 *  %x      - hexadecimal number
 *              ignores 'l' modifier (all values upgraded to longs)
 *              supports leading spaces and zeros
 *
 *                  mini_printf("0x%x 0x%2x 0x%04x 0x%lx\r\n", num1, num2, num3, num4);
 *
 *  %c      - single character
 *
 *                  mini_printf("%c %c%c\r\n", 'W', 'H', 'i');
 *
 *  %s      - character string
 *
 *                  mini_printf("%s %s", my_string, "YeeHaw\r\n");
 *
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

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

int mini_printf( const char* format, ...);
int hex_dump_print(const void* data, uint16_t length, int show_ascii);

#ifdef __cplusplus
} /* extern "C" */
#endif
