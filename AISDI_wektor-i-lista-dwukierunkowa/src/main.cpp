#include <cstddef>
#include <cstdlib>
#include <string>

//#include <ctime>
//#include <iostream>

#include "Vector.h"
#include "LinkedList.h"

//clock_t start, stop;
//double time_ss;

namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;
//using LinearCollection = aisdi::Vector<T>;

void perfomTest()
{
    LinearCollection<std::string> collection;
    collection.append("TODO");
/*
   LinearCollection <int> collection({3,8,1,1,4,10,7,2,0,5,7,3,2});
   LinearCollection <int> other({2,3,7,9,1,2});
   collection = other;
 //  collection.append(11);
 //  collection.prepend(33);
 //  collection.erase(collection.begin());
 //  collection.erase(collection.begin(), collection.end());
 //  collection.popLast();
 //  collection.popFirst();
*/
}

//}

/*
template <typename T>
using LinkedList = aisdi::LinkedList<T>;
void perfomTestList(std::size_t n)
{
  //LinearCollection<std::string> collection;
  //collection.append("TODO");
  LinkedList<int> collection;

  start = clock();
  for(std::size_t i=0; i<n; ++i)
    collection.append(i);
  stop = clock();
  time_ss = (double)(stop - start)/CLOCKS_PER_SEC;
  std::cout<<"LinkedList:   Append time:                   "<<time_ss<<"\n";

  start = clock();
  for(std::size_t i=0; i<n; ++i)
    collection.prepend(i);
  stop = clock();
  time_ss = (double)(stop - start)/CLOCKS_PER_SEC;
  std::cout<<"LinkedList:   Prepend time:                  "<<time_ss<<"\n";

  start = clock();
  for(std::size_t i=0; i<n; ++i)
    collection.erase(collection.begin());
  stop = clock();
  time_ss = (double)(stop - start)/CLOCKS_PER_SEC;
  std::cout<<"LinkedList:   EraseBegin time:               "<<time_ss<<"\n";

  start = clock();
  for(std::size_t i=0; i<n; ++i)
    collection.erase(collection.end() - 1);
  stop = clock();
  time_ss = (double)(stop - start)/CLOCKS_PER_SEC;
  std::cout<<"LinkedList:   EraseEnd time:                 "<<time_ss<<"\n";

}

template <typename T>
using Vector = aisdi::Vector<T>;
void perfomTestVector(std::size_t n)
{
  //LinearCollection<std::string> collection;
  //collection.append("TODO");

  Vector<int> collection;

  start = clock();
  for(std::size_t i=0; i<n; ++i)
    collection.append(i);
  stop = clock();
  time_ss = (double)(stop - start)/CLOCKS_PER_SEC;
  std::cout<<"Vector:       Append time:                   "<<time_ss<<"\n";

  start = clock();
  for(std::size_t i=0; i<n; ++i)
    collection.prepend(i);
  stop = clock();
  time_ss = (double)(stop - start)/CLOCKS_PER_SEC;
  std::cout<<"Vector:       Prepend time:                  "<<time_ss<<"\n";

  start = clock();
  for(std::size_t i=0; i<n; ++i)
    collection.erase(collection.begin());
  stop = clock();
  time_ss = (double)(stop - start)/CLOCKS_PER_SEC;
  std::cout<<"Vector:       EraseBegin time:               "<<time_ss<<"\n";

  start = clock();
  for(std::size_t i=0; i<n; ++i)
    collection.erase(collection.end() - 1);
  stop = clock();
  time_ss = (double)(stop - start)/CLOCKS_PER_SEC;
  std::cout<<"Vector:       EraseEnd time:                 "<<time_ss<<"\n";

}
*/
} // namespace



int main(int argc, char** argv)
{

  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();
 //   perfomTestList(repeatCount);
 //   perfomTestVector(repeatCount);




  return 0;
}
