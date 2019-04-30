#include "Ass-02.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MATH_STR 34				//This is the maximum number of characters that an expression may have in calculator.
#define MAX_DISP_STR 17				//This is the maximum number of characters that may be displayed on the screen at any time.


int characterPositionsX[10] = {15,47,79,111,143,175,207,239,271,303};	//X-Places in textbox. Maybe even these out (minus all by 5 or something).
static int characterSpots[10] = {0};
static int mathStrLen = 0;
static char mathString = '\0';									//This is the string that contains the characters that contain the entire mathematical expression.



/*
 * Takes in a mathematical expression as a String and parses it for the solution.
 * */
static void mathParser(void){

	//This function will parse the math expression and return the result OR an error if the expression made no sense.
	// +, -, x, / ,% , ( , ) , ., =, ^,
}


/* Just writes over the entire text box with background color. Also Resets the flags for the free position on the screen.*/
static void ClearTextBox(){

	BSP_LCD_SetTextColor (0xFFFF);				//Text Color: White.
	BSP_LCD_FillRect (0, 0, 320, 46);			//Write over screen.
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.

	mathString = '\0'; //Sets start of string to null again.
	mathStrLen = 0;
}

/* Displays an error when you go over maximum number of characters in expression. */
static void displayError(){

	char* error = "Max Char";
	uint8_t UCharError = (uint8_t)*error;

	BSP_LCD_SetTextColor (0xFFFF);				//Text Color: White.
	BSP_LCD_FillRect (0, 0, 320, 46);			//Write over screen.
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.


	BSP_LCD_DisplayStringAt(15,13,"Max Char Reached",LEFT_MODE); 		//Display Error
	HAL_Delay(1000);

	BSP_LCD_SetTextColor (0xFFFF);				//Text Color: White.
	BSP_LCD_FillRect (0, 0, 320, 46);			//Write over screen.
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.

	BSP_LCD_DisplayStringAt(15,13,&mathString,LEFT_MODE);				//Display Expression

}

/* Update strings and display. */
static void updateDisplay(const char* theChar){ //PROBLEM IS MATHSTRING IS MUNTED.

	char* displayPointer;					//Points to beginning of portion of expression.

	if((mathStrLen) > MAX_MATH_STR){		//If max expression length exceeded.
		displayError();					 	// Display error message in text box.
	}
	else
	{
		//concatenate character onto expression.
		printf("A. MathString: %s Length: %d \n",&mathString, mathStrLen);
		strcat(&mathString,theChar);

		mathStrLen++;
		displayPointer = (&mathString + (mathStrLen - MAX_DISP_STR));

		printf("B. MathString: %s Length: %d \n\n",&mathString, mathStrLen);

		if(mathStrLen > MAX_DISP_STR) 	BSP_LCD_DisplayStringAt(15,13,displayPointer,LEFT_MODE);	//Expression > Display
		else 						 	BSP_LCD_DisplayStringAt(15,13,(&mathString),LEFT_MODE);		//Expression < Display

	}

}


void CalculatorInit (void)
{

  // Initialize and turn on LCD and calibrate the touch panel
  BSP_LCD_Init ();
  BSP_LCD_DisplayOn ();
  BSP_TP_Init ();


  BSP_LCD_Clear (LCD_COLOR_WHITE);


  /*** Drawing Display: Consult Spreadsheet ***/


  /** DRAWING BOXES **/

  /* Numbers [0-9] */
  BSP_LCD_SetTextColor(0x00FF);
  BSP_LCD_FillRect (0, 49, 240, 192);

  /* Operations */
  BSP_LCD_SetTextColor(0xFF00);
  BSP_LCD_FillRect (241, 49, 79, 192);	// [+, -, x, %]
  BSP_LCD_SetTextColor(0x1685);
  BSP_LCD_FillRect (161, 193, 80, 48);	// [=]

  /* Clear Button  */
  BSP_LCD_SetTextColor(0xE0A2);
  BSP_LCD_FillRect (81, 193, 80, 48);	// [C]

  /* Draw Grid (Borders are 2 thick) */
  BSP_LCD_SetTextColor (LCD_COLOR_BLACK);
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

  //Updated buttons vert lines.
  BSP_LCD_DrawVLine (119, 193, 48);		//1a
  BSP_LCD_DrawVLine (120, 193, 48);		//1b
  BSP_LCD_DrawVLine (199, 193, 48);		//2a
  BSP_LCD_DrawVLine (200, 193, 48);		//2b
  BSP_LCD_DrawVLine (279, 49 , 192);	//3a
  BSP_LCD_DrawVLine (280 ,49 , 192);	//3b

  /** DRAWING CHARACTERS **/

  BSP_LCD_SetTextColor (LCD_COLOR_BLACK);	//Text Color: Black
  BSP_LCD_SetFont (&Font24);				//Font Size: 24

  /* Col 0 */
  BSP_LCD_SetBackColor(0x00FF);				//Background Color: Blue
  BSP_LCD_DisplayChar (32, 61, '7');
  BSP_LCD_DisplayChar (32, 109, '4');
  BSP_LCD_DisplayChar (32, 157, '1');
  BSP_LCD_DisplayChar (32, 205, '0');

  /* Col 1 */
  BSP_LCD_SetBackColor(0x00FF);				//Background Color: Blue
  BSP_LCD_DisplayChar (114, 61, '8');
  BSP_LCD_DisplayChar (114, 109, '5');
  BSP_LCD_DisplayChar (114, 157, '2');
  /* Split Cell */
  BSP_LCD_SetBackColor(0xE0A2);				//Background Color: Red
  BSP_LCD_DisplayChar (93, 205, 'C');
  BSP_LCD_SetFont (&Font12);				//Font Size: 12
  BSP_LCD_DisplayStringAt (130, 210, "DEL",LEFT_MODE);	//Trial and errored into place.
  BSP_LCD_SetFont (&Font24);				//Font Size: 24

  /* Col 2 */
  BSP_LCD_SetBackColor(0x00FF);				//Background Color: Blue
  BSP_LCD_DisplayChar (194, 61, '9');
  BSP_LCD_DisplayChar (194, 109, '6');
  BSP_LCD_DisplayChar (194, 157, '3');
  /* Split Cell */
  BSP_LCD_SetBackColor(0x1685);				//Background Color: Green
  BSP_LCD_DisplayChar (173, 205, '=');
  BSP_LCD_SetFont (&Font12);				//Font Size: 12
  BSP_LCD_DisplayStringAt (210, 210, "ANS",LEFT_MODE);	//Trial and errored into place.
  BSP_LCD_SetFont (&Font24);				//Font Size: 24

  /* Col 3: All Split*/
  BSP_LCD_SetBackColor(0xFF00);				//Background Color: Yellow
  BSP_LCD_DisplayChar (253, 61, '+');
  BSP_LCD_DisplayChar (290, 61, '-');
  BSP_LCD_DisplayChar (253, 109, 'x');
  BSP_LCD_DisplayChar (290, 109, '/');
  BSP_LCD_DisplayChar (253, 157, '%');
  BSP_LCD_DisplayChar (290, 160, '^');
  BSP_LCD_DisplayChar (250, 205, '(');
  BSP_LCD_DisplayChar (293, 205, ')');





}


/* Handles user inputs and passes the math expression as a string to a parser which returns result of mathematical expression. */
void CalculatorProcess (void)
{

  if (BSP_TP_GetDisplayPoint (&display) == 0)	//Reading in the touched points.
  {

	BSP_LCD_SetBackColor(0xFFFF);				//Set Background Color: White
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Set Text Color: Black.


    if (((display.x >= 0) && (display.x <= 80)))			/* COL1: [7,4,1,0] */
    {
      if ((display.y >= 49) && (display.y < 96)) 			/* 7 */
      {
    	  updateDisplay("7");
    	  HAL_Delay(100);
      }
      else if ((display.y >= 97) && (display.y <= 144)) 	/* 4 */
      {
    	  updateDisplay("4");
		  HAL_Delay(100);
      }
      else if ((display.y >= 145) && (display.y <= 192))  	/* 1 */
      {
    	  updateDisplay("1");
		  HAL_Delay(100);
      }
      else if ((display.y >= 193) && (display.y <= 240))	/* 0 */
      {
    	  updateDisplay("0");
		  HAL_Delay(100);
      }
    }
    else if (((display.x >= 81) && (display.x <= 160))){	/* COL2: [8,5,2,[C,DEL]] */

    	if ((display.y >= 49) && (display.y < 96))			/* 8 */
	   {
		  updateDisplay("8");
		  HAL_Delay(100);
	   }
	   else if ((display.y >= 97) && (display.y <= 144))	/* 5 */
	   {
		  updateDisplay("5");
		  HAL_Delay(100);
	   }
	   else if ((display.y >= 145) && (display.y <= 192))	/* 2 */
	   {

		  updateDisplay("2");
		  HAL_Delay(100);
	   }
	   else if ((display.y >= 193) && (display.y <= 240))
	   {
		   if(display.x <= 120){							/* C */
			   ClearTextBox();
			   HAL_Delay(100);
		   }else{
			   updateDisplay("D");							/* DEL */
			   HAL_Delay(100);
		   }
	   }
    }
    else if (((display.x >= 161) && (display.x <= 240))){	 /* COL3: [9,6,3,[=,ANS]] */

    	if ((display.y >= 49) && (display.y < 96))			/* 9 */
	   {
	      updateDisplay("9");
		  HAL_Delay(100);
	   }
	   else if ((display.y >= 97) && (display.y <= 144))	/* 6 */
	   {
		  updateDisplay("6");
		  HAL_Delay(100);
	   }
	   else if ((display.y >= 145) && (display.y <= 192)) 	/* 3 */
	   {
		  updateDisplay("3");
		  HAL_Delay(100);
	   }
	   else if ((display.y >= 193) && (display.y <= 240))
	   {
		   if(display.x <= 200){
			   updateDisplay("=");							/* = */
			   HAL_Delay(100);
		   }else{
			   updateDisplay("A");							/* ANS */
			   HAL_Delay(100);
		   }
	   }
    }
	else if (((display.x >= 241) && (display.x <= 320))){	 /* COL4: [ [+, -], [x, /], [%, ^], [(, )] ] */

	   if ((display.y >= 49) && (display.y < 96))
	   {
		   if(display.x <= 280){
				   updateDisplay("+");						/* + */
				   HAL_Delay(100);
			   }else{
				   updateDisplay("-");						/* - */
				   HAL_Delay(100);
			   }
	   }
	   else if ((display.y >= 97) && (display.y <= 144))
	   {
		   if(display.x <= 280){
				   updateDisplay("x");						/* x */
				   HAL_Delay(100);
			   }else{
				   updateDisplay("/");						/* / */
				   HAL_Delay(100);
			   }
	   }
	   else if ((display.y >= 145) && (display.y <= 192))
	   {
		   if(display.x <= 280){
				   updateDisplay("%");						/* % */
				   HAL_Delay(100);
			   }else{
				   updateDisplay("^");						/* ^ */
				   HAL_Delay(100);
			   }
	   }
	   else if ((display.y >= 193) && (display.y <= 240))
	  {
		   if(display.x <= 280){
				   updateDisplay("(");						/* ( */
				   HAL_Delay(100);
			   }else{
				   updateDisplay(")");						/* ) */
				   HAL_Delay(100);
			   }
	  }
	}//End Col
  }// End Button Checks
}//End CalculatorProcess

















