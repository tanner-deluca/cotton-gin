/**
 * @file gin_queue.h
 * 
 * @brief a file holding a queue type
 */


#pragma once


#include <vector>


template< class type >
class gin_queue
{
  /* variables */
  public:
    ~gin_queue();
    
  private:
    std::vector< type > queue_vector;
  
  /* functions */
  public:
    void push( type element );
    type pop();
    int size();
    void clear();
  private:
};


gin_queue::~gin_queue()
{
  clear();
}


template< class type >
void gin_queue::push( type element )
{
  queue_vector.push_back( element );
}


template< class type >
type gin_queue::pop()
{
  type return_value = queue_vector.front();
  queue_vector.erase( queue_vector.
}


template< class type >
void gin_queue::push( type element )
{
}


template< class type >
void gin_queue::push( type element )
{
}
/* eof */