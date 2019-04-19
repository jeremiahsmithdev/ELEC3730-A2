/*        $Id: Ass-02-Q01.c 1592 2019-02-05 14:38:48Z Peter $
 *  $Revision: 1592 $
 *      $Date: 2019-02-06 01:38:48 +1100 (Wed, 06 Feb 2019) $
 *    $Author: Peter $
 */

#include "Ass-02.h"
#ifdef STM32F407xx
#include "usart.h"
#endif

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE
//

void
CommandLineParserInit (void)
{
  // Print welcome message
  // printf("\014");
  printf ("\n");
  printf ("%s\n", ASS_STRING);
  printf ("%s\n", VER_STRING);
  printf ("Command Line Parser Example\n");
}

// Check for input and echo back
void
CommandLineParserProcess (void)
{
#ifdef STM32F407xx
  uint8_t c;
  // STEPIEN: Can't use getchar() here since need to continue if no character to read
  if (HAL_UART_Receive (&huart2, &c, 1, 0x0) == HAL_OK)
  {
    printf ("\nSERIAL: Got ");
    if ((c < 32) | (c > 126))
    {
      printf ("ASCII %d\n", c);
    }
    else
    {
      printf ("character '%c'\n", c);
    }
    HAL_GPIO_TogglePin (GPIOD, LD4_Pin); // Toggle LED4

    // STEPIEN: The following is some test code that can be removed
    //
    {
      int c1, c2;
      char si[] = "1234";
      int i;
      char sf1[] = "Testing";
      char sf2[] = "1.234";
      float f1 = 1.11;
      float f2 = 2.22;

      sscanf (si, "%d", &i);
      printf ("TEST: Input int string   : '%s'\n", si);
      printf ("TEST: Input int value    : %d\n", i);
      c1 = sscanf (sf1, "%f", &f1);
      c2 = sscanf (sf2, "%f", &f2);
      printf ("TEST: Input float string : '%s'\n", sf1);
      printf ("TEST: Input float value  : %f\n", f1);
      printf ("TEST: sscanf() returns   : %d\n", c1);
      printf ("TEST: Input float string : '%s'\n", sf2);
      printf ("TEST: Input float value  : %f\n", f2);
      printf ("TEST: sscanf() returns   : %d\n", c2);
    }
  }
#else
  int c;

  c = getchar();
  if (c<0)
  {
    printf("INFO: Exiting program.\n");
    exit(0);
  }
  printf("SERIAL: Got ");
  if ((c<32) | (c>126))
  {
    printf("ASCII %d\n", c);
  }
  else
  {
    printf("character '%c'\n", c);
  }
#endif
}
