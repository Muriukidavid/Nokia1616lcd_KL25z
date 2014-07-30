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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include"1616_lcd.h"
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */
enum logics{false, true};

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  lcd_init(Background);
  lcd_printstr("University of Nairobi", 2, 10, Blue);
  lcd_printstr("Physics Department", 10, 30, Red);
  lcd_printstr("David Muriuki K.", 20, 50, White);
  lcd_printstr("http://karibe.co.ke", 8, 70, Blue);
  lcd_printstr("Embedded ARM Dev't  ", 8, 90, Crimson);
  lcd_delayms(3000);
  while(1){
	  lcd_invert(true);
	  lcd_printstr("C, C++, SystemC     ", 8, 90, Yellow);
	  lcd_delayms(1000);
	  lcd_printstr("Freescale(TM), Nokia", 8, 90, Purple);
	  lcd_delayms(1000);
	  lcd_invert(false);
	  lcd_delayms(50);
	  lcd_invert(true);
	  lcd_delayms(100);
	  lcd_invert(false);
	  lcd_delayms(50);
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
