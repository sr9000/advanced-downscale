#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nd.h"
#include "downscale.h"
#include "precisionless.h"

static void libinit(void) __attribute__((constructor));

#define QUANTLEVELS 256

static void libinit(void)
{
  // initialize first half of `nd` array
  assign_nd();
  // initialize `nd` array
  for (size_t i = 2; i < IMAX; ++i)
    for (size_t k = 0; k < (i + 1) / 2; ++k)
      nd[i][i - 1 - k] = nd[i][k];
}

void downscale(
  // mutable params, only pointer to them is 'const'
    double  **const dmatr
  , size_t   *const dheight
  , size_t   *const dwidth
  // const params
  , double const *const smatr
  , size_t const sheight
  , size_t const swidth
  , size_t const factor
  , size_t const smoothness
  , double const boldness)
{
  calculate_downscaled_resolution(dheight, dwidth, sheight, swidth, factor);
  (*dmatr) = (double*) malloc((*dwidth) * (*dheight) * sizeof(double));
  if ((*dmatr) == NULL) return; // exit if no memory allocated
  padownscale((*dmatr), smatr, sheight, swidth, factor, smoothness, boldness);
}

void calculate_downscaled_resolution(
  // mutable params, only pointer to them is 'const'
    size_t *const dheight
  , size_t *const dwidth
  // const params
  , size_t const sheight
  , size_t const swidth
  , size_t const factor)
{
  (*dheight) = sheight / factor + (sheight % factor ? 1 : 0);
  (*dwidth)  = swidth  / factor + (swidth  % factor ? 1 : 0);
}

inline size_t limit(long int index, size_t size);
inline size_t quantize(
    double const *const matr
  , size_t const h
  , size_t const w
  , size_t const ih
  , size_t const iw);

void padownscale(
  // mutable params, only pointer to them is 'const'
    double *const dmatr
  // const params
  , double const *const smatr
  , size_t const sheight
  , size_t const swidth
  , size_t const factor
  , size_t const smoothness
  , double const boldness)
{
  size_t dwidth, dheight;
  calculate_downscaled_resolution(&dheight, &dwidth, sheight, swidth, factor);
  double       (*darr)[dwidth] = (void*) dmatr;
  double const (*sarr)[swidth] = (void*) smatr;
  size_t hist[QUANTLEVELS]; // histogram of quantized values
  double    f[QUANTLEVELS]; // weight function based on histogram
  precless_t a, b, fsum;

  // ++++ <- `+` is extra area
  // +##+ <- `#` is downscaled area
  // +##+
  // ++++
  // `inc_to_factor` is size of area of values around downscaled area
  size_t inc_to_factor  = (factor + 1) / 2;
  size_t full_area_size = factor + 2 * inc_to_factor;
  size_t smoothval = 2 * smoothness + 1;
  // weight function based on `nd`
  double  fnd[IMAX][IMAX];
  for (long int i = 0; i < full_area_size; ++i)
    for (long int j = 0; j < full_area_size; ++j)
      fnd[i][j] = nd[full_area_size][i] * nd[full_area_size][j];

  // main for loop over resulting values
  for (size_t ih = 0; ih < dheight; ++ih)
    for (size_t iw = 0; iw < dwidth; ++iw)
    {
      long int hfrom = (-inc_to_factor) + factor *  ((long int)ih);
      long int wfrom = (-inc_to_factor) + factor *  ((long int)iw);
      long int hto   =   inc_to_factor  + factor * (((long int)ih) + 1);
      long int wto   =   inc_to_factor  + factor * (((long int)iw) + 1);
      // build histogram
      memset(hist, 0, QUANTLEVELS * sizeof(size_t));
      for (long int jh = hfrom; jh < hto; ++jh)
        for (long int jw = wfrom; jw < wto; ++jw)
        {
          size_t n = quantize(smatr, sheight, swidth, jh, jw);
          hist[n] += 1;
        }
      // calculate weight function for each discrete value from 0 to 255
      for (size_t i = 0; i < QUANTLEVELS; ++i)
      {
        plinit(&fsum);
        long int begin = ((long int)i) - (smoothval / 2);
        long int end   = ((long int)i) + (smoothval / 2); // used less or equal condition!
        for (long int k = 0, j = begin; j <= end; ++j, ++k)
          pladd(&fsum, nd[smoothval][k] * ((double)hist[limit(j, QUANTLEVELS)]));
        f[i] = pow(plread(&fsum), -(0.5 + 2.0 * boldness));
      }
      // prepare downscaling
      plinit(&a);
      plinit(&b);
      for (long int kh = 0, jh = hfrom; jh < hto; ++jh, ++kh)
        for (long int kw = 0, jw = wfrom; jw < wto; ++jw, ++kw)
        {
          size_t n = quantize(smatr, sheight, swidth, jh, jw);
          double x = sarr[limit(jh, sheight)][limit(jw, swidth)];
          pladd(&a, fnd[kh][kw] * f[n]);
          pladd(&b, fnd[kh][kw] * f[n] * x);
        }
      // finally got approximation
      darr[ih][iw] = plread(&b) / plread(&a);
    }
}

size_t limit(long int index, size_t size)
{
  size_t pindex = (size_t) ((index < 0)? -index : index);
  pindex = pindex % (2 * (size - 1));
  return (pindex >= size) ? 2 * (size - 1) - pindex : pindex;
}

size_t quantize(
    double const *const matr
  , size_t const h
  , size_t const w
  , size_t const ih
  , size_t const iw)
{
  // *** algorithm constants
  static double const
    quantmultiplier = ((double)(QUANTLEVELS - 1));
  static long double const
    quantattractor  = 0.5l * ((long double)(QUANTLEVELS));
  // quantanization
  double x = matr[limit(iw, w) + w * limit(ih, h)];
  return round(nexttoward(nexttoward(x, 0.5l) * quantmultiplier, quantattractor));
}
