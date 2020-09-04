/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/* 
 * Please fill in the following student struct 
 */
team_t team = {
    "2264612",              /* Student ID */

    "Zeki Oren",     /* full name */
    "e226461@metu.edu.tr",  /* email address */

    "",                   /* leave blank */
    ""                    /* leave blank */
};


/***************
 * Sobel KERNEL
 ***************/

/******************************************************
 * Your different versions of the sobel functions  go here
 ******************************************************/

/* 
 * naive_sobel - The naive baseline version of Sobel 
 */
char naive_sobel_descr[] = "sobel: Naive baseline implementation";
void naive_sobel(int dim,int *src, int *dst) {
    int i,j,a;
    int b = dim + 1;
  

for(int k=0;k<dim;k++){
	dst[k]=0;
	dst[dim*dim-dim +k]=0;
	dst[k*dim]=0;
	dst[k*dim+dim-1]=0;
}
    for(i = 1; i < dim-1; i++){
	dst[dim*i+dim-1]=0;
        for(j = 1; j < dim-2; j+=2) {
	   a = j*dim+i;	
	   dst[a]=0;
			dst[a] = dst[a] - src[a - b]
					- 2 * src[a - 1]
					- src[a + dim - 1]
					+ src[a - dim + 1]
					+ 2 * src[a + 1]
					+ src[a + b];
	   a = (j+1)*dim+i;	
	   dst[a]=0;
			dst[a] = dst[a] - src[a - b]
					- 2 * src[a - 1]
					- src[a + dim - 1]
					+ src[a - dim + 1]
					+ 2 * src[a + 1]
					+ src[a + b];


      }
     } 


}
/* 
 * sobel - Your current working version of sobel
 * IMPORTANT: This is the version you will be graded on
 */

char sobel_descr[] = "Dot product: Current working version";
void sobel(int dim,int *src,int *dst) 
{

       naive_sobel(dim,src,dst);

}

/*********************************************************************
 * register_sobel_functions - Register all of your different versions
 *     of the sobel functions  with the driver by calling the
 *     add_sobel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_sobel_functions() {
    add_sobel_function(&naive_sobel, naive_sobel_descr);   
    add_sobel_function(&sobel, sobel_descr);   
    /* ... Register additional test functions here */
}




/***************
 * MIRROR KERNEL
 ***************/

/******************************************************
 * Your different versions of the mirror func  go here
 ******************************************************/

/* 
 * naive_mirror - The naive baseline version of mirror 
 */
char naive_mirror_descr[] = "Naive_mirror: Naive baseline implementation";
void naive_mirror(int dim,int *src,int *dst) {
    //RIDX(i,j,n) ((i)*(n)+(j))
	 int i,j,d,b=0;
	 int a = dim-1;
  for(j = 0; j < dim; j++){
	d=b+a;
        for(i = 0; i < dim-31;i+=32) {
            dst[b+i]=src[d-i];
            dst[b+i+1]=src[d-(i+1)];
            dst[b+i+2]=src[d-(i+2)];
            dst[b+i+3]=src[d-(i+3)];
            dst[b+i+4]=src[d-(i+4)];
            dst[b+i+5]=src[d-(i+5)];
            dst[b+i+6]=src[d-(i+6)];
            dst[b+i+7]=src[d-(i+7)];
            dst[b+i+8]=src[d-(i+8)];
            dst[b+i+9]=src[d-(i+9)];
            dst[b+i+10]=src[d-(i+10)];
            dst[b+i+11]=src[d-(i+11)];
            dst[b+i+12]=src[d-(i+12)];
            dst[b+i+13]=src[d-(i+13)];
            dst[b+i+14]=src[d-(i+14)];
            dst[b+i+15]=src[d-(i+15)];
            dst[b+i+16]=src[d-(i+16)];
            dst[b+i+17]=src[d-(i+17)];
            dst[b+i+18]=src[d-(i+18)];
            dst[b+i+19]=src[d-(i+19)];
            dst[b+i+20]=src[d-(i+20)];
            dst[b+i+21]=src[d-(i+21)];
            dst[b+i+22]=src[d-(i+22)];
            dst[b+i+23]=src[d-(i+23)];
            dst[b+i+24]=src[d-(i+24)];
            dst[b+i+25]=src[d-(i+25)];
            dst[b+i+26]=src[d-(i+26)];
            dst[b+i+27]=src[d-(i+27)];
            dst[b+i+28]=src[d-(i+28)];
            dst[b+i+29]=src[d-(i+29)];
            dst[b+i+30]=src[d-(i+30)];
            dst[b+i+31]=src[d-(i+31)];
	    

        }
	b+=dim;
	}
}
//
//


/* 
 * mirror - Your current working version of mirror
 * IMPORTANT: This is the version you will be graded on
 */
char mirror_descr[] = "Mirror: Current working version";
void mirror(int dim,int *src,int *dst) 
{
 	naive_mirror(dim,src,dst);
}

/*********************************************************************
 * register_mirror_functions - Register all of your different versions
 *     of the mirror functions  with the driver by calling the
 *     add_mirror_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_mirror_functions() {
    add_mirror_function(&naive_mirror, naive_mirror_descr);   
    add_mirror_function(&mirror, mirror_descr);   
    /* ... Register additional test functions here */
}

