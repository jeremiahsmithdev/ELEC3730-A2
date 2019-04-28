/*        $Id: Ass-02-Q02.c 1592 2019-02-05 14:38:48Z Peter $
 *  $Revision: 1592 $
 *      $Date: 2019-02-06 01:38:48 +1100 (Wed, 06 Feb 2019) $
 *    $Author: Peter $
 */

#include "Ass-02.h"

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE 
//

void CalculatorInit (void)
{

  // Initialize and turn on LCD and calibrate the touch panel
  BSP_LCD_Init ();
  BSP_LCD_DisplayOn ();
  BSP_TP_Init ();

//   Display welcome message											/* No Longer Required */
   BSP_LCD_Clear (LCD_COLOR_WHITE);
   BSP_LCD_SetFont (&Font12);
   BSP_LCD_SetTextColor (LCD_COLOR_BLACK);
   BSP_LCD_DisplayStringAtLine (0, (uint8_t*) ASS_STRING);
   BSP_LCD_DisplayStringAtLine (1, (uint8_t*) VER_STRING);
   BSP_LCD_DisplayStringAtLine (2, (uint8_t*) "Calculator Example");

  /* Drawing Button Boxes: Consult Spreadsheet */




  /** Background Highlighting **/


  /* Numbers [0-9] */
  BSP_LCD_SetTextColor(0x00FF);
  BSP_LCD_FillRect (0, 49, 240, 192);

  /* Operations [+,-,x,%,=] */
  BSP_LCD_SetTextColor(0xFF00);
  BSP_LCD_FillRect (241, 49, 80, 192);


  BSP_LCD_SetTextColor (LCD_COLOR_BLACK);
  BSP_LCD_SetFont (&Font24);
  BSP_LCD_DisplayChar (290, 205, 'C');

  /* Clear Button [C] */



  /* Draw Grid */

  BSP_LCD_DrawHLine (0,47, 320);		//0a
  BSP_LCD_DrawHLine (0,48, 320);		//0b
  BSP_LCD_DrawHLine (0,95, 320);		//1a
  BSP_LCD_DrawHLine (0,96, 320);		//1b
  BSP_LCD_DrawHLine (0,143, 320);		//2a
  BSP_LCD_DrawHLine (0,144, 320);		//2c
  BSP_LCD_DrawHLine (0,191, 320);		//3a
  BSP_LCD_DrawHLine (0,192, 320);		//3b
  BSP_LCD_DrawHLine (0,239, 320);		//4a
  BSP_LCD_DrawHLine (0,240, 320);		//4b

  BSP_LCD_DrawVLine (79, 49, 192);		//0a
  BSP_LCD_DrawVLine (80, 49, 192);		//0b
  BSP_LCD_DrawVLine (159, 49, 192);		//1a
  BSP_LCD_DrawVLine (160, 49, 192);		//1b
  BSP_LCD_DrawVLine (239, 49, 192);		//2a
  BSP_LCD_DrawVLine (240, 49, 192);		//2b
  BSP_LCD_DrawVLine (319, 49, 192);		//3a
  BSP_LCD_DrawVLine (320, 49, 192);		//3b


}

void CalculatorProcess (void)
{
  // STEPIEN: Assume horizontal display

  uint16_t linenum = 0;

  if (BSP_TP_GetDisplayPoint (&display) == 0)
  {
    if (((display.y < 190) && (display.y >= 38)))
    {
      if ((display.x >= 318) || (display.x < 2))
      {
      }
      else
      {
        BSP_LCD_FillCircle (display.x, display.y, 2);
        printf ("TOUCH:  Got (%3d,%3d)\n", display.x, display.y);
      }
    }
    else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 180)
        && (display.x <= 210))
    {
      BSP_LCD_SetTextColor (LCD_COLOR_ORANGE);
    }
    else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 215)
        && (display.x <= 245))
    {
      BSP_LCD_SetTextColor (LCD_COLOR_CYAN);
    }
    else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 250)
        && (display.x <= 280))
    {
      BSP_LCD_SetTextColor (LCD_COLOR_YELLOW);
    }
    else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 5)
        && (display.x <= 35))
    {
      BSP_LCD_SetTextColor (LCD_COLOR_RED);
    }
    else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 40)
        && (display.x <= 70))
    {
      BSP_LCD_SetTextColor (LCD_COLOR_BLUE);
    }
    else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 75)
        && (display.x <= 105))
    {
      BSP_LCD_SetTextColor (LCD_COLOR_GREEN);
    }
    else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 110)
        && (display.x <= 140))
    {
      BSP_LCD_SetTextColor (LCD_COLOR_BLACK);
    }
    else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 145)
        && (display.x <= 175))
    {
      BSP_LCD_SetTextColor (LCD_COLOR_MAGENTA);
    }
    else if ((display.y <= 230) && (display.y >= 190) && (display.x >= 285)
        && (display.x <= 315))
    {
      BSP_LCD_SetFont (&Font12);
      for (linenum = 3; linenum < 16; linenum++)
      {
        BSP_LCD_ClearStringLine (linenum);
      }
    }
    else
    {
    }
  }
}
