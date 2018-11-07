//Osama Kergaye
//0U767339
//oct 3 2018
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defs.h"

/******************************************************
 * PINWHEEL KERNEL
 *
 * Your different versions of the pinwheel kernel go here
 ******************************************************/

/* 
 * naive_pinwheel - The naive baseline version of pinwheel 
 */
char naive_pinwheel_descr[] = "naive_pinwheel: baseline implementation";
void naive_pinwheel(pixel *src, pixel *dest)
{
  int i, j;

  for (i = 0; i < src->dim; i++)
    for (j = 0; j < src->dim; j++) {
      /* Check whether we're in the diamond region */
      if ((fabs(i + 0.5 - src->dim/2) + fabs(j + 0.5 - src->dim/2)) < src->dim/2) {
        /* In diamond region, so rotate and grayscale */
        int s_idx = RIDX(i, j, src->dim);
        int d_idx = RIDX(src->dim - j - 1, i, src->dim);
        dest[d_idx].red = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].green = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].blue = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
      } else {
        /* Not in diamond region, so keep the same */
        int s_idx = RIDX(i, j, src->dim);
        int d_idx = RIDX(i, j, src->dim);
        dest[d_idx] = src[s_idx];
      }
    }
}

/* with dim refactored*/
char pinwheel_descr[] = "pinwheel: Current working version";
void pinwheel(pixel *src, pixel *dest)
{
  int i, j;

  int dim = src->dim;
  int dimO2 = dim/2;
  int i_1, i_2, i_3, i_4, a1, a2, a3, a4;
  
  memcpy(dest, src, (dim*dim+1) * sizeof(pixel) );
  int move = 1;
  for (i = 1; i < dimO2; i++){
    for (j = dimO2-move; j < dimO2; j++) {

        /* In diamond region, so rotate and grayscale */
        i_1   = RIDX(i, j, dim);
        a1 = (dest[i_1].red + dest[i_1].green + dest[i_1].blue) / 3;

        i_2   = RIDX(dim - j - 1, i, dim);
        a2 = (dest[i_2].red + dest[i_2].green + dest[i_2].blue) / 3;

        i_3   = RIDX(dim - i - 1, dim - j - 1, dim);
        a3 = (dest[i_3].red + dest[i_3].green + dest[i_3].blue) / 3;

        i_4   = RIDX(j, dim - i - 1, dim);
        a4 = (dest[i_4].red + dest[i_4].green + dest[i_4].blue) / 3;

        dest[i_1].red = a4;
        dest[i_1].green = a4;
        dest[i_1].blue = a4;

        dest[i_2].red = a1;
        dest[i_2].green = a1;
        dest[i_2].blue = a1;

        dest[i_3].red = a2;
        dest[i_3].green = a2;
        dest[i_3].blue = a2;

        dest[i_4].red = a3;
        dest[i_4].green = a3;
        dest[i_4].blue = a3;
    }
    move++;
  }
}

 





/* 
 * pinwheel - Your current working version of pinwheel
 * IMPORTANT: This is the version you will be graded on
 *   #define RIDX(i,j,n) (1+(i)*(n)+(j))
 */
char pinwheel_dim_refactor_descr[] = "pinwheel: refact";
void pinwheel_dim_refactor(pixel *src, pixel *dest)
{
  int i, j;

  int dim = src->dim;
  int dimO2 = dim/2;
  int s_idx, d_idx, rgbVal, mov = 0;
//if ((fabs(i + 0.5 - dimO2) + fabs(j + 0.5 - dimO2)) < dimO2) {

  int temp = dim;
  mov  = dimO2-2;
  for (i = 1; i < dimO2; i++){
    for (j = dimO2-1; j > mov; j--) {
        s_idx = (1+(temp))+(j);
        /* In diamond region, so rotate and grayscale */
        d_idx = RIDX(dim - j - 1, i, dim);
        //d_idx = (1+((dim-j-1)*dim)+(i));

        //#define RIDX(i,j,n) (1+(i)*(n)+(j))
        // in+1+j

        rgbVal = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].red = rgbVal;
        dest[d_idx].green = rgbVal;
        dest[d_idx].blue = rgbVal;
    }
    temp+=dim;

    mov--;
  }

//bottomleft
  mov  = 0;
  for (i = dimO2; i < dim-1; i++){
    for (j = dimO2-1; j > mov; j--) {
        s_idx = (1+(i*dim))+(j);
        /* In diamond region, so rotate and grayscale */
        d_idx = RIDX(dim - j - 1, i, dim);
        rgbVal = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].red = rgbVal;
        dest[d_idx].green = rgbVal;
        dest[d_idx].blue = rgbVal;
    }
    mov++;
  }

  mov  = dimO2+1;
  temp = dim;
  for (i = 1; i < dimO2; i++){
    for (j = dimO2; j < mov; j++) {
        s_idx = (1+(temp))+(j);
        /* In diamond region, so rotate and grayscale */
        d_idx = RIDX(dim - j - 1, i, dim);
        rgbVal = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].red = rgbVal;
        dest[d_idx].green = rgbVal;
        dest[d_idx].blue = rgbVal;
    }
    temp+= dim;
    mov++;
  }

  mov  = dim-1;
  for (i = dimO2; i < dim-1; i++){
  temp = (1+(i*dim));

    for (j = dimO2; j < mov; j++) {
        s_idx = temp+(j);
        /* In diamond region, so rotate and grayscale */
        d_idx = RIDX(dim - j - 1, i, dim);

        rgbVal = ((int)src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;
        dest[d_idx].red = rgbVal;
        dest[d_idx].green = rgbVal;
        dest[d_idx].blue = rgbVal;
    }
    mov--;
  }









  //just copy back
  //topleft
  mov = dimO2;
  int mov2 = dimO2;
  temp = 0;
  pixel* destPtr;
  for (i = 0; i < dimO2; i++){
    for (j = 0; j < mov-1; j+=2){
        s_idx = (1+(temp)+(j));
        destPtr = &dest[s_idx];

        //#define RIDX(i,j,n) (1+(i*n)+(j))

        *destPtr = src[s_idx];
        *(destPtr+1) = src[s_idx+1];

    }
    for(; j < mov; j+=1){
        s_idx = (1+(temp)+(j));

        dest[s_idx] = src[s_idx];
    } 
    mov--;
    temp+=dim;
   for (j = mov2; j < dim-1; j+=2){
        s_idx = RIDX(i, j, dim);
        destPtr = &dest[s_idx];

        *destPtr = src[s_idx];
        *(destPtr+1) = src[s_idx+1];
    }
     for(; j < dim; j+=1){
        s_idx = RIDX(i, j, dim);
        dest[s_idx] = src[s_idx];
    } 
   mov2++;


  }

 

  //bottomleft
  mov = 1;
  for (i = dimO2; i < dim-1; i++){
    for (j = 0; j < mov-1; j+=2){
        s_idx = RIDX(i, j, dim);
        destPtr = &dest[s_idx];
        *destPtr = src[s_idx];
        *(destPtr+1) = src[s_idx+1];

    }
    for(; j < mov; j+=1){
        s_idx = RIDX(i, j, dim);
        dest[s_idx] = src[s_idx];
    }  
    mov++;
  }
//bottomright
  mov = dim-1;
  for (i = dimO2; i < dim-1; i++){
    for (j = mov; j < dim-1; j+=2){

        s_idx = RIDX(i, j, dim);
        destPtr = &dest[s_idx];
        *destPtr = src[s_idx];
        *(destPtr+1) = src[s_idx+1];
    }
    for(; j < dim; j+=1){
        s_idx = RIDX(i, j, dim);
        dest[s_idx] = src[s_idx];
    }
   mov--;
  }
  //bottom row
   i = dim-1;
  
    for (j = 0; j < dim-1; j+=2){
        s_idx = RIDX(i, j, dim);
        destPtr = &dest[s_idx];
        *destPtr = src[s_idx];
        *(destPtr+1) = src[s_idx+1];
    }
    for(; j < dim; j+=1){
        s_idx = RIDX(i, j, dim);
        dest[s_idx] = src[s_idx];
    }
   
  


}

/*********************************************************************
 * register_pinwheel_functions - Register all of your different versions
 *     of the pinwheel kernel with the driver by calling the
 *     add_pinwheel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_pinwheel_functions() {
  add_pinwheel_function(&pinwheel, pinwheel_descr);
  add_pinwheel_function(&pinwheel_dim_refactor, pinwheel_dim_refactor_descr);

  add_pinwheel_function(&naive_pinwheel, naive_pinwheel_descr);
}


/***************************************************************
 * GLOW KERNEL
 * 
 * Starts with various typedefs and helper functions for the glow
 * function, and you may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
  int red;
  int green;
  int blue;
} pixel_sum;

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
  sum->red = sum->green = sum->blue = 0;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_weighted_sum(pixel_sum *sum, pixel p, short weight) 
{
  sum->red += (int) p.red * weight/100;
  sum->green += (int) p.green * weight/100;
  sum->blue += (int) p.blue * weight/100;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
  current_pixel->red = (unsigned short)sum.red;
  current_pixel->green = (unsigned short)sum.green;
  current_pixel->blue = (unsigned short)sum.blue;
}

/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel_sum sum;
  pixel current_pixel;
 
  initialize_pixel_sum(&sum);
      
  for (ii=-1; ii < 2; ii+=2)
    for (jj=-1; jj < 2; jj+=2) 
      if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim) && (jj == -2)){
          pixel temp = src[RIDX(i+ii,j+jj,dim)];
          sum.red   +=  temp.red;
          sum.green += temp.green;
          sum.blue  += temp.blue;
          // temp = src[RIDX(i+2,j+jj,dim)];
          // sum.red   +=  temp.red;
          // sum.green += temp.green;
          // sum.blue  += temp.blue;
            
      }else if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)){

          accumulate_weighted_sum(&sum,
                                     src[RIDX(i+ii,j+jj,dim)],
                                     (16));

      }
  accumulate_weighted_sum(&sum, 
                              src[RIDX(i,j,dim)],
                              (30));
  assign_sum_to_pixel(&current_pixel, sum);

  return current_pixel;
}

/******************************************************
 * Your different versions of the glow kernel go here
 ******************************************************/

/*
 * naive_glow - The naive baseline version of glow 
 */
char naive_glow_descr[] = "naive_glow: baseline implementation";
void naive_glow(pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < src->dim; i++)
    for (j = 0; j < src->dim; j++)
      dst[RIDX(i, j, src->dim)] = weighted_combo(src->dim, i, j, src);
}





/*
 * glow - Your current working version of glow. 
 * IMPORTANT: This is the version you will be graded on
 */
char glow_descr[] = "glow: Current working version";
void glow(pixel *src, pixel *dst) 
{
  int i, j, temp;
  int dim = src->dim;  
  temp = 0;
  pixel_sum memo[dim][dim];

  //memoize the .16
  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){
      memo[i][j].red = (src[RIDX(i, j, dim)].red * 16)/100;
      memo[i][j].green = (src[RIDX(i, j, dim)].green * 16)/100;
      memo[i][j].blue = (src[RIDX(i, j, dim)].blue * 16)/100;
    }
  }




  int ii, jj;
  pixel_sum sum;
  pixel current_pixel;
  pixel tempPix;

  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){
        sum.red = sum.green = sum.blue = 0;

for (ii=-1; ii < 2; ii+=2){
    for (jj=-1; jj < 2; jj+=2){
      if ((i + ii >= 0) && (j + jj >= 0) && (i + ii < dim) && (j + jj < dim)){
          sum.red   +=  memo[i+ii][j+jj].red;
          sum.green += memo[i+ii][j+jj].green;
          sum.blue  += memo[i+ii][j+jj].blue;
   
      }
    }
  }

          tempPix = src[RIDX(i,j,dim)];
          sum.red   +=  tempPix.red * 30/100;
          sum.green += tempPix.green * 30/100;
          sum.blue  += tempPix.blue * 30/100;
          current_pixel.red = (unsigned short)sum.red;
          current_pixel.green = (unsigned short)sum.green;
          current_pixel.blue = (unsigned short)sum.blue;

  
      dst[(1+(temp)+(j))] = current_pixel;




    }
    temp+=dim;
  }
}



//epmpty space between fucns







//clean start func
char glow_descr_clean[] = "glow: Starting Clean";
void glow_clean(pixel *src, pixel *dst) 
{
  int i, j;
  int dim = src->dim;  
  for (i = 0; i < dim; i++){
    for (j = 0; j < dim; j++){

      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
      dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j+1, src);
      

    }
  }
}

#define max(a,b) (a) > (b) ? (a) : (b)
/********************************************************************* 
 * register_glow_functions - Register all of your different versions
 *     of the glow kernel with the driver by calling the
 *     add_glow_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_glow_functions() {
  add_glow_function(&glow, glow_descr);
  add_glow_function(&glow_clean, glow_descr_clean);

  add_glow_function(&naive_glow, naive_glow_descr);
}
