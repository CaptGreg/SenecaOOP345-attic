// https://web.archive.org/web/20100717072236/http://public.carnet.hr/~jbrecak/sm.html
// This is 32 bit code. Compile with -m32 flag.
// gcc -Wall -m32 -mmmx -msse -fmax-errors=1 -Wfatal-errors sm.c -o sm && ./sm
// (runs on gcc 5.4.1 under Ubuntu 16.04)

// see http://asm.sourceforge.net/articles/smc.html

/*                         sm.c - self modifying C code
                             -------------------
    begin                : Wed Nov 23 21:05:29 CET 2004
    copyright            : (C) 2004 by Juraj Brecak
    e-mail               : jbrecak (AT) public (DOT) carnet (DOT) hr
	
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
 
/*	this is an example of self modifying code in C.

	it works for me, on a gnu/linux & gcc 2.95.4
	i have no idea if this is portable to other systems/compilers.
	there are no big reasons why it shouldn't be. if you try it out,
	please let me know how it went.
	
	the code was inspired by Karsten Scheibler's article
	"Using self modifying code under Linux" at linuxassembly.org
	and a guy from the web (whose name i had forgotten) asking around
	whether	it was possible to write SMC in C. i was as courious as he was
	and this was one way to check it out. as you can see it (almost) is.
	i had to cheat just a bit and label my code from asm. ok,
	for the impatient: make; ./smc; echo $?

	if you have any questions feel free to contact me.

	happy coding.. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

/*	gcc is oblivious to asm level so we have to spell out
	our intentions.. it's an ugly hack. if you know of
	a proper way to do it in pure C please tell me. */
	
extern char injectHere;
extern char injectionStart;
extern char injectionEnd;

void dummyTextContext( void )
{
/*	sometimes gcc is just too smart.. in an ideal world
	we would write smthng like (*(&exit))( 42 ); and get
	an indirect call (via absolute address). anyway,
	we don't get that so we have to make one pointer for
	every type of function we want to call */

	int (*callPrintf)( const char *format, ... ); 
	void (*callExit)( int ); /* man 3 printf exit */
	
	asm volatile( "injectionStart:" );
	
/*	do what you like here but keep it relocatable
	fire up objdump, gdb or whatever to check what you get */

	(*(callPrintf=&printf))( "Hello :-)\n" );
	(*(callPrintf=&printf))( "No endless loop here!\n" );

	(*(callExit=&exit))( 42 ); /* the meaning of life (for this proggie anyway..) */

	asm volatile( "injectionEnd:" );
}

int main( void )
{
/*	it's fairly safe to assume page is 2^N,
	now.. let's find us a page to bash >:*/
	
	unsigned page = (unsigned)&injectHere & ~( getpagesize() - 1 );

/*	chmod u=rwx page ;-) */

	if( mprotect( (char*)page, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC ) )
	{
		perror( "mprotect failed" );
		exit( errno );
	}

endlessLoop:

	asm volatile( "injectHere:" );	/* ground zero */

/*	a few stats to ease debugging (and some code to fill our loop too) */

	printf( "PAGE(0x%x): cp <0x%x>[0..%u] <0x%x>\n",
		page, (int)&injectionStart, &injectionEnd - &injectionStart, (int)&injectHere );

/*	inject some */

	memcpy( &injectHere, &injectionStart, &injectionEnd - &injectionStart );

goto endlessLoop;

	return 0;
}

/*	we can inject into the loop (ie. stomp over current code) but
	then we have to make sure there's enough padding (or real code)
	so we don't trample the jmp (goto endlessLoop;). if we do that,
	the injection never gets executed because when memcpy returns
	the execution will continue in the middle of our injected code
	and that is *bad*. sooo.. we can either inject into the loop
	and make sure the whole	new code fits inside..
	or start injecting over the "goto". that way injected code will
	get executed as soon as memcpy returns. also note, that that
	would not literally be self modifying code as we would be
	injecting over a code that has not yet run. now *thats*
	pointless (unless you are intentionally obscuring things).
	OTH we could completely change the insides of the loop
	and neatly pad potential leftover space with nops thus
	keeping the loop runing. */

/* EOF */
