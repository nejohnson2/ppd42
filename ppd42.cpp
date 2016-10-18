/*
	PPD42.cpp - Library for the ShinyeiPPD42 dust sensor.
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

#include  "ppd42.h"

// variables for dust calulations
const double pi = 3.14159;
const double density = 1.65 * pow(10, 12);
const double r10 = 2.6 * pow(10, -6);
const double vol10 = (4.0 / 3.0) * pi * pow(r10, 3);
const double mass10 = density * vol10;
const double r25 = 0.44 * pow(10, -6);
const double vol25 = (4.0 / 3.0) * pi * pow(r25, 3);
const double mass25 = density * vol25;
const double K = 3531.5;

PPD42::PPD42(uint8_t INPUT_P1, uint8_t INPUT_P2)
{
	
	_INPUT_P1 = INPUT_P1;
	_INPUT_P2 = INPUT_P2;	
	_sampletime = sampletime;

	pinMode(_INPUT_P1, INPUT);
	pinMode(_INPUT_P2, INPUT);
}

void PPD42::begin()
{
	attachInterrupt(_INPUT_P1, _P1, CHANGE);
	attachInterrupt(_INPUT_P2, _P2, CHANGE);

}

void PPD42::end()
{
	cli();
}

void PPD42::P1()
{
  if (digitalRead(_INPUT_P1) == LOW)  {
    p1_timer_start = micros();
  }
  else  {
    //only worry about this if the timer has actually started
    if (p1_timer_start != 0)
    {
      p1_pulse_time += micros() - p1_timer_start;
      p1_timer_start = 0; //restart the timer
    }
  }	
}

/*
 * Interrupt for P2 >2.5
 */
void PPD42::P2() {
  if (digitalRead(_INPUT_P2) == LOW)  {
    p2_timer_start = micros();
  }
  else  {
    //only worry about this if the timer has actually started
    if (p2_timer_start != 0)
    {
      p2_pulse_time += micros() - p2_timer_start;
      p2_timer_start = 0; //restart the timer
    }
  }
}

/*
 * Generate PM10 and PM2.5 counts
 */
void PPD42::calcDust() {
  // Generates PM10 and PM2.5 count from LPO.
  // Derived from code created by Chris Nafis
  // http://www.howmuchsnow.com/arduino/airquality/grovedust/

  p1_ratio = p1_pulse_time / (sampletime_ms * 10.0);
  p2_ratio = p2_pulse_time / (sampletime_ms * 10.0);

  // from shinyei datasheet
  countP1 = 1.1 * pow(p1_ratio, 3) - 3.8 * pow(p1_ratio, 2) + 520 * p1_ratio + 0.62;
  countP2 = 1.1 * pow(p2_ratio, 3) - 3.8 * pow(p2_ratio, 2) + 520 * p2_ratio + 0.62;
  
  PM10count = countP2;
  PM25count = countP1 - countP2;

  // Calculate ug/m3 from counts.  Used in Dustduino
  // and originally developed by researchers at Drexel
  // http://github.com/nejohnson2/ShinyeiPPD42
  
  // begins PM10 mass concentration algorithm
  concLarge = (PM10count) * K * mass10;

  // next, PM2.5 mass concentration algorithm
  concSmall = (PM25count) * K * mass25;
}