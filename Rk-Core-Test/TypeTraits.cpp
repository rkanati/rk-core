//
// Copyright (C) 2013 Rk
// Permission to use, copy, modify and distribute this file for any purpose is hereby granted without fee or other
// limitation, provided that
//  - this notice is reproduced verbatim in all copies and modifications of this file, and
//  - neither the names nor trademarks of the copyright holder are used to endorse any work which includes or is derived
//    from this file.
// No warranty applies to the use, either of this software, or any modification of it, and Rk disclaims all liabilities
// in relation to such use.
//

#include <Rk/TypeTraits.hpp>
#include <iterator>
#include <vector>

namespace RkTest
{
  class ContainerBad1
  {
  public:
    
  };

  class ContainerBad2
  {
  public:
    void begin ();
    void end   ();

  };

  class ContainerBad3
  {
  public:
    char* begin ();
    int*  end   ();

  };

  class ContainerBad4
  {
  public:
    typedef int& iterator;

    int& begin ();
    int& end   ();

  };

  class ContainerBad5
  {
  public:
    struct iterator
    { };

    iterator begin ();
    iterator end   ();

  };
  
  class ContainerOk1
  {
  public:
    typedef char* iterator;

    char* begin ();
    char* end   ();

  };

  class ContainerOk2
  {
  public:
    typedef char* iterator;

  };

}

namespace std
{
  template <>
  char* begin (RkTest::ContainerOk2&)
  {
    return 0;
  }

  template <>
  char* end (RkTest::ContainerOk2&)
  {
    return 0;
  }

}

namespace RkTest
{
  class ContainerOk3
  {
  public:
    typedef char* iterator;

  };

  char* begin (ContainerOk3&) { return 0; }
  char* end   (ContainerOk3&) { return 0; }

  extern "C" int main () try
  {
    static_assert (!Rk::IsIterable <int>::value,      "IsIterable <int> fail");
    static_assert ( Rk::IsIterable <int [20]>::value, "IsIterable <int [20]> fail");

    static_assert (!Rk::IsIterable <ContainerBad1>::value, "IsIterable <ContainerBad1> fail");
    static_assert (!Rk::IsIterable <ContainerBad2>::value, "IsIterable <ContainerBad2> fail");
    static_assert (!Rk::IsIterable <ContainerBad3>::value, "IsIterable <ContainerBad3> fail");
    static_assert (!Rk::IsIterable <ContainerBad4>::value, "IsIterable <ContainerBad4> fail");
    static_assert (!Rk::IsIterable <ContainerBad5>::value, "IsIterable <ContainerBad5> fail");

    static_assert ( Rk::IsIterable <ContainerOk1>::value, "IsIterable <ContainerOk1> fail");
    static_assert ( Rk::IsIterable <ContainerOk2>::value, "IsIterable <ContainerOk2> fail");
    static_assert ( Rk::IsIterable <ContainerOk3>::value, "IsIterable <ContainerOk3> fail");

    static_assert ( Rk::IsIterable <std::vector <int>>::value, "IsIterable <std::vector <int>> fail");

    return 0;
  }
  catch (...)
  {
    return 1;
  }

}
