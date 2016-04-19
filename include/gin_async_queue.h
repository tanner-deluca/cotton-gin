/**
 * @file gin_async_queue.h
 * 
 * @brief a file holding a thread safe queue type
 */


#pragma once


#include <queue>
#include <mutex>


/**< the thread safe queue template class */
template <class queue_type>
class gin_async_queue
{
  /* variables */
  public:
    
  private:
    std::queue<queue_type>	async_queue;	/**< the container for the elements */
    std::mutex			queue_lock;	/**< the lock to prevent simultaneous accesses to the queue */
    
  /* functions */
  public:
    /**
     * @brief adds an element to the end of the queue
     * 
     * @param element the element to add
     */
    void push( queue_type element )
    {
      queue_lock.lock();
      async_queue.push( element );
      queue_lock.unlock();
    }
    
    
    /**
     * @brief attempts to add an element to the end of the queue
     * 
     * @param element the element to try to add
     * 
     * @return true on success, false if the queue is currently being used by another thread
     */
    bool try_push( queue_type element )
    {
      if( queue_lock.try_lock() )
      {
	async_queue.push( element );
	queue_lock.unlock();
	return true;
      }
      else
	return false;
    }
    
    
    /**
     * @brief removes the first element from the queue
     */
    void pop()
    {
      queue_lock.lock();
      async_queue.pop();
      queue_lock.unlock();
    }
    
    
    /**
     * @brief attempts to remove the first element from the queue
     * 
     * @note will fail if the queue is currently being used by another thread
     */
    void try_pop()
    {
      if( queue_lock.try_lock() )
      {
	async_queue.pop();
	queue_lock.unlock();
	return true;
      }
      else
	return false;
    }
    
    
    /**
     * @brief gets the first element in the queue
     * 
     * @return the first element in the queue
     * 
     * @note do NOT use between is_empty() and is_empty_confirmation() calls
     */
    queue_type front()
    {
      queue_type element;
      
      queue_lock.lock();
      element = async_queue.front();
      queue_lock.unlock();
    
      return element;
    }
    
    
    /**
     * @brief gets the first element in the queue without waiting for the queue_lock
     * 
     * @return the first element in the queue
     * 
     * @note potentially dangerous if the first element is being pushed or popped at the same time as this call
     * @note not dangerous between is_empty() and is_empty_confirmation() calls
     */
    queue_type quick_front()
    {
      return async_queue.front();
    }
    
    
    /**
     * @brief gets the number of elements in the queue
     * 
     * @return the number of elements in the queue
     * 
     * @note do NOT use between is_empty() and is_empty_confirmation() calls
     */
    int size()
    {
      int length;
      
      queue_lock.lock();
      length = async_queue.size();
      queue_lock.unlock();
      
      return length;
    }
    
    
    /**
     * @brief gets the number of elements in the queue
     * 
     * @return the number of elements in the queue
     * 
     * @note potentially inaccurate
     * @note guaranteed accuracy between is_empty() and is_empty_confirmation() calls
     */
    int quick_size()
    {
      return async_queue.size();
    }
    
    
    /**
     * @brief checks if there is anything in the queue
     *
     * @return true if there isn't, false if there is
     * 
     * @note doesn't unlock the queue_lock to prevent the senario where this function returns true as the last element of the queue is popped, which would cause a crash
     * @note need to call is_empty_confirmation() to use the queue again
     * @note do NOT use between is_empty() and is_empty_confirmation() calls
     */
    bool is_empty()
    {
      bool empty;
      
      queue_lock.lock();
      empty = async_queue.empty();
      
      return empty;
    }
    
    /**
     * @brief reenables the use of the queue after is_empty() is called
     * 
     * @note do NOT call this before calling is_empty()
     */ 
    void is_empty_confirmation()
    {
      queue_lock.unlock();
    }
    
  private:
};


/* eof */