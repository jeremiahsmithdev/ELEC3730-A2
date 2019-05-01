#include "Ass-02.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_MATH_STR 20				//This is the maximum number of characters that an expression may have in calculator.
#define MAX_DISP_STR 10				//This is the maximum number of characters that may be displayed on the screen at any time.
int characterPositionsX[10] = {15,47,79,111,143,175,207,239,271,303};	//X-Places in textbox. Maybe even these out (minus all by 5 or something).
static int characterSpots[10] = {0};

static uint8_t* mathString;			//This is the string that contains the characters that contain the entire mathematical expression.
static uint8_t* theResult;
static int mathStringP;				//This is the pointer for the math string.

// ================ MATH PARSING SECTION =======================

int isOperator(char );
int isFunction(char );
static void mathParser(void);
static int getWordCharCount(char* word, char delim);
int string_parser(char *inp, char **array_of_words_p[]);
static int getNumWords(char *s, char delim);
float evaluatePostfix(char* queue, int qlen);
static int getSize(char *s);



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
	if (token == '%')
		return 1;
	if (token == '^')
		return 1;
	return 0;
}

int isFunction(char token)
{
	// no functions implemented yet
	return 0;
}

#define qpush(x) queue[qlen++] = x
#define spush(x) stack[slen++] = x
#define spop() stack[--slen]

static void mathParser(void)
{
	char stack[100];//malloc
	char queue[100];//malloc
	int qlen = 0;
	int slen = 0;

	char* mathString = "3+4x2/(1-5)^2^3";	// currently works on single digits in a string like so
	/* mathString = "((15/(7−(1+1)))x3)−(2+(1+1))"; */
	for (int j = 0; j < 15; j++)
		printf("%c", mathString[j]);
	printf("\n");

	/* char* mathString = "12+43"; */
	//This function will parse the math expression and return the result OR an error if the expression made no sense.
	// +, -, x, / ,% , ( , ) , ., =, ^,
	// Shunting-yard algorithm
	/* 	while there are tokens to be read: */
	for (int j = 0; j < 15; j++)
	{
/*     read a token. */
		char token = mathString[j];
		printf("Reads token '%c'", token);
		printf("	Action: ");
/*     if the token is a number, then: */
		if (isdigit(token))
		{
			printf("Push it to the output queue\n");
			/*         push it to the output queue. */
			qpush(token);
	/* printf("Output queue: "); */
	/* for (int a = 0; a < qlen; a++) */
	/* 		printf("%c", queue[a]); */

		}
/*     if the token is a function then: */
		if (isFunction(token))
		{
			printf("Push it to the operator stack\n");
			/*         push it onto the operator stack  */
			spush(token);
		}
/*     if the token is an operator, then: */
		if (isOperator(token))
		{
			/*         while ((there is a function at the top of the operator stack) */
			while ((slen > 0 && isFunction(stack[slen-1])) 
					
					/*                or (there is an operator at the top of the operator stack with greater precedence) */
					|| (slen > 0 && isOperator(stack[slen-1]) && greaterPrecedence(stack[slen-1], token) == -1) 
					
					/*                or (the operator at the top of the operator stack has equal precedence and is left associative)) */
					|| ((slen > 0 && isOperator(stack[slen-1]) && greaterPrecedence(stack[slen-1], token) == 0 && associative(stack[slen-1]) == 0) 
						
						/*               and (the operator at the top of the operator stack is not a left parenthesis): */
						&& stack[slen-1] != '('))
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
		if (token == '(')
/*         push it onto the operator stack. */
		{
			printf("Push it to the operator stack\n");
			spush(token);
		}
/*     if the token is a right paren (i.e. ")"), then: */
		if (token == ')')
		{
			/*         while the operator at the top of the operator stack is not a left paren: */
			while (slen > 0 && stack[slen-1] != '(')
			{
				printf("Pop stack to output\n");

				/*             pop the operator from the operator stack onto the output queue. */
				qpush(spop());
				/*         #<{(| if the stack runs out without finding a left paren, then there are mismatched parentheses. |)}># */
			}
			
			/*         if there is a left paren at the top of the operator stack, then: */
			if (stack[slen-1] == '(')
			{
				printf("Pop stack\n");
				/*             pop the operator from the operator stack and discard it */
				spop();
			}
		}
	printf("Output queue: ");
	for (int a = 0; a < qlen; a++)
			printf("%c ", queue[a]);
	printf("	Operator stack: ");
	for (int a = 0; a < slen; a++)
			printf("%c ", stack[a]);
	printf("\n\n");
	}
	// end
/* after while loop, if operator stack not null, pop everything to output queue */
/* if there are no more tokens to read: */
/*     while there are still operator tokens on the stack: */
	while (slen > 0)
	{
		/*         #<{(| if the operator token on the top of the stack is a paren, then there are mismatched parentheses. |)}># */
		if (stack[0] == '(' || stack[0] == ')')
			printf("mismatched parenthesis\n");
		qpush(spop();)

	}
/*         pop the operator from the operator stack onto the output queue. */
/* exit. */

	printf("Output queue: ");
	for (int a = 0; a < qlen; a++)
			printf("%c ", queue[a]);
	printf("	Operator stack: ");
	for (int a = 0; a < slen; a++)
			printf("%c ", stack[a]);
	printf("\n");

	float result = evaluatePostfix(queue, qlen);
	printf("Result: %g\n", result);
}

float evaluatePostfix(char* queue, int qlen)
{

#define opush(x) opStack[olen++] = x
#define opop() opStack[--olen]
	int olen = 0;
	float result = 0;
	float opStack[100];
/* 	for each token in the postfix expression: */
	for (int i = 0; i < qlen; i++)
	{
		for (int j = 0; j < olen; j++)
			printf("%g ", opStack[j]);

		char token = queue[i];
		printf("[%c] ", token);
		for (int a = i+1; a < qlen; a++)
			printf("%c ", queue[a]);
		printf("\n");

		/*   if token is an operator: */
		if (isOperator(token))
		{
			/*     operand_2 ← pop from the stack */
			float operand2 = opop();
			/*     operand_1 ← pop from the stack */
			float operand1 = opop();
			/* #<{(|     push token onto the stack |)}># */
			/*     result ← evaluate token with operand_1 and operand_2 */
			if (token == '+')
				result = operand1 + operand2;
			if (token == '-')
				result = operand1 - operand2;
			if (token == 'x')
				result = operand1 * operand2;
			if (token == '/')
				result = operand1 / operand2;
			if (token == '^')
				result = pow(operand1, operand2);
			/*     push result back onto the stack */
			opush(result);
		}
		/*   else if token is an operand: */
		else if (isdigit(token))
		{
			// push token onto the stack
			opush(atof(&token));
		}
	}
	/* result ← pop from the stack */
	result = opop();
	return result;
}

int string_parser(char *inp, char **array_of_words_p[]) {

	int sizeOfInput = getSize(inp);
	int wordsInInput = getNumWords(inp, ' ');
	int count = wordsInInput;

	if(wordsInInput == 0 || sizeOfInput == 0) return 0;

	*array_of_words_p = (char**)malloc(sizeof(char*) * wordsInInput);
	wordsInInput = 0;
	char* carrier;      //Points to characters in the input.
	int numChar = 0;


	for(int i = 0; i < sizeOfInput; i++){
		carrier = inp + i;
		if(*(carrier) != ' '){
			numChar = getWordCharCount((carrier), ' ');
			(*array_of_words_p)[wordsInInput] = (char*)malloc(sizeof(char)*(numChar+1));
			i+= numChar;
			wordsInInput++;
			numChar = 0;
		}
	}

	wordsInInput = 0;
	numChar = 0;


	for(int i = 0; i < sizeOfInput; i++){
		carrier = inp + i;
		if(*(carrier) != ' '){                                          //If delimiter reached stop writing word to array.
			(*array_of_words_p)[wordsInInput][numChar] = *(carrier);
			numChar++;
		}

		if(*(carrier) == ' ' && i!=0){                                  //If end of word reached and it's no beginning of input.
			if(*(carrier - 1) != ' '){                                    //If previous character was not delim, then a new word has been logged.
				(*array_of_words_p)[wordsInInput][numChar] = '\0';          //Append NULL to end of string to terminate word.
				wordsInInput++;                                             //Count the word.
			}

			numChar = 0;                                                  //Reset numbe rof characters.
		}

		if(i == (sizeOfInput - 1) && *(carrier) != ' '){                //If end of input reached and last character is not delim then append the NULL.
			(*array_of_words_p)[wordsInInput][numChar] = '\0';            //Append NULL to end of string to terminate word.
		}
	}


	/* return getNumWords(inp,' '); */
	return count;
}

static int getWordCharCount(char* word, char delim){
	int numChar = 0;
	int i = 0;

	while(*(word+i) != delim){
		numChar++;
		i++;
	}


	return numChar;
}

static int getNumWords(char *s, char delim){

	char* t;                                  //This pointer will move through the input input pointer's contents.
	char tPrev;
	int wordCount = 0;
	int flag1 = 0;
	int flag2 = 0;                      //Flags for determining if word is present.



	for(t = s+1; *t !='\0'; t++){             //Loop begins at second character.

		tPrev = *(t-1);                         //Hold previous character.

		if((*t != delim && tPrev == delim) || (t == (s+1) && tPrev != delim)){     //Check if the beginning of word has been found.
			flag1 = 1;
		}
		if((tPrev != delim && *t == delim) || ((t-s) == (getSize(s)-1) && *t != delim)){//Check if the end of word has been found.
			flag2 = 1;
		}


		if(flag1 && flag2){                     //If both flags set a whole word has been found!
			wordCount++;
			flag1 = 0;                            //Reset the flags.
			flag2 = 0;
		}

	}
	return wordCount;


}

static int getSize(char *s){

	char* t;                        //This pointer will move through the input input pointer's contents.
	for(t = s; *t !='\0'; t++){}    //Moves the t through contents of s until it NULL terminator
	return t - s;                   //The difference in address between two pointers is the size of s. s points to beggining and t points to the end.
}
// ================ MATH PARSING SECTION END =======================

/*
 * Just writes over the entire text box with background color.
 * Also Resets the flags for the free position on the screen.
 *
 * */
static void ClearTextBox(){


	BSP_LCD_SetTextColor (0xFFFF);				//Text Color: White.
	BSP_LCD_FillRect (0, 0, 320, 46);			//Write over screen.
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.

	//Reset Flags.
	for(int i = 0; i<10; i++){
		characterSpots[i] = 0;
	}
}

/*
 *  Manages where characters may be written to in the TextBox.
 */
static int characterManager(){


	//Loop through and mark next available position.
	for(int i = 0; i < 10; i++){

		if(characterSpots[i] == 0){
			return i;
		}
	}

	//If we got to here then the screen is full. Put some sort of user feedback in here.
	//For now just clear textbox.
	//ClearTextBox();
	return 0;


}

/* Displays an error when you go over maximum number of characters in expression. */
static void displayError(){

	ClearTextBox();											//Clear Display
	BSP_LCD_DisplayStringAtLine(0,"MAX CHAR REACHED");		//Show Error
	ClearTextBox();											//Clear Display
	BSP_LCD_DisplayStringAtLine(0,mathString);				//Display String Again

}

/* Update strings and display. */
static void updateDisplay(char theChar){

	if(strlen(mathString) > MAX_MATH_STR){					//calculates length based on characters up to but not including null.

		displayError();										//displays the error.

	}else{

		strcat(mathString,theChar);							//Update string

		BSP_LCD_DisplayStringAtLine(0, mathString);			//display string
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

  /* Draw Grid (Borders) */
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
  BSP_LCD_SetBackColor(0xE0A2);				//Background Color: Red
  BSP_LCD_DisplayChar (114, 205, 'C');

  /* Col 2 */
  BSP_LCD_SetBackColor(0x00FF);				//Background Color: Blue
  BSP_LCD_DisplayChar (194, 61, '9');
  BSP_LCD_DisplayChar (194, 109, '6');
  BSP_LCD_DisplayChar (194, 157, '3');
  BSP_LCD_SetBackColor(0x1685);				//Background Color: Green
  BSP_LCD_DisplayChar (194, 205, '=');

  /* Col 3 */
  BSP_LCD_SetBackColor(0xFF00);				//Background Color: Yellow
  BSP_LCD_DisplayChar (274, 61, '+');
  BSP_LCD_DisplayChar (274, 109, '-');
  BSP_LCD_DisplayChar (274, 157, 'x');
  BSP_LCD_DisplayChar (274, 205, '%');





}

void CalculatorProcess (void)
{

  uint16_t linenum = 0;
  int charPos = characterManager();

  if (BSP_TP_GetDisplayPoint (&display) == 0)	//Reading in the touched points.
  {
	/*** If statements describe a box around touch zone corresponding to buttons. ***/
	BSP_LCD_SetBackColor(0xFFFF);				//Background Color: White
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.

	/** Column 1 [7,4,1,0] **/
    if (((display.x >= 0) && (display.x <= 80)))
    {
      /* 7 */
      if ((display.y >= 49) && (display.y < 96))
      {
//    	 BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '7');	//Get X pos from array.
//    	 characterSpots[charPos] = 1;									//Update available locations in textbox.
    	 updateDisplay();
    	  HAL_Delay(500);												//Stops it from wigging out.

      }
      /* 4 */
      else if ((display.y >= 97) && (display.y <= 144))
      {
    	  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '4');	//Get X pos from array.
		  characterSpots[charPos] = 1;									//Update available locations in textbox.
		  HAL_Delay(500);	//Stops it from wigging out.
      }
      /* 1 */
      else if ((display.y >= 145) && (display.y <= 192))
      {
    	  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '1');	//Get X pos from array.
		  characterSpots[charPos] = 1;									//Update available locations in textbox.
		  HAL_Delay(500);	//Stops it from wigging out.
      }
      /* 0 */
      else if ((display.y >= 193) && (display.y <= 240))
      {
    	  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '0');	//Get X pos from array.
		  characterSpots[charPos] = 1;									//Update available locations in textbox.
		  HAL_Delay(500);	//Stops it from wigging out.
      }


    }
    /** Column 2 [8,5,2,C] **/
    else if (((display.x >= 81) && (display.x <= 160))){
          /* 8 */
		   if ((display.y >= 49) && (display.y < 96))
		   {
			 BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '8');	//Get X pos from array.
			 characterSpots[charPos] = 1;									//Update available locations in textbox.
			 HAL_Delay(500);												//Stops it from wigging out.
		   }
		   /* 5 */
		   else if ((display.y >= 97) && (display.y <= 144))
		   {
			  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '5');	//Get X pos from array.
			  characterSpots[charPos] = 1;									//Update available locations in textbox.
			  HAL_Delay(500);	//Stops it from wigging out.
		   }
		   /* 2 */
		   else if ((display.y >= 145) && (display.y <= 192))
		   {
			  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '2');	//Get X pos from array.
			  characterSpots[charPos] = 1;									//Update available locations in textbox.
			  HAL_Delay(500);	//Stops it from wigging out.
		   }
		   /* C */
		   else if ((display.y >= 193) && (display.y <= 240))
		  {
			 ClearTextBox();
			 HAL_Delay(500);
		  }


    }
    /** Column 2 [9,6,3,=] **/
    else if (((display.x >= 161) && (display.x <= 240))){
          /* 8 */
		   if ((display.y >= 49) && (display.y < 96))
		   {
			 BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '9');	//Get X pos from array.
			 characterSpots[charPos] = 1;									//Update available locations in textbox.
			 HAL_Delay(500);												//Stops it from wigging out.
		   }
		   /* 5 */
		   else if ((display.y >= 97) && (display.y <= 144))
		   {
			  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '6');	//Get X pos from array.
			  characterSpots[charPos] = 1;									//Update available locations in textbox.
			  HAL_Delay(500);	//Stops it from wigging out.
		   }
		   /* 2 */
		   else if ((display.y >= 145) && (display.y <= 192))
		   {
			  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '3');	//Get X pos from array.
			  characterSpots[charPos] = 1;									//Update available locations in textbox.
			  HAL_Delay(500);	//Stops it from wigging out.
		   }
		   /* C */
		   else if ((display.y >= 193) && (display.y <= 240))
		  {
			  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '=');	//Get X pos from array.
			  characterSpots[charPos] = 1;									//Update available locations in textbox.
			  HAL_Delay(500);	//Stops it from wigging out.
		  }


    }
    /** Column 2 [+,-,x,%] **/
	else if (((display.x >= 241) && (display.x <= 320))){
		  /* 8 */
		   if ((display.y >= 49) && (display.y < 96))
		   {
			 BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '+');	//Get X pos from array.
			 characterSpots[charPos] = 1;									//Update available locations in textbox.
			 HAL_Delay(500);												//Stops it from wigging out.
		   }
		   /* 5 */
		   else if ((display.y >= 97) && (display.y <= 144))
		   {
			  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '-');	//Get X pos from array.
			  characterSpots[charPos] = 1;									//Update available locations in textbox.
			  HAL_Delay(500);	//Stops it from wigging out.
		   }
		   /* 2 */
		   else if ((display.y >= 145) && (display.y <= 192))
		   {
			  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, 'x');	//Get X pos from array.
			  characterSpots[charPos] = 1;									//Update available locations in textbox.
			  HAL_Delay(500);	//Stops it from wigging out.
		   }
		   /* C */
		   else if ((display.y >= 193) && (display.y <= 240))
		  {
			  BSP_LCD_DisplayChar (characterPositionsX[charPos], 13, '%');	//Get X pos from array.
			  characterSpots[charPos] = 1;									//Update available locations in textbox.
			  HAL_Delay(500);	//Stops it from wigging out.
		  }


	}


  }



}//End CalculatorProcess

















