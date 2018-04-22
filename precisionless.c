#include "precisionless.h"

// initialize precision less variable `pl` with 0
void plinit (precless_t *const pl)
{
  pl->_dh = pl->_dl = 0.0;
}
// convert precision less structure to double
double plread (precless_t const *const pl)
{
  return pl->_dh;
}
// initialize precision less variable `pl` with `x`
void plassign (precless_t *const pl, double x)
{
  pl->_dh = x;
  pl->_dl = 0.0;
}
// add `x` to current value of precision less variable `pl`
void pladd (precless_t *const pl, double x)
{
  double diff, dh;
  diff   = x - pl->_dl;
  dh     = pl->_dh + diff;
  pl->_dl = (dh - pl->_dh) - diff;
  pl->_dh = dh;
}
