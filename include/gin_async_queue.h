/**
 * @file gin_async_queue.h
 * 
 * @brief a file holding a thread safe queue type
 */


#pragma once


#include <queue>
#include <mutex>


/**
 * The gin_async_queue class is a asynchronous ( thread-safe ) queue type.
 */
template< class queue_type > class gin_async_queue
{
  /* variables */
public:
  std::queue< queue_type* >	async_queue;	/**< the queue for holding objects */
  std::mutex			queue_lock;	/**< the queue lock */
  
private:
  
  
  /* functions */
public:
  /**
   * @brief the destructor for the queue
   */
  ~gin_async_queue()
  {
    queue_lock.lock();
    clear_locked();
  }
  
  
  /**
   * @brief adds an element to the queue
   * 
   * @param element a pointer to the element to add
   * 
   * @note should NOT be called while holding the queue lock
   */
  void push( queue_type *element )
  {
    queue_lock.lock();
    async_queue.push( element );
    queue_lock.unlock();
  }
  
  
  /**
   * @brief adds an element to the queue
   * 
   * @param element a pointer to the element to add
   * 
   * @note may cause undefined behaviour if called while NOT holding the queue lock
   */
  void push_locked( queue_type *element )
  {
    async_queue.push( element );
  }
  
  
  /**
   * @brief removes and returns an element from the queue
   * 
   * @return the first element in the queue
   * 
   * @note ALWAYS checks if the return exists to prevent crashes
   * @note should NOT be called while holding the queue lock
   */
  queue_type* pop()
  {
    queue_lock.lock();
    
    queue_type *return_value;
    
    if( async_queue.empty() )
      return_value = nullptr;
    else
    {
      return_value = async_queue.front();
      async_queue.pop();
    }
    
    queue_lock.unlock();
    return return_value;
  }
  
  
  /**
   * @brief removes and returns an element from the queue
   * 
   * @return the first element in the queue
   * 
   * @note may cause undefined behaviour if called while NOT holding the queue lock
   */
  queue_type* pop_locked()
  {
    if( async_queue.empty() )
      return nullptr;
    else
    {
      queue_type *return_value = async_queue.front();
      async_queue.pop();
      return return_value;
    }
  }
  
  
  /**
   * @brief returns the number of elements in the queue
   * 
   * @note should NOT be called while holding the queue lock
   * @note the size may never be truly be 100% accurate
   */
  int size()
  {
    queue_lock.lock();
    int queue_size = async_queue.size();
    queue_lock.unlock();
    return queue_size;
  }
  
  
  /**
   * @brief returns the number of elements in the queue
   * 
   * @note the size will only be 100% accurate while holding the queue lock
   */
  int size_locked()
  {
    return async_queue.size();
  }
  
  
  /**
   * @brief checks if the queue is empty or not
   * 
   * @return true if the queue is empty, false if else
   * 
   * @note should NOT be called while holding the queue lock
   * @note it is theoretically possible for this to provide a false answer, therefore you should always check popped data to see if it exists
   */
  bool is_empty()
  {
    queue_lock.lock();
    bool emp = async_queue.empty();
    queue_lock.unlock();
    return emp;
  }
  
  
  /**
   * @brief checks if the queue is empty or not
   * 
   * @return true if the queue is empty, false if else
   * 
   * @note may cause undefined behaviour if called while NOT holding the queue lock
   */
  bool is_empty_locked()
  {
    return async_queue.empty();
  }
  
  
  /**
   * @brief clears all the elements from the queue
   * 
   * @note will delete all elements
   * @note should NOT be called while holding the queue lock
   */
  void clear()
  {
    queue_lock.lock();
    
    queue_type *temp;
    
    while( !async_queue.empty() )
    {
      temp = pop_locked();
      delete( temp );
    }
    
    queue_lock.unlock();
  }
  
  
  /**
   * @brief clears all the elements from the queue
   * 
   * @note will delete all elements
   * @note may cause undefined behaviour when called while NOT holding the queue lock
   */
  void clear_locked()
  {
    queue_type *temp;
    
    while( !async_queue.empty() )
    {
      temp = pop_locked();
      delete( temp );
    }
  }
  
  
  /**
   * @brief locks the queue lock
   * 
   * @note only *_locked() functions should be called while holding the queue lock to prevent deadlock
   */
  void lock()
  {
    queue_lock.lock();
  }
  
  
  /**
   * @brief unlocks the queue lock
   * 
   * @note may cause undefined behaviour if called while not holding the queue lock
   */
  void unlock()
  {
    queue_lock.unlock();
  }
  
  
private:
};


/* eof */