#ifndef Patternise_h
#define Patternise_h

#define __Patternise(mem, el, count, size_t)                                   \
  for (size_t i = 0; i < (count); i++) {                                       \
    (mem)[i] = el;                                                             \
  }
#define __Zero(mem, count, size_t) __Patternise(mem, 0, count, size_t)

#include <stddef.h>

static inline char IsZero(const char *mem, size_t count) {
  if (!mem)
    ;
  else
    for (size_t i = 0; i < count; i++) {
      if (*mem)
        return 0;
    }
  return 1;
}

#endif
