#ifndef ND_H
#define ND_H

#define IMAX 51

// the segmented integral of standard normal distribution
// density function (`sigma` = 1, `mean` = 0)
// value nd[i][k] means integral of k-th segment of total `i`.
// integral normilazed to definite integral from -3*`sigma` to 3*`sigma`
// so sum of all integral values of each segment for each `i` is equal to 1
extern double nd[IMAX][IMAX - 1];

// initialize `nd` array with extremely high precision values
void assign_nd(void);

#endif
