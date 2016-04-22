/**
 * @file gin_async_queue.h
 * 
 * @brief a file holding a thread safe queue type
 */


#pragma once


#include <queue>
#include <mutex>

#include "gin_std_types.h"


/**
 * @brief the thread safe queue class 
 * 
 * @note one of the clear functions should be called before deleting
 * 
 */
class gin_async_queue
{
  /* variables */
  public:
    
  private:
    std::queue<data_ptr>	async_queue;	/**< the container for the elements */
    std::mutex			queue_lock;	/**< the lock to prevent simultaneous accesses to the queue */

 
  /* functions */
  public:
    /**
     * @brief adds data onto the queue
     * 
     * @param element the data to add ( CANNOT be NULL )
     */
    void push( data_ptr element );
    
    /**
     * @brief adds data onto the queue
     * 
     * @param element the data to add ( CANNOT be NULL )
     * 
     * @note this function should only be called between lock() and unlock() calls
     */
    void push_locked( data_ptr element );
    
    /**
     * @brief removes the oldest element from the queue and returns it
     * 
     * @return the oldest element in the queue or NULL if the queue is empty
     */
    data_ptr pop();
    
    /**
     * @brief removes the oldest element from the queue and returns it
     * 
     * @return the oldest element in the queue or NULL if the queue is empty
     * 
     * @note this function should only be called between lock() and unlock() calls
     */
    data_ptr pop_locked();
    
    /**
     * @brief returns the number of elements in the queue
     * 
     * @return the number of elements that will potentially be in the queue
     * 
     * @note actually returns the size + pending_pushes - pending_pops
     */
    int size();
    
    /**
     * @brief returns the number of elements in the queue
     * 
     * @return the number of elements currently in the queue
     * 
     * @note this function should only be called between lock() and unlock() calls
     */
    int size_locked();
    
    /**
     * @brief locks the queue
     * 
     * @note while locked, you should only call the *_locked() function to prevent deadlock
     */
    void lock();
    
    /**
     * @brief unlocks the queue
     * 
     * @note calling this function without having 1st called lock() may lead to undefined behavior
     */
    void unlock();
    
    /**
     * @brief removes and deletes all elements of a special type in the queue
     *
     * @param destroy a pointer to a function to free the elements in the queue ( CANNOT be NULL )
     */
    void clear_special( free_function destroy );
    
    /**
     * @brief removes and deletes all elements in the queue
     * 
     * @note use clear_special for elements that require a non-destructor free function
     */
    template< class type >
    void clear( type t );

  private:
};


template< class type >
void gin_async_queue::clear( type t)
{
  queue_lock.lock();
  
  while( !async_queue.empty() )
  {
    delete( ( type )( pop_locked() ) );
  }
  
  queue_lock.unlock();
}


/* eof */