//function performs noise removal on image, using ranked order filter method.

void RankedOrderFilter(Mat image, int x, int y, int rank) //x,y must be odd
{
	image.convertTo(image, CV_32F); 
	for(int channel=0;channel<image.channels();channel++) //for each channel
	{
		for(int i=0;i<image.rows;i++) //for each row
		{
			for(int j=0;j<image.cols;j++) //for each column
			{
				vector<float> xi; //make a temporary list-vector
				for(int k=i-x/2;k<=i+x/2;k++) //apply the window specified by x and y
				{
					for(int m=j-y/2;m<=j+y/2;m++)
					{
						if((k<0)||(m<0)) xi.push_back(0); //on edges of the image use 0 values
						else xi.push_back(image.data[image.step[0]*i + image.step[1]* j + channel]);
					}
				}				
				std::sort(std::begin(xi),std::end(xi));	//sort elements of 'xi' neighbourhood vector 			
				image.data[image.step[0]*i + image.step[1]* j + channel]=xi[rank]; //replace pixel with element specified by 'rank'				
			}
		}
	}
	image.convertTo(image, CV_8U);
}