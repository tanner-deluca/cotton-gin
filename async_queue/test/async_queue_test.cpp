#include <iostream>

#include "gin_async_queue.h"
#include "gin_std_macros.h"

using namespace std;
 

int main()
{
  cout << "hi\n";
  gin_async_queue<int> q;
  
  for( int i = 0; i < 10; i++ )
  {
    q.push( i );
    cout << q.front() << "\n";
    q.pop();
  }
  
  if( q.is_empty() )
    cout << "empty\n";
  else
    cout << "not empty\n";
  
  q.push( 10 );
  if( q.is_empty() )
    cout << "empty\n";
  else
    cout << "not empty\n";
  
  return 0;
}


/* eof */