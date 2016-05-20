#include <thread>
#include <iostream>
#include <bitset>
#include <chrono>
#include "gin_logger.h"


using namespace std;
  
void thread1();
void thread2();

gin_logger *test_logger;


int main()
{
  test_logger = new gin_logger( "test_file_1.txt", 10 );
  
//   test_logger->enable_cmd_line();
  test_logger->start();
  
  thread t1( thread1 );
  thread t2( thread2 );
  
  t1.join();
  t2.join();
  
  delete( test_logger );
  
  return 0;
}


void thread1()
{
  string msg;
  
  for( int i = 1000000; i > 0; i-- )
  {
    msg = "t1::" + to_string( i );
    test_logger->log( __FILE__, __LINE__, "TEST", msg);
  }
}


void thread2()
{
  string msg;
  
  for( int i = 1000000; i > 0; i-- )
  {
    msg = "t2::" + to_string( i );
    test_logger->log( __FILE__, __LINE__, "TEST", msg);
  }
}