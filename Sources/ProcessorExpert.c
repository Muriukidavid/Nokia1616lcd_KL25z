/* ###################################################################
**     Filename    : ProcessorExpert.c
**     Project     : ProcessorExpert
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-07-26, 00:13, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file ProcessorExpert.c
** @version 01.01
** @date 2014-07-26, 00:13, # CodeGen: 0
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup ProcessorExpert_module ProcessorExpert module documentation
**  @{
*/         
/* MODULE ProcessorExpert */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "RESET.h"
#include "BitIoLdd1.h"
#include "CS.h"
#include "BitIoLdd2.h"
#include "MOSI.h"
#include "BitIoLdd3.h"
#include "SCK.h"
#include "BitIoLdd4.h"
#include "WAIT1.h"
#include "UTIL1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "main.h"
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
enum logics{false, true};
uint16_t rotation = 0;
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  lcd_init(Background);

  add2display((unsigned char *)"University of Nairobi",0);
  add2display((unsigned char *)" Physics Department  ",0);
  add2display((unsigned char *)" another string for  ",0);
  add2display((unsigned char *)" another string menu ",0);
  selected=0;
  while(1){
	  if(rotation>3)
		  rotation = 0;
	  if(selected>3)
		  selected=0;
	  lcd_setOrientation(rotation);
	  lcd_clear(Background);
	  display();
	  //lcd_invert(rotation);
	  lcd_fillrect(10,50,40,60, blue);
	  lcd_drawrect(10,50,40,60, red);
	  lcd_drawline(11,51,39,59, green);
	  lcd_drawcircle(30, 100,10, red);
	  lcd_fillcircle(30, 100,10, crimson);
	  WAIT1_Waitms(1000);
	  selected++;
	  rotation++;
  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.2 [05.06]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
