void referenceCalculation(const uchar4* const rgbaImage,
                          unsigned char *const greyImage,
                          size_t numRows,
                          size_t numCols)
{
  for (size_t r = 0; r < numRows; ++r) {
    for (size_t c = 0; c < numCols; ++c) {
      uchar4 rgba = rgbaImage[r * numCols + c];
      float channelSum = .299f * rgba.x + .587f * rgba.y + .114f * rgba.z;
      greyImage[r * numCols + c] = channelSum;
    }
  }
  
  //now that the grey-scale image is complete clear noise
  for(size_t r = 0; r < numRows; ++r) { //for each row
	for(size_t c = 0; c < numCols; ++c) { //for each column
		vector<float> xi; //make a temporary list-vector
		for(int k=r-5/2;k<=r+5/2;k++) { //apply the window specified by x and y
			for(int m=c-5/2;m<=c+5/2;m++) {
				if((k<0)||(m<0)) xi.push_back(0); //on edges of the image use 0 values
				else xi.push_back(greyImage[k * numCols + m]);
			}
		}				
		std::sort(std::begin(xi),std::end(xi));	//sort elements of 'xi' neighbourhood vector 			
		greyImage[r * numCols + c]=xi[3]; //replace pixel with element specified by 'rank'				
	}
  }
  
}
