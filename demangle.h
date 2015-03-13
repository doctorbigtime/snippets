#pragma once
#include <string>
#include <cxxabi.h>
inline std::string demangle(const std::type_info &theType)
{
   int status;
   char* name = abi::__cxa_demangle(theType.name(), 0, 0, &status);
   if( status == 0 )
   {
      std::string result(name);
      ::free(name);
      return result;
   }
   else
      return theType.name();
}

