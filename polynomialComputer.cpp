/*-------------------------------------------------------------------------*
 *---									---*
 *---		polynomialComputer.cpp					---*
 *---									---*
 *---	    This file defines a program that computes:			---*
 *---									---*
 *---			f(x) = 3*x^2 + 2*x + 1				---*
 *---									---*
 *---	in an inefficient, 4-child thread fashion.  It is only good at	---*
 *---	harassing Computer Systems II students to see if they 		---*
 *---	understand threading well enough to complete it and get it	---*
 *---	working.							---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2021 May 27		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/


#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<pthread.h>

const	int	BUFFER_LEN	= 4096;

const	int	NUM_TERMS	= 3;

const	int	NUM_THREADS	= NUM_TERMS + 1;

#define		END_OF_INPUT_MARKER	"end"

float		x		= 0.0;

float		fOfX		= 0.0;

pthread_cond_t	cond[NUM_THREADS];

pthread_mutex_t	xAndFOfXLock;

bool		shouldRun	= true;

typedef	enum	{
		  READ_TURN,
		  CONSTANT_TURN,
		  LINEAR_TURN,
		  SQUARED_TURN
		}
		turn_ty;

turn_ty		turn		= READ_TURN;

void*		squaredTerm	(void*		vPtr
				)
{
  while  (true)
  {

    pthread_mutex_lock(&xAndFOfXLock);
    while(turn!= SQUARED_TURN && shouldRun){
      pthread_cond_wait(&cond[3], &xAndFOfXLock);

    }
    //  Begin critial section
    if  (!shouldRun)
    {
      fprintf(stderr,"Stopping squaredTerm()\n");
   
      pthread_cond_signal(&cond[0]);
      pthread_mutex_unlock(&xAndFOfXLock);

      break;
      
    }

    float	thisTerm	= 3*x*x;
    float	sum		= fOfX + thisTerm;

    fprintf(stderr,"3*%g^2 + 2*%g + 1 = %g\n",x,x,sum);
    fOfX			= sum;
    turn			= READ_TURN;
    printf("%g %g\n",x,fOfX);
    //  End critial section


    pthread_cond_signal(&cond[0]);
    pthread_mutex_unlock(&xAndFOfXLock);

  }
}


void*		linearTerm	(void*		vPtr
				)
{
  while  (true)
  {

    pthread_mutex_lock(&xAndFOfXLock);
    while(turn != LINEAR_TURN && shouldRun){
      pthread_cond_wait(&cond[2], &xAndFOfXLock);

    }

    //  Begin critial section
    if  (!shouldRun)
    {
      fprintf(stderr,"Stopping linearTerm()\n");

      pthread_cond_signal(&cond[3]);
      pthread_mutex_unlock(&xAndFOfXLock);

      break;
    }

    float	thisTerm	= 2*x;
    float	sum		= fOfX + thisTerm;

    fprintf(stderr,"2*%g + 1 = %g\n",x,sum);
    fOfX			= sum;
    turn			= SQUARED_TURN;
    //  End critial section


    pthread_cond_signal(&cond[3]);
    pthread_mutex_unlock(&xAndFOfXLock);

  }
}


void*		constantTerm	(void*		vPtr
				)
{
  while  (true)
  {
 
    pthread_mutex_lock(&xAndFOfXLock);
    while(turn != CONSTANT_TURN && shouldRun){
      pthread_cond_wait(&cond[1], &xAndFOfXLock);

    }

    //  Begin critial section
    if  (!shouldRun)
    {
      fprintf(stderr,"Stopping constantTerm()\n");

      pthread_cond_signal(&cond[2]);
      pthread_mutex_unlock(&xAndFOfXLock);

      break;
    }

    float	thisTerm	= 1;
    float	sum		= fOfX + thisTerm;

    fprintf(stderr,"1 = %g\n",sum);
    fOfX			= sum;
    turn			= LINEAR_TURN;
    //  End critial section


    pthread_cond_signal(&cond[2]);
    pthread_mutex_unlock(&xAndFOfXLock);

  }
}


void*		readX		(void*		vPtr
				)
{
  char	buffer[BUFFER_LEN];
  char*	cPtr;

  while  (true)
  {

    pthread_mutex_lock(&xAndFOfXLock);
    while(turn != READ_TURN && shouldRun){
      pthread_cond_wait(&cond[0], &xAndFOfXLock);
    }


    //  Begin critial section
    fgets(buffer,BUFFER_LEN,stdin);

    if  ( (cPtr = strchr(buffer,'\n')) != NULL )	*cPtr = '\0';

    if  (strcmp(buffer,END_OF_INPUT_MARKER) == 0)
    {
      fprintf(stderr,"Stopping readX()\n");
      shouldRun	= false;

      pthread_cond_signal(&cond[1]);
      pthread_mutex_unlock(&xAndFOfXLock);

      break;
    }

    x		= atof(buffer);
    fOfX	= 0.0;
    fprintf(stderr,"x = %g\n",x);
    turn	= CONSTANT_TURN;
    //  End critial section


    pthread_cond_signal(&cond[1]);
    pthread_mutex_unlock(&xAndFOfXLock);

  }
}


int		main		()
{
  int		i;
  pthread_t	threadArray[NUM_THREADS];

  pthread_mutex_init(&xAndFOfXLock,NULL);

  for  (i = 0;  i < NUM_THREADS;  i++)
  {
    pthread_cond_init(&cond[i],NULL);
  }

  pthread_create(&threadArray[3],NULL,squaredTerm,NULL);
  pthread_create(&threadArray[2],NULL,linearTerm,NULL);
  pthread_create(&threadArray[1],NULL,constantTerm,NULL);
  pthread_create(&threadArray[0],NULL,readX,NULL);

  for  (i = 0;  i < NUM_THREADS;  i++)
  {
    pthread_join(threadArray[i],NULL);
  }

  for  (i = 0;  i < NUM_THREADS;  i++)
  {
    pthread_cond_destroy(&cond[i]);
  }

  pthread_mutex_destroy(&xAndFOfXLock);
  
  return(EXIT_SUCCESS);
}

