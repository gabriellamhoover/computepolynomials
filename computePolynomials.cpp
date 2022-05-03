
/*-------------------------------------------------------------------------*
 *---									---*
 *---		computePolynomial.cpp					---*
 *---									---*
 *---	    This file defines a function that creates a child process,	---*
 *---	sends the contents of '*inputStackPtr' to the child process  	---*
 *---	via a pipe, reads the results that the child gives back using	---*
 *---	another pipe, and put those results into '*outputStackPtr'.	---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"PolynomialServerHeaders.h"
#include	<wait.h>

void		computePolynomials
				(FloatStack*	outputStackPtr,
				 FloatStack*	inputStackPtr
				)
{
  int		toChild[2];
  int		fromChild[2];
  pid_t		polynomialPid;
  char		buffer[BUFFER_LEN];
  float		fOfX;
  float		x;
  float num;
  pipe(toChild);
  pipe(fromChild);
  polynomialPid = fork();
 
  if  (polynomialPid == 0)
  {
   
    close(toChild[1]);
    close(fromChild[0]);
    dup2(toChild[0], STDIN_FILENO);
    dup2(fromChild[1], STDOUT_FILENO);
    execl(PROGRAM_NAME, PROGRAM_NAME, NULL);
    exit(EXIT_FAILURE);
  }
  if(polynomialPid > 0){
 
    close(fromChild[1]);
    close(toChild[0]);
  while  ( !inputStackPtr->isEmpty() )
  {
   
    num  =  inputStackPtr->pop();
    int len = snprintf(buffer, BUFFER_LEN, "%g\n", num);
    write(toChild[1], buffer, len);
    
  }
  write(toChild[1],END_OF_INPUT_MARKER, sizeof(END_OF_INPUT_MARKER));
 
  FILE*	inputPtr	= fdopen(fromChild[0],"r");
  while  ( fgets(buffer,BUFFER_LEN,inputPtr) != NULL )
  {
    sscanf(buffer, "%g %g", &x, &fOfX);
    outputStackPtr->push(fOfX);
    outputStackPtr->push(x);
    

    
  }
 
  fclose(inputPtr);
  wait(NULL);
  

  }
}
