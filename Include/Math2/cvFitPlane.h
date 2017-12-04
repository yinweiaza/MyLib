//³ö×Ôhttp://blog.csdn.net/zhouyelihua/article/details/46122977

#if(1500 == _MSC_VER)
#include "..\..\Include\OpenSource\OpenCV\cxtypes.h"
#include "..\..\Include\OpenSource\OpenCV\highgui.h"
#include "..\..\Include\OpenSource\OpenCV\cxcore.h"
#include "..\..\Include\OpenSource\OpenCV\cv.h"
#else
#include "..\..\Include\OpenSource\OpenCV\OpenCV2.4.11\opencv.hpp"
#endif

//Ax+by+cz=D  
void cvFitPlane(const CvMat* pPoints, float* plane)
{  
	// Estimate geometric centroid.  
	int nrows = pPoints->rows;  
	int ncols = pPoints->cols;  
	int type = pPoints->type;  
	CvMat* pCentroid = cvCreateMat(1, ncols, type);  
	cvSet(pCentroid, cvScalar(0));  
	for (int c = 0; c<ncols; c++){  
		for (int r = 0; r < nrows; r++)  
		{  
			pCentroid->data.fl[c] += pPoints->data.fl[ncols*r + c];  
		}  
		pCentroid->data.fl[c] /= nrows;  
	}  
	// Subtract geometric centroid from each point.  
	CvMat* pPoints2 = cvCreateMat(nrows, ncols, type);  
	for (int r = 0; r<nrows; r++)  
		for (int c = 0; c<ncols; c++)  
			pPoints2->data.fl[ncols*r + c] = pPoints->data.fl[ncols*r + c] - pCentroid->data.fl[c];  
	// Evaluate SVD of covariance matrix.  
	CvMat* pA = cvCreateMat(ncols, ncols, type);  
	CvMat* pW = cvCreateMat(ncols, ncols, type);  
	CvMat* pV = cvCreateMat(ncols, ncols, type);  
	cvGEMM(pPoints2, pPoints, 1, NULL, 0, pA, CV_GEMM_A_T);  
	cvSVD(pA, pW, NULL, pV, CV_SVD_V_T);  
	// Assign plane coefficients by singular vector corresponding to smallest singular value.  
	plane[ncols] = 0;  
	for (int c = 0; c<ncols; c++){  
		plane[c] = pV->data.fl[ncols*(ncols - 1) + c];  
		plane[ncols] += plane[c] * pCentroid->data.fl[c];  
	}  
	// Release allocated resources.  
	cvReleaseMat(&pCentroid);  
	cvReleaseMat(&pPoints2);  
	cvReleaseMat(&pA);  
	cvReleaseMat(&pW);  
	cvReleaseMat(&pV);  
}  