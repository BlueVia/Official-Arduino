/*
This file is part of the GSM3 communications library for Arduino
-- Multi-transport communications platform
-- Fully asynchronous
-- Includes code for the Arduino-Telefonica GSM/GPRS Shield V1
-- Voice calls
-- SMS
-- TCP/IP connections
-- HTTP basic clients

This library has been developed by Telefónica Digital - PDI -
- Physical Internet Lab, as part as its collaboration with
Arduino and the Open Hardware Community. 

September-December 2012

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://www.tid.es
*/
#ifndef __GSM3_CIRCULARBUFFER__
#define __GSM3_CIRCULARBUFFER__


#include <inttypes.h>
#include <stddef.h>

#ifndef byte
#define byte uint8_t
#endif

// These values have to be interrelated
// To-Do: may we have just one? (BUFFERMASK)
#define __BUFFERSIZE__ 128
#define __BUFFERMASK__ 0x7F

class GSM3CircularBufferManager
{
	public:
	
		virtual void spaceAvailable();		
};

class GSM3CircularBuffer
{
	private:
		// Buffer pointers. 
		// head=tail means buffer empty
		// tail=head-1 means buffer full
		// tail=head+1 means just one char (pointed by head)
		// REMEMBER. head can be moved only by the main program
		// REMEMBER. tail can be moved only by the other thread (interrupts)
		// REMEMBER. head and tail can move only FORWARD
		volatile byte head; // First written one
		volatile byte tail; // Last written one. 
		
		GSM3CircularBufferManager* cbm;
		
		// The buffer
		volatile byte theBuffer[__BUFFERSIZE__];
		
		bool locate(const char* reference, byte thishead, byte thistail, byte* from=0, byte* to=0);
		
	public:
		GSM3CircularBuffer(GSM3CircularBufferManager* mgr=0);
		// TO-DO.Check if this formule runs too at the buffer limit
		inline byte availableBytes(){ return ((head-(tail+1))&__BUFFERMASK__);};
		inline byte storedBytes(){ return ((tail-head)&__BUFFERMASK__);};

		
		int write(char c);
		// Returns a character and MOVES the pointer
		char read();
		// Returns a character but does not move the pointer.
		char peek(int increment);
		// Returns a pointer to the head of the buffer
		inline char* firstString(){return (char*)theBuffer+head;};
		
		// Go forward one string
		char* nextString();
		void flush();
		inline byte getTail(){return tail;};
		inline byte getHead(){return head;};
		
		// Only can be executed from the interrupt!
		inline void deleteToTheEnd(byte from){tail=from;};
		
		// Checks if a substring exists in the buffer
		//move=0, dont move, =1,put head at the beginning of the string, =2, put head at the end
		bool locate(const char* reference);
		// Locates reference. If found, moves head (or tail) to the beginning (or end)
		bool chopUntil(const char* reference, bool movetotheend, bool head=true);
		int readInt(); // Reads an integer from the head. Stops with first non blank, non number character
		
		// Caveat: copies the first bytes until buffer is full
		bool extractSubstring(const char* from, const char* to, char* buffer, int bufsize);
		
		//Retrieve all the contents of buffer from head to tail
		bool retrieveBuffer(char* buffer, int bufsize, int& SizeWritten);
		
		//Debug function to print the buffer after receiving data from the modem.
		void debugBuffer();
		
		// Utility: dump character if printable, else, put in %x%
		static void printCharDebug(uint8_t c);
		
		
};


#endif