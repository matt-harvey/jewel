#ifndef GUARD_helper_macros_hpp
#define GUARD_helper_macros_hpp

/**
 * Macros for internal use within Jewel library only.
 */

#define JEWEL_MAKE_STRING(p) #p

#	ifdef __PRETTY_FUNCTION__
#		define JEWEL_FUNCTION __PRETTY_FUNCTION__
#	elif defined __func__
#		define JEWEL_FUNCTION __func__
#	else
#		define JEWEL_FUNCTION "(Function macro unavailable)"
#	endif


#endif   // GUARD_helper_macros_hpp
