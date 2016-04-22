#include "gin_async_queue.h"
#include "gin_std_macros.h"


using namespace std;


void gin_async_queue::push( data_ptr element )
{
  return_if_fail( element );
  
  queue_lock.lock();
  async_queue.push( element );
  queue_lock.unlock();
}


void gin_async_queue::push_locked( data_ptr element )
{
  return_if_fail( element );
  
  async_queue.push( element );
}


data_ptr gin_async_queue::pop()
{
  data_ptr return_value;
  
  queue_lock.lock();
  
  if( async_queue.empty() )
    return_value = NULL;
  else
  {
    return_value = async_queue.front();
    async_queue.pop();
  }
  
  queue_lock.unlock();
  return return_value;
}


data_ptr gin_async_queue::pop_locked()
{
  data_ptr return_value;
  
  if( async_queue.empty() )
    return_value = NULL;
  else
  {
    return_value = async_queue.front();
    async_queue.pop();
  }
  
  return return_value;
}


int gin_async_queue::size()
{
  queue_lock.lock();
  int return_value = async_queue.size();
  queue_lock.unlock();
  
  return return_value;
}


int gin_async_queue::size_locked()
{
  return async_queue.size();
}


void gin_async_queue::lock()
{
  queue_lock.lock();
}


void gin_async_queue::unlock()
{
  queue_lock.unlock();
}


void gin_async_queue::clear_special( free_function destroy )
{
  queue_lock.lock();
  
  return_if_fail( destroy );
  
  while( !async_queue.empty() )
  {
    destroy( pop_locked() );
  }
  
  queue_lock.unlock();
}


/* eof */