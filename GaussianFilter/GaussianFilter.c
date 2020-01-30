// C++ prgroam to generate Gaussian filter
#include <stdio.h>
#include <math.h>
#ifdef opt
#include <arm_neon.h>
#endif

#define FILTER_SIZE 11       //filter size must be a odd number!!
// Function to create Gaussian filter
#ifndef opt
void FilterCreation(float GKernel[FILTER_SIZE][FILTER_SIZE], int size)
{
    // intialising standard deviation to 1.0
    float sigma = 1.0;
    float r, s = 2.0 * sigma * sigma;
    int n_max = (size%2) ? size/2: size/2-1;    //this is stricly to prevent errors caused by an even FILTER_SIZE
    // sum is for normalization
    float sum = 0.0;
 
    // generating 5x5 kernel
    for (int x = -n_max; x <= n_max; x++)
    {
        for(int y = -n_max; y <= n_max; y++)
        {
            GKernel[x + n_max][y + n_max] =
                     (exp(-(x*x + y*y)/s))/(M_PI * s);
            sum += GKernel[x + n_max][y + n_max];
        }
    }
 
    // normalising the Kernel
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            GKernel[i][j] /= sum;
}
#else 
void FilterCreation(float GKernel[FILTER_SIZE][FILTER_SIZE], int size)
{
    // intialising standard deviation to 1.0
    float sigma = 1.0;
    float r, s = 2.0 * sigma * sigma;
    int n_max = (size%2) ? size/2: size/2-1;    //this is stricly to prevent errors caused by an even FILTER_SIZE
    float32x4_t v_Gkernel, v_inv_sum, v_x, v_y;
    float32x4_t v_inv_s = vdupq_n_f32(1/(2.0 * sigma * sigma));     //1/s vector constant
    float32x4_t v_inv_s_pi = vdupq_n_f32(1/(2.0 * sigma * sigma* M_PI));  //1/(s*pi) vector constant
    float32x4_t v_four = vdupq_n_f32(4);  //four constant
    float32x4_t v_init_y;
    // sum is for normalization
    v_init_y[0] = -n_max;
    v_init_y[1] = -n_max+1;
    v_init_y[2] = -n_max+2;
    v_init_y[3] = -n_max+3;
    float sum = 0.0;
    
    // generating 5x5 kernel
    for (int x = -n_max; x <= n_max; x++)
    {
        v_x = vdupq_n_f32((float) x);
        v_y = v_init_y;
        for(int y = -n_max; y <= n_max-(size%4); y+=4)
        {
            //r = sqrt(x*x + y*y);        //isso é inutil???
            v_Gkernel = vaddq_f32(vmulq_f32(v_x,v_x),vmulq_f32(v_y,v_y));
            v_Gkernel = vmulq_f32(v_Gkernel, v_inv_s);
            v_Gkernel[0] = exp(-v_Gkernel[0]);
            v_Gkernel[1] = exp(-v_Gkernel[1]);
            v_Gkernel[2] = exp(-v_Gkernel[2]);
            v_Gkernel[3] = exp(-v_Gkernel[3]);
            v_Gkernel = vmulq_f32(v_Gkernel, v_inv_s_pi);
            vst1q_f32(&GKernel[x+n_max][y+n_max], v_Gkernel);
            sum += v_Gkernel[0] + v_Gkernel[1] + v_Gkernel[2] + v_Gkernel[3];
            v_y = vaddq_f32(v_y, v_four);  //v_y++
        }
        //we cannot vectorize this:
        for(int y = n_max-(size%4)+1; y <= n_max; y++)
        {
            //r = sqrt(x*x + y*y);        //isso é inutil???
            GKernel[x + n_max][y + n_max] =
                     (exp(-(x*x+y*y)/s))/(M_PI * s);
            sum += GKernel[x + n_max][y + n_max];
        }
    }
    v_inv_sum = vdupq_n_f32(1/sum); //1/sum vector for parallel computing
    // normalising the Kernel
    for (int i = 0; i < size; ++i)
    {
        for(int j = 0; j < size-(size%4); j+=4)
        {
            v_Gkernel = vld1q_f32(&GKernel[i][j]);
            v_Gkernel = vmulq_f32(v_Gkernel, v_inv_sum);
            vst1q_f32(&GKernel[i][j], v_Gkernel);
        }
        for (int j = size-(size%4); j < size; ++j)
            GKernel[i][j] /= sum;
    }
}
#endif
// Driver program to test above function
int main()
{
    float GKernel[FILTER_SIZE][FILTER_SIZE];
    FilterCreation(GKernel,FILTER_SIZE);
    
    for (int i = 0; i < FILTER_SIZE; ++i)
    {
        for (int j = 0; j < FILTER_SIZE; ++j)
            printf("%g\t", GKernel[i][j]);
        printf("\n");
    }
}
