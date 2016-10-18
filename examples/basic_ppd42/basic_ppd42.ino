/*
	Basic PPD42 Example
	by Nicholas E. Johnson

	Pin 1
	Pin 2
*/

#include <PPD42>

PPD42 shinyei1 = PPD42();

void setup() {
  shinyei1.setPeriod(30); // 30seconds
  shinyei1.begin();
}

void loop() {

  shinyei1.readPM25();
  shinyei1.readPM10();
}
