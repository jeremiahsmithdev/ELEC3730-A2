/**
 * File: Ass-02-Q01.c
 * 
 * @author Thomas Courtney - c3175353
 * @author Jeremiah Smith - c3238179
 * @since 03-05-2019
 */
#include "Ass-02.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <time.h>

int isOperator(char );
int isFunction(char );
void mathParser(void);
float evaluatePostfix(char** queue, int qlen);
int associative(char token);
int greaterPrecedence(char first, char second);

#define MAX_MATH_STR 30				//This is the maximum number of characters that an expression may have in calculator.
#define MAX_DISP_STR 13				//This is the maximum number of characters that may be displayed on the screen at any time.

static int   mathStrLen = 0;
static char  mathString[32];			//This is the string that contains the characters that contain the entire mathematical expression.
static char* globalDisplayPointer;		//Holds scroll position. When new character is input is reset to that most up to date display.
static int cursorX[] = {89,106,123,140,157,174,191,208,225,242,259,276,293,310};
static int cursorPos;
static float result;
static char ans[32];




/* Prints a cursor to screen. i determines the position of cursor. */
static void printCursor(int i){

	//Clear Cursor Area
	BSP_LCD_SetTextColor (0xFFFF);						//Text Color: White.
	BSP_LCD_FillRect (81, 1, 239, 12);					//Write over screen.
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);					//Text Color: Black.

	//Determine pixel positions
	BSP_LCD_SetBackColor(0xFFFF);						//Background Color: White
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);					//Text Color: Black
	BSP_LCD_SetFont (&Font12);						//Font Size: 24
	BSP_LCD_DisplayChar(cursorX[i],1,'^');
	BSP_LCD_SetFont (&Font24);



	printf("Cursor Position: %d     ", cursorPos);
	printf("absolute Position: %d \n", absolutePosition());
}

/* Moves the displayed string around.*/
static void scroll(int direction){

	char showString[MAX_DISP_STR +1];
	int diff = mathStrLen - MAX_DISP_STR;								//The difference between the string length and maximum displayable characters.

	if(mathString != '\0'){										//If the display is not empty.

		/*
		 * direction			: Is the flag saying scroll to the right.
		 * diff < MAX_DISP_STR	: If false, you must stop scrolling or you'll scroll the display entirely off the screen.
		 * diff > 0 			: If false, the mathString is not yet greater than the maximum amount of display char, so you'd be scrolling content off the screen entirely.
		 */
		if(direction && ((globalDisplayPointer - mathString) <= (diff - 1)) && (diff > 0) && cursorPos == 12){	//Scroll Right
//	if(direction  && (diff > 0) && cursorPos == 12){	//Scroll Right
			globalDisplayPointer++;

			//Performs the assignment for the string to be displayed.
			for(int j = 0; j<MAX_DISP_STR; j++){
				showString[j] = *(globalDisplayPointer+j);
			}

			showString[MAX_DISP_STR] = '\0';


			/* Refresh Display */
			BSP_LCD_SetTextColor (0xFFFF);									//Text Color: White.
			BSP_LCD_FillRect (81, 0, 239, 46);								//Write over screen.
			BSP_LCD_SetTextColor (LCD_COLOR_BLACK);								//Text Color: Black.
			BSP_LCD_DisplayStringAt(85,13,showString,LEFT_MODE);		//Display Expression after scroll.

		}else if( (!direction) && (diff>0) && (globalDisplayPointer != mathString) && cursorPos == 0){		//Scroll Left					//Prevents negative scrolling back in time.

			globalDisplayPointer--;

			//Performs the assignment for the string to be displayed.
			for(int j = 0; j<MAX_DISP_STR; j++){
				showString[j] = *(globalDisplayPointer+j);
			}

			showString[MAX_DISP_STR] = '\0';

			/* Clear Screen */
			BSP_LCD_SetTextColor (0xFFFF);				//Text Color: White.
			BSP_LCD_FillRect (81, 0, 239, 46);			//Write over screen.
			BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.

			BSP_LCD_DisplayStringAt(85,13,showString,LEFT_MODE);		//Display Expression after scroll.


		}
		//For overflowed display case and cursor moving to start of string. Only want to execute if other two cases don't.
	}


//	if((globalDisplayPointer-mathString) <= 0 ){
		//Just for cursor. CASE: Full string is shown.
//		if(mathStrLen <= MAX_DISP_STR ){			//Don't ask me why 12, it's just what happened.
			if(direction &&  cursorPos < 12 && cursorPos < mathStrLen-1){
				cursorPos ++;
			}else if(!direction  && cursorPos > 0){
				cursorPos--;
			}
//	}else{
//		if(direction &&  cursorPos < (mathStrLen-1)){
//			cursorPos ++;
//		}else if(!direction  && cursorPos > 0){
//			cursorPos--;
//		}
//	}

//		}


		//if portion being shown don't move cursor.
		//we don't want to move cursor to left if string is being display from the beginning.
		//we don't want to move cursor to the right if portion is being displayed.

		//Cursor. CASE: String longer than display.
		//else if()
//		else{
//			if(direction  && cursorPos < (MAX_DISP_STR-1)){
//				cursorPos ++;
//			}else if(!direction && cursorPos > 0){
//				cursorPos--;
//			}
//		}

		printf("display ptr: %d    ",(globalDisplayPointer -mathString));
		printCursor(cursorPos);
}

/* Just writes over the entire text box with background color. Also Resets the flags for the free position on the screen.*/
static void ClearTextBox(){

	BSP_LCD_SetTextColor (0xFFFF);				//Text Color: White.
	BSP_LCD_FillRect (81, 0, 239, 46);			//Write over screen.
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.

	mathString[0] = '\0'; //Sets start of string to null again.
	mathStrLen = 0;
}

/* Displays an error when you go over maximum number of characters in expression. */
static void displayError(){

	BSP_LCD_SetTextColor (0xFFFF);				//Text Color: White.
	BSP_LCD_FillRect (81, 0, 239, 46);			//Write over screen.
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.


	BSP_LCD_DisplayStringAt(85,13,"Max Char 30",LEFT_MODE); 		//Display Error
	HAL_Delay(1000);

	BSP_LCD_SetTextColor (0xFFFF);				//Text Color: White.
	BSP_LCD_FillRect (81, 0, 239, 46);			//Write over screen.
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.

	BSP_LCD_DisplayStringAt(85,13,globalDisplayPointer,LEFT_MODE);				//Display Expression
	printCursor(cursorPos);
}

/* Update strings and display. */
static void updateDisplay(const char* theChar){ //PROBLEM IS MATHSTRING IS MUNTED.

	//This logic is for the cursor and only applies to when a new char is added.
	int diff = mathStrLen - MAX_DISP_STR;

	if(diff<0) cursorPos = mathStrLen;		//Whole string is shown.
	else{
		//Amount over is shown.
		cursorPos = 12;
	}

	mathString[mathStrLen] = '\0';
	if((mathStrLen) > MAX_MATH_STR){		//If max expression length exceeded.
		displayError();					 	// Display error message in text box.
	}
	else
	{
		//concatenate character onto expression.
		strcat(mathString,theChar);

		mathStrLen++;
		diff = mathStrLen - MAX_DISP_STR;											//Gets difference between length of string and maximum amount of display.

		if(diff > 0){
			globalDisplayPointer = (mathString + (mathStrLen - MAX_DISP_STR));		//If the mathStrLen is greater than display move display down the expression.
		}else{
			globalDisplayPointer = mathString;									    //Else if the expression doesn't yet exceed the display just display the entire mathString.
		}

		if(mathStrLen > MAX_DISP_STR) 	BSP_LCD_DisplayStringAt(85,13,globalDisplayPointer,LEFT_MODE);	//Expression > Display
		else 						 	BSP_LCD_DisplayStringAt(85,13,(mathString),LEFT_MODE);		//Expression < Display

		printCursor(cursorPos);
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

  /* Scroll Buttons */
  BSP_LCD_SetTextColor(0xF81E);			//Text Colour: Power Ranger Pink
  BSP_LCD_FillRect (0, 0, 80, 48);

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

  BSP_LCD_DrawVLine (79, 0, 240);		//0a: Updated for scroll buttons
  BSP_LCD_DrawVLine (80, 0, 240);		//0b: Updated for scroll buttons
  BSP_LCD_DrawVLine (159, 49, 192);		//1a
  BSP_LCD_DrawVLine (160, 49, 192);		//1b
  BSP_LCD_DrawVLine (239, 49, 192);		//2a
  BSP_LCD_DrawVLine (240, 49, 192);		//2b
  BSP_LCD_DrawVLine (319, 49, 192);		//3a
  BSP_LCD_DrawVLine (320, 49, 192);		//3b
  BSP_LCD_DrawVLine (39, 0, 48);		//Scrolla
  BSP_LCD_DrawVLine (40, 0, 48);		//Scrollb

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
  BSP_LCD_DisplayChar (253, 157, '.');
  BSP_LCD_DisplayChar (290, 160, '^');
  BSP_LCD_DisplayChar (250, 205, '(');
  BSP_LCD_DisplayChar (293, 205, ')');

  /* Scroll Buttons */
  BSP_LCD_SetBackColor(0xF81E);				//Background Color: Power Ranger Pink
  BSP_LCD_DisplayChar (10, 13, '<');
  BSP_LCD_DisplayChar (55, 13, '>');




}

/* Handles user inputs and passes the math expression as a string to a parser which returns result of mathematical expression. */
void CalculatorProcess ()
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
      else if((display.y >= 0) && (display.y <= 48)){		/* Scroll Buttons */
    	  if(display.x <= 40){
    		  scroll(0);
			  HAL_Delay(100);
    	  }else{											/* > */
    		  scroll(1);
    		  HAL_Delay(100);
    	  }
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
			   if(charRight()) deleteKey();									/* DEL */
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
			   equalsPressed();
			   HAL_Delay(100);
		   }else{
			   ansPressed();
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
				   updateDisplay(".");						/* % */
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

/* return true if char right else false. */
int charRight(){
	int flag = 0;
	//If cursor not in last position and str is shorter than screen.
	if(absolutePosition() != mathStrLen){
		flag = 1;
	}
	//else there's nothing to the right.

	return flag;
}

/* Return true if char left else false. */
int charLeft(){
	int flag = 0;

	if(cursorPos!=0){
		//There's always something to the left as long as the cursor isn't at the beginnning.
		flag = 1;
	}
	//else you're at the beginning and there's nothing to the left.
	return flag;
}

/* Deletes the last character entered. */
void deleteKey(){


	int diff = 0;


	if(mathStrLen != 0){

		//mathString[mathStrLen-1] = '\0'; 					//Replaces the last character of the string with a null
		//mathStrLen--;										//Update the length of the mathString.

		printf("Before deletion: %s\n", mathString);

		modifyMathString(1);								//Simply modifies the string.

		diff = mathStrLen - MAX_DISP_STR;
//
		if(diff <= 0){										//If the diff < 0 then the length of string is less than display so show whole string.
			if(cursorPos >= mathStrLen && cursorPos != 0){				//If delete character is last character.
				cursorPos = mathStrLen - 1 ;
			}

////			printf("making display ptr = mathString \n");
			globalDisplayPointer = mathString;				//This assignment means that the whole string will be shown from the beginning.
////			printf("String to be displayed: %s \n", globalDisplayPointer);
//
		}else{
//			cursorPos = 12;
			globalDisplayPointer = (mathString + (mathStrLen - MAX_DISP_STR));		//If the mathStrLen is greater than display move display down the expression.
////			printf("Global Ptr not updated \n");
		}


		/* Refresh Display */
		BSP_LCD_SetTextColor (0xFFFF);										//Text Color: White.
		BSP_LCD_FillRect (81, 0, 239, 46);									//Write over screen.
		BSP_LCD_SetTextColor (LCD_COLOR_BLACK);								//Text Color: Black.
		BSP_LCD_DisplayStringAt(85,13,globalDisplayPointer,LEFT_MODE);		//Display Expression after scroll.


		printCursor(cursorPos);
	}
}

/* Get the absolute position of the cursor relative to the string. */
int absolutePosition(){
	int absPos = 0;

	if(globalDisplayPointer == mathString){							//If full string shown then cursor is at absolute position.
		absPos = cursorPos;
	}else { 															//If partial string shown.
		absPos = (globalDisplayPointer - mathString) + cursorPos;	//String diplays from global Pointer.
	}

	return absPos;
}

/* Calculate the solution and display it. Store in ANS. */
void equalsPressed(){
	int diff = 0; 								//For calculating cursor position.
	mathParser();								//This has replaced the mathString with the float.
	mathStrLen = strlen(mathString);			//Update length of the string.

	diff = mathStrLen - MAX_DISP_STR;			//Get difference.

	BSP_LCD_SetTextColor (0xFFFF);				//Text Color: White.
	BSP_LCD_FillRect (81, 0, 239, 46);			//Write over screen.
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.

	if(diff <= 0){								//String length is less than screen.
		cursorPos = (mathStrLen-1);				//-1 because strLen is not zero based.
	}else{										//String is too long for screen so...
		cursorPos = 12;
	}

	printCursor(cursorPos);

	char showString[MAX_DISP_STR + 1];
	//Performs the assignment for the string to be displayed.
	for(int j = 0; j<MAX_DISP_STR; j++){
		showString[j] = *(mathString+j);
	}

	showString[MAX_DISP_STR] = '\0';

	BSP_LCD_DisplayStringAt(85,13,showString,LEFT_MODE);	//Display result on screen.
}

/* Insert ANS into equation. */
void ansPressed(){
	int i = 0;
	char num = ans[0];
	char* ansStr = &num;
	*(ansStr +1) = '\0';

	while(num != '\0'){

		updateDisplay(ansStr);

		i++;

		num = ans[i];
		ansStr = &num;
		*(ansStr +1) = '\0';

	}

}

/* Called if deletion or insert made. int i selects if del or ins made. */
void modifyMathString(int mode, char theChar)
{

	char tempString[32];
	strcpy(tempString, mathString);					//Holds a copy of the old string.
	int targetPos;

	if(mode){										//If delete performed. Remove the character to the right of the cursor (aka absolutePosition + 1).


			for(int i = 0; i <= absolutePosition(); i++){		//Get characters before deleted char pos.
				mathString[i] = tempString[i];					//Copy old characters into string.
				targetPos = i;
			}


			//get characters after deleted char. i is already initialized and sitting at where the position of deleted char.
			for(int i = targetPos+1; i<= mathStrLen; i++){
				mathString[i-1] = tempString[i];
			}

			mathStrLen --;								//Reduce length of string.


	}else{																//Insert
		for(int i = 0; i < absolutePosition(); i++){				//Get characters before inserted char pos.
						mathString[i] = tempString[i];					//Copy old characters into string.
						targetPos = i;
					}
		//insert new char.
		mathString[targetPos] = theChar;

		//get characters after inserted char. Target pos is sitting at position after inserted char.
		for(int i = targetPos + 1; i<= mathStrLen; i++){
			mathString[i] = tempString[i-1];
		}

		mathStrLen ++;								//Increase length of string.
	}

}


// ================ MATH PARSING SECTION =======================
//
// This function evaluates a maths expression that has already been converted
// from infix to postfix format
// From the mathsParser(), this takes an output queue and its length and returns
// a result of type float
float evaluatePostfix(char** queue, int qlen)
{

// define queue operations
#define opush(x) opStack[olen++] = x
#define opop() opStack[--olen]
	int olen = 0;		// length of operator stack
	float result = 0;	// stores final result
	float opStack[100];	// operator stack
/* 	for each token in the postfix expression: */
	for (int i = 0; i < qlen; i++)
	{
		for (int j = 0; j < olen; j++)
			printf("%g ", opStack[j]);

		char* token = queue[i];
		printf("[%s] ", token);
		for (int a = i+1; a < qlen; a++)
			printf("%s ", queue[a]);
		printf("\n");

		/*   if token is an operator: */
		/* printf("token is %s with length", token); */
		if (isOperator(token[0]) && !isdigit(token[1]))
		/* if (isOperator(token[0])) */
		{
			/*     operand_2 ← pop from the stack */
			float operand2 = opop();
			/*     operand_1 ← pop from the stack */
			float operand1 = opop();
			/* #<{(|     push token onto the stack |)}># */
			/*     result ← evaluate token with operand_1 and operand_2 */
			if (token[0] == '+')
				result = operand1 + operand2;
			if (token[0] == '-')
				result = operand1 - operand2;
			if (token[0] == 'x')
				result = operand1 * operand2;
			if (token[0] == '/')
				result = operand1 / operand2;
			if (token[0] == '^')
				result = pow(operand1, operand2);
			/*     push result back onto the stack */
			opush(result);
		}
		/*   else if token is an operand: */
		else if (isdigit(token[0]) || token[0] == '-')
		{
			// push token onto the stack
			opush(atof(token));
		}
	}
	/* result ← pop from the stack */
	result = opop();
	return result;
}


// returns 0 if left and 1 if right associative respectively
int associative(char token)
{
	if (token == '^')
		return 1;

	if (token == 'x')
		return 0;

	if (token == '/')
		return 0;

	if (token == '+')
		return 0;

	if (token == '-')
		return 0;
	return -1;
}


// returns -1 if first has greater prec, 0 if equal and
// 1 if second has greater
int greaterPrecedence(char first, char second)
{
	int fir = 0;
	int sec = 0;
	if (first == '^')
		fir = 4;
	else if (first == 'x')
		fir = 3;
	else if (first == '/')
		fir = 3;
	else if (first == '+')
		fir = 2;
	else if (first == '-')
		fir = 2;

	if (second == '^')
		sec = 4;
	else if (second == 'x')
		sec = 3;
	else if (second == '/')
		sec = 3;
	else if (second == '+')
		sec = 2;
	else if (second == '-')
		sec = 2;

	if (fir > sec)
	{
		printf("%c has higher precedence than %c\n", first, second);
		return -1;
	}
	if (fir == sec)
	{

		printf("%c has equal precedence to %c\n", first, second);
		return 0;
	}

		printf("%c has higher precedence than %c\n", second, first);
	return 1;
}

// returns whether or not a token is an operator or not
int isOperator(char token)
{
	if (token == 'x')
		return 1;
	if (token == '/')
		return 1;
	if (token == '+')
		return 1;
	if (token == '-')
		return 1;
	if (token == '*')
		return 1;
	if (token == '^')
		return 1;
	return 0;
}

// returns whether a token is a function or not
int isFunction(char token)
{
	// no functions implemented yet
	return 0;
}

// these define our queue and stack operations
#define qpush(x) queue[qlen++] = x
#define spush(x) stack[slen++] = x
#define spop() stack[--slen]

// SHUNTING-YARD ALGORITHM
//This function will parse the math expression and return the result OR an error if the expression made no sense.
static char* mathParser(char* mathString)
{
	char* stack[100];//malloc
	char* queue[100];//malloc
	int qlen = 0;
	int slen = 0;

	printf("math string is : ");
	for (int q = 0; q < strlen(mathString); q++)
		printf("%c", mathString[q]);
	printf("-end\n");

	/* 	while there are tokens to be read: */
	for (int j = 0; j < strlen(mathString); j++)
	{
/*     read a token. */
		char* token = (char*)malloc(sizeof(char)*100);
		token[0] = mathString[j];
		token[1] = '\0';
		printf("%dReads token '%s'", j, token);
		printf("	Action: ");
/*     if the token is a number, then: */
		/* if (isdigit(token[0])) */
		if (isdigit(token[0]) || (token[0] == '-' && !isdigit(mathString[j-1])&& isdigit(mathString[j+1])) || (token[0] == '-' && j == 0 && token[1] == '('))
		{
			int len = 0;
			token[len++] = token[0];
			// read til end of number
			while (isdigit(mathString[j+1]) || mathString[j+1] == '.')
			{
				j++;
				token[len++] = mathString[j];
			}
			token[len] = '\0';
			printf("num is %s\n", token);
			printf("Push it to the output queue\n");
			/*         push it to the output queue. */
			qpush(token);

		}
/*     if the token is a function then: */
		else if (isFunction(token[0]))
		{
			printf("Push it to the operator stack\n");
			/*         push it onto the operator stack  */
			spush(token);
		}
/*     if the token is an operator, then: */
		else if (isOperator(token[0]))
		{
			/*         while ((there is a function at the top of the operator stack) */
			while ((slen > 0 && isFunction(stack[slen-1][0])) 
					
					/*                or (there is an operator at the top of the operator stack with greater precedence) */
					|| (slen > 0 && isOperator(stack[slen-1][0]) && greaterPrecedence(stack[slen-1][0], token[0]) == -1) 
					
					/*                or (the operator at the top of the operator stack has equal precedence and is left associative)) */
					|| ((slen > 0 && isOperator(stack[slen-1][0]) && greaterPrecedence(stack[slen-1][0], token[0]) == 0 && associative(stack[slen-1][0]) == 0) 
						
						/*               and (the operator at the top of the operator stack is not a left parenthesis): */
						&& stack[slen-1][0] != '('))
			{
				printf("ENTER?\n");
				/*             pop operators from the operator stack onto the output queue. */
				printf("Pop stack to output\n");
				qpush(spop());
			}
			/*         push it onto the operator stack. */
			printf("Push it to the operator stack\n");
			spush(token);
		}
/*     if the token is a left paren (i.e. "("), then: */
		else if (token[0] == '(')
/*         push it onto the operator stack. */
		{
			printf("Push it to the operator stack\n");
			spush(token);
		}
/*     if the token is a right paren (i.e. ")"), then: */
		else if (token[0] == ')')
		{
			/*         while the operator at the top of the operator stack is not a left paren: */
			while (slen > 0 && stack[slen-1][0] != '(')
			{
				printf("Pop stack to output\n");

				/*             pop the operator from the operator stack onto the output queue. */
				qpush(spop());
				/*         #<{(| if the stack runs out without finding a left paren, then there are mismatched parentheses. |)}># */
			}
			
			/*         if there is a left paren at the top of the operator stack, then: */
			if (stack[slen-1][0] == '(')
			{
				printf("Pop stack\n");
				/*             pop the operator from the operator stack and discard it */
				spop();
			}
		}
	printf("Output queue: ");
	for (int a = 0; a < qlen; a++)
		printf("%s ", queue[a]);
	printf("	Operator stack: ");
	for (int a = 0; a < slen; a++)

		printf("%s ", stack[a]);
	printf("\n\n");
	}
	// end
	/* after loop, if operator stack not null, pop everything to output queue */
	/* if there are no more tokens to read: */
	/*     while there are still operator tokens on the stack: */
	while (slen > 0)
	{
		/*         #<{(| if the operator token on the top of the stack is a paren, then there are mismatched parentheses. |)}># */
		if (stack[slen-1][0] == '(' || stack[slen-1][0] == ')')
			printf("mismatched parenthesis\n");
		/*         pop the operator from the operator stack onto the output queue. */
		qpush(spop();)

	}
	/* exit algorithm. */

	// print final contents of stack and queue
	printf("Output queue: ");
	for (int a = 0; a < qlen; a++)
			printf("%s ", queue[a]);
	printf("	Operator stack: ");
	for (int a = 0; a < slen; a++)
			printf("%s ", stack[a]);
	printf("\n");

	//Getting the solution.
	result = evaluatePostfix(queue, qlen);	// evaluate postfix value read from stack.
	sprintf(&mathString, "%.2f", result);	//Store the reult in the string. Null terminate that badboi
	sprintf(&ans, "%.2f", result);		//Store the reult in the string. Null terminate that badboi
	printf("Result: %g\n", result);
}

