#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>

int main()
{
      int  n, incx, incy, i;
      float   *x, *y;
      //float    res;
      int  len_x, len_y;

	  double mat[] = {	1,2,3,
		  				4,5,6,
		  				7,8,9,
						1,1,1
					};
	  double vec[] = {1,2,3};
	  double res[4];

	  cblas_dgemv(CblasRowMajor, CblasNoTrans, 4, 3, 1, mat, 3, vec, 1, 0, res, 1);
	  //?gemv(                       'T',      M, N, a,  A,  M,  X,  1, b,  Y,  1)

	  // void cblas_dgemv(
		//   		const enum CBLAS_ORDER Order,
      //            const enum CBLAS_TRANSPOSE TransA,
		// 		 const int M,
		// 		 const int N,
      //            const double alpha,
		// 		 const double *A,
		// 		 const int lda,
      //            const double *X,
		// 		 const int incX,
		// 		 const double beta,
      //            double *Y,
		// 		 const int incY
		// 	 );


      // n = 5;
      // incx = 2;
      // incy = 1;
      //
      // len_x = 1+(n-1)*abs(incx);
      // len_y = 1+(n-1)*abs(incy);
      // x    = (float *)calloc( len_x, sizeof( float ) );
      // y    = (float *)calloc( len_y, sizeof( float ) );
      // if( x == NULL || y == NULL ) {
      //     printf( "\n Can't allocate memory for arrays\n");
      //     return 1;
      // }
      //
      // for (i = 0; i < n; i++) {
      //     x[i*abs(incx)] = 2.0;
      //     y[i*abs(incy)] = 1.0;
      // }
      //
      // res = cblas_sdot(n, x, incx, y, incy);
      //
      // printf("\n       SDOT = %7.3f", res);
      //
      // free(x);
      // free(y);
      //
	  // printf("\n");

	  for (int i = 0; i < 4; i++) {
	  	printf("%d  ", (int) res[i]);
	  }

	  printf("\n");

      return 0;
}
