#include "K197_set.h"

short digit2index(short segment) {
   if (segment<1) segment=1; 
   if (segment>6) segment=6;
   return 7-segment; 
}
