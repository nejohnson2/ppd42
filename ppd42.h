/*
	PPD42.h - Library for the ShinyeiPPD42 dust sensor.
	Fork this on github - https://github.com/nejohnson2/ppd42

	Copyright (c) 2016 Nicholas Johnson

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
	
*/

#ifndef __PPD42_h__
#define __PPD42_h__

#include <stdint.h>

class PPD42
{
	public:
		PPD42(uint8_t INPUT_P1, uint8_t INPUT_P2);
		boolean begin();

		// Digital Pin 1 - P1 - Particles >0.1
		// Digital Pin 2 - P2 - Particles >2.5
		
		volatile unsigned long p1_pulse_time;             // use volatile for shared variables
		volatile unsigned long p2_pulse_time;             // which is necessary when using
		volatile unsigned long p1_timer_start = 0;        // interrupts
		volatile unsigned long p2_timer_start = 0;

		unsigned long starttime = 0;                //
		int sampletime_ms = 5000;                   // should be 30s
		float p1_ratio, p2_ratio, countP1, countP2;
		float concSmall, concLarge, PM10count, PM25count = 0;

		void calcDust();

	private:
		void P1();
		void P2();
};

#endif