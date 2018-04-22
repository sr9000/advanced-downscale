#ifndef PRECISIONLESS_H
#define PRECISIONLESS_H

typedef struct
{
  double _dh, _dl;
} precless_t;

// initialize precision less variable `pl` with 0
void    plinit   (precless_t *const pl);
// convert precision less structure to double
double  plread   (precless_t const *const pl);
// initialize precision less variable `pl` with `x`
void    plassign (precless_t *const pl, double x);
// add `x` to current value of precision less variable `pl`
void    pladd    (precless_t *const pl, double x);

#endif
