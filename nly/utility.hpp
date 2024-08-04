#ifndef NLY_UTILITY
#define NLY_UTILITY

#include "boost/noncopyable.hpp"

namespace nly
{
typedef boost::noncopyable noncopyable;

#define NLY_RANGE(container) std::begin(container), std::end(container)
} // namespace nly

#endif