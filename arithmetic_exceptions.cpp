#include "arithmetic_exceptions.hpp"

#include <string>

namespace jewel
{

// UnsafeArithmeticException

UnsafeArithmeticException::UnsafeArithmeticException(std::string p_message):
  m_message(p_message)
{
}

UnsafeArithmeticException::~UnsafeArithmeticException() throw()
{
}

const char*
UnsafeArithmeticException::what() const throw()
{
	return m_message.c_str();
}


}  // namespace jewel



