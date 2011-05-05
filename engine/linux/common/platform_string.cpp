#include "stdafx.h"

#include <strings.h>

#include "common/istring.h"

//==============================================================================

namespace engine
{
	//============================================================================

  int String::CompareNoCase(const char* str1, const char* str2)
  {
    return strcasecmp(str1, str2);
  }

	//============================================================================
} // End [namespace engine]

//==============================================================================
// [EOF]
