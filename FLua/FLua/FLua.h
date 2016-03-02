
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <float.h>
#define isnan _isnan
#define snprintf _snprintf
#else
#include <math.h>
#endif
