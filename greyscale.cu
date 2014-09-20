#include <math.h>
#include "calc.cpp"
#include "utils.h"
#include <stdio.h>
#include <algorithm>

__global__
void rgba_to_greyscale(const uchar4* const rgbaImage,
                       unsigned char* const greyImage,
                       int numRows, int numCols)
{
  int index_x = blockIdx.x * blockDim.x + threadIdx.x;
  int index_y = blockIdx.y * blockDim.y + threadIdx.y;

  // map the two 2D indices to a single linear, 1D index
  int grid_width = gridDim.x * blockDim.x;
  int index = index_y * grid_width + index_x;
  
  vector<float> xi; //make a temporary list-vector
  for(int k=index_x-5/2;k<=index_x+5/2;k++) { //apply the window specified by x and y
	for(int m=index_y-5/2;m<=index_y+5/2;m++) {
		if((k<0)||(m<0)) xi.push_back(0); //on edges of the image use 0 values
		else xi.push_back(rgbaImage[k * numCols + m]);
	}
  }				
  std::sort(std::begin(xi),std::end(xi));	//sort elements of 'xi' neighbourhood vector 			
  greyImage[index]=xi[3]; //replace pixel with element specified by 'rank' (3)				
  
  // write out the final result
  greyImage[index] =  .299f * rgbaImage[index].x + .587f * rgbaImage[index].y + .114f * rgbaImage[index].z;
  
}

void your_rgba_to_greyscale(const uchar4 * const h_rgbaImage, uchar4 * const d_rgbaImage,
                            unsigned char* const d_greyImage, size_t numRows, size_t numCols)
{
  const int thread = 16;
  const dim3 blockSize( thread, thread, 1);
  const dim3 gridSize( ceil(numRows/(float)thread), ceil(numCols/(float)thread), 1);
  rgba_to_greyscale<<<gridSize, blockSize>>>(d_rgbaImage, d_greyImage, numRows, numCols);
  
  cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());
}
