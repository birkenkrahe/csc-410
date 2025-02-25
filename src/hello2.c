/*****************************************************************/    
// hello2.c: print "Hello, World!" to stdout use a void function
//           hello() to print Input: NONE Output: Hello, World!
//           (string) Author: Marcus Birkenkrahe Date: 02/24/2025
/*****************************************************************/    

// include libraries
#include <stdio.h>

// prototype functions

// hello: print hello world
// returns: nothing (void)
// params: none (void)
void hello(void); 

// main: main program
// returns: integer (int)
// params: none (void)
int main(void)  
{
  hello(); // function call
  return 0;
}

// function definition
void hello(void)
{
  puts("Hello, world!");  
}
