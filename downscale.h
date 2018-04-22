#ifndef DOWNSCALE_H
#define DOWNSCALE_H

// downscale
// matrices are passed and returned as a one-dimensional
// array, that flattens row by row
extern void downscale(
  // *** mutable params, only pointer to them is 'const'
    double  **const dmatr
  , size_t   *const dheight
  , size_t   *const dwidth
  // *** const params
  , double const *const smatr
  , size_t const sheight
  , size_t const swidth
    // `factor` possible values 2..16 both inclusive
    // set how many source values should be
    // aggregated for one destination value
  , size_t const factor
    // `smoothness` possible values 0..24 both inclusive
    // control blur in output values
  , size_t const smoothness
    // `boldness` possible values 0.0..1.0 both inclusive
    // control boldness of think lines on result image
  , double const boldness);

// downscale with manually pre-allocated memory for result
// matrices are passed and returned as a one-dimensional
// array, that flattens row by row
extern void padownscale(
  // *** mutable params, only pointer to them is 'const'
    double *const dmatr
  // *** const params
  , double const *const smatr
  , size_t const sheight
  , size_t const swidth
    // `factor` possible values 2..16 both inclusive
    // set how many source values should be
    // aggregated for one destination value
  , size_t const factor
    // `smoothness` possible values 0..24 both inclusive
    // control blur in output values
  , size_t const smoothness
    // `boldness` possible values 0.0..1.0 both inclusive
    // control boldness of think lines on result image
  , double const boldness);

// just compute final resolution
extern void calculate_downscaled_resolution(
  // mutable params, only pointer to them is 'const'
    size_t *const dheight
  , size_t *const dwidth
  // const params
  , size_t const sheight
  , size_t const swidth
  , size_t const factor);

#endif
