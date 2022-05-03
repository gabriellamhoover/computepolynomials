/*-------------------------------------------------------------------------*
 *---									---*
 *---		FloatStack.cpp						---*
 *---									---*
 *---	    This file defines methods for a class that implements a	---*
 *---	stack ADT of 'float' instances using individually 'new'ed	---*
 *---	FloatNode instances.						---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"PolynomialServerHeaders.h"

FloatStack::~FloatStack	()
{
  FloatNode*	next;
  while( !isEmpty()){
    pop();
  }

}


void		FloatStack::push(float	number
				)
{
  FloatNode*	next	= new FloatNode(number);
  if(topPtr_ != NULL){
    next->nextPtr_ = topPtr_;
  }
  topPtr_ = next;

}


float		FloatStack::pop	()
{
  if  (isEmpty())
  {
    fprintf(stderr,"Attempt to pop() empty stack!\n");
    exit(EXIT_FAILURE);
  }

  float		toReturn	= topPtr_->number_;

  FloatNode* temp = topPtr_;
  topPtr_ = topPtr_->nextPtr_;
  temp->nextPtr_ = NULL;
  delete(temp);
  return(toReturn);
}
