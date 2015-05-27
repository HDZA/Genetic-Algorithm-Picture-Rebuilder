
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>

using namespace std;
using namespace cv;

void mutate(Mat image, Mat overlay);
double findFitness(Mat image,Mat source);

int main()
{

	Mat img = imread("images.jpg");
    srand (time(NULL)); //Seed the random function/

    double parentFitness = 0;
    double childFitness = 0;

    //init the parent
    int rows = img.rows;
    int cols = img.cols;

    int numIterations = 0;
    Mat parent_image = Mat::zeros( rows, cols, CV_8UC3 ); //Create an empty drawing space that is the same size as the source image.
    parent_image = cv::Scalar(255,255,255);
    Mat parent_overlay = parent_image.clone(); //Create an overlay for the parent.
   // mutate(parent_image, parent_overlay); //First mutation



    Mat child_image= parent_image.clone();
    Mat child_overlay = child_image.clone();
   // mutate(child_image, child_overlay);





    //parentFitness = findFitness(parent_image, img);
    //childFitness = findFitness(child_image, img);

    while(numIterations != 100000){
    	if(numIterations == 0){
			mutate(parent_image, parent_overlay);

			child_image= parent_image.clone();
			child_overlay = child_image.clone();

			mutate(child_image, child_overlay);
    	}else{
    		mutate(child_image, child_overlay);
    	}

    	parentFitness = findFitness(parent_image, img);
    	childFitness = findFitness(child_image, img);

    	if(childFitness <= parentFitness){
    		parent_image = child_image.clone();
    		parent_overlay = parent_image.clone();
    		//child_image = parent_image.clone();
    		//mutate(child_image, child_overlay);
    		cout << "Found a better version."<<"#"<<numIterations <<'\n';
    	}else{
    		child_image = parent_image.clone();
    		child_overlay = child_image.clone();
    	}
    	numIterations++;

    }

    imshow("best so far", parent_image);
    waitKey(0);

    return 0;

}




void mutate(Mat image, Mat overlay){

	int lineType = 8;
	/** Create some points */
	Point points[1][3];
	points[0][0] = Point( rand() % image.cols,rand() % image.rows );
	points[0][1] = Point( rand() % image.cols,rand() % image.rows );
	points[0][2] = Point( rand() % image.cols,rand() % image.rows  );

	const Point* ppt[1] = {points[0]};
	int npt[] = {3};

	fillPoly(overlay, ppt, npt, 1, Scalar(rand()%255,rand()%255,rand()%255), lineType); //Draw a random colored triangle that is a random size on the image overlay.

	double opacity = .08; //Lower = more transparent, higher = more solid. 1 is the highest you can go.
	addWeighted(overlay, opacity, image, 1-opacity, 0, image, -1);

}

double findFitness(Mat image, Mat source){
	int matchingPixels = 0;
	int rows = image.rows;
	int cols = image.cols;

	double blue = 0;
	double green = 0;
	double red = 0;
	double error;

	for(int i = 0; i< rows; i++){
		for(int j = 0; j<cols; j++){
			blue  =  image.at<Vec3b>(i,j)[0] - source.at<Vec3b>(i,j)[0]; //Subtract the parent pixel blue value from the child pixel blue value;
			green =  image.at<Vec3b>(i,j)[1] - source.at<Vec3b>(i,j)[1]; //Subtract the parent pixel green value from the child pixel blue value;
			red   =  image.at<Vec3b>(i,j)[2] - source.at<Vec3b>(i,j)[2]; //Subtract the parent pixel red value from the child pixel blue value;
			error += blue*blue + green*green + red * red;
		}
	}


	return error;
}

