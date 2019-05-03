/**
 * File: Ass-02-Q01.c
 * 
 * @author Thomas Courtney - c3175353
 * @author Jeremiah Smith - c3238179
 * @since 03-05-2019
 */

#include "Ass-02.h"


// REPLACE THE EXAMPLE CODE WITH YOUR CODE 
//

void Ass_02_Main (void){

	#ifdef STM32F407xx
		uint16_t i = 0;
	#endif

	CommandLineParserInit ();

	#ifdef STM32F407xx
		CalculatorInit ();
	#endif

	// Loop indefinitely
	while (1){

		CommandLineParserProcess ();

		#ifdef STM32F407xx
			CalculatorProcess ();
		#endif

		#ifdef STM32F407xx

			if (i++ > 10000){
				HAL_GPIO_TogglePin (GPIOD, LD3_Pin); // Toggle LED3
				i = 0;
			}

		#endif
	}
}
