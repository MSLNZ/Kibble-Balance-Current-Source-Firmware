/*
 * GEVCU.h
 *
Copyright (c) 2013 Collin Kidder, Michael Neuweiler, Charles Galpin
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:
The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef KIBBLE_REV1_H_
#define KIBBLE_REV1_H_

#include <arduino.h>
#include <math.h>
#include "AD5790.h"



#ifdef __cplusplus
extern "C" {
#endif

void loop();
void setup();
void initDACs();

#ifdef __cplusplus
} // extern "C"
#endif

#define UARTBAUD 115200
#define ENC28J60_CONTROL_CS 28
#define LED_CS 1
#define ETH_CLK 21;
#define RELEASE_LED_CS  PIOB -> PIO_SODR |= (1<<LED_CS)         
#define ASSERT_LED_CS  PIOB -> PIO_CODR |= (1<<LED_CS) 

#endif /* KIBBLE_REV1_H_ */
