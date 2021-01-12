/*
©  2018 Microchip Technology Inc. and its subsidiaries.
 
Subject to your compliance with these terms, you may use this Microchip software and any derivatives of this software. You must retain the above copyright notice with any redistribution of this software and the following disclaimers. It is your responsibility to comply with third party license terms applicable to your use of third party software (including open source software) that may accompany this Microchip software.
  
THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  MICROCHIP WILL NOT SUPPORT THIS SOFTWARE IN ANY MANNER. NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE. 
 
IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THIS SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

 *	Assertion - use liberally for debugging. Defining NDEBUG
 *	turns assertions off.
 *	assert(exp) where exp is non-zero does nothing, while
 *	assert(exp) where exp evaluates to zero aborts the program
 *	with a message like
 *
 *	Assertion failed: prog.c line 123: "exp"
 *
 */

#ifndef	NDEBUG
extern void	_fassert(int, char *, char *);
#define	assert(exp)	if(!(exp)) {_fassert(__LINE__, __FILE__, "exp");}
#else
#define	assert(exp)
#endif
