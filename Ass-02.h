//
/**
 * File: Ass-02.h
 * 
 * @author Thomas Courtney - c3175353
 * @author Jeremiah Smith - c3238179
 * @since 03-05-2019
 */

// Assignment 2 include file
#ifndef ASS_02_H_
#define ASS_02_H_

// Standard includes
#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#include "openx07v_c_lcd.h"
#include "touch_panel.h"
#include "main.h"


#else

#if WINNT
#include <windows.h>//EDIT
#endif

#include <stdio.h>
#include <stdint.h>

#if WINNT
#include <malloc.h>//EDIT
#endif

#if MAC
#include <sys/malloc.h>//EDIT
#endif

#endif

#include <string.h>

// Assignment and version strings
#define ASS_STRING "ELEC3730-2019 Semester 1 Assignment 2"
#define VER_STRING "Ver: 01 $Rev: 1592 $"



// Assignment main
extern void
Ass_02_Main (void);

//
// REPLACE THE EXAMPLE CODE WITH YOUR CODE 
//

// Question 1
extern void
CommandLineParserInit (void);
extern void
CommandLineParserProcess (void);

// Question 2
extern void
CalculatorInit (void);
extern void
CalculatorProcess (void);

// Library functions
#ifdef STM32F407xx
extern uint8_t
BSP_TP_Init (void);
extern uint8_t
BSP_TP_GetDisplayPoint (Coordinate *pDisplay);
#endif

#endif /* ASS_02_H_ */
