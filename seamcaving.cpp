#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat EnergyMatrix(Mat &image) {
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    Mat gradX, gradY;
    Sobel(gray, gradX, CV_16S, 1, 0, 3);
    Sobel(gray, gradY, CV_16S, 0, 1, 3);

    Mat absGradX, absGradY;
    convertScaleAbs(gradX, absGradX);
    convertScaleAbs(gradY, absGradY);

    Mat Energy;
    add(absGradX, absGradY, Energy);
    return Energy;
}

void vertical_seam(Mat &Energy, Mat &image) {
    int rows = image.rows;
    int columns = image.cols;
    int **Ematrix = new int*[rows];
    for (int i=0; i<rows; i++)
        Ematrix[i] = new int[columns];

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            Ematrix[i][j] = Energy.at<uchar>(i, j);

    // Building DP matrix
    int **dp_table = new int*[rows];

    for (int i=0; i<rows; i++) {
        dp_table[i] = new int[columns];
    }
        
    for (int i=0; i<columns; i++)
        dp_table[0][i] = Ematrix[0][i];

    for (int i=1; i<rows; i++) {
        dp_table[i][0] = Ematrix[i][0] + min(dp_table[i-1][0], dp_table[i-1][1]);
        for (int j=1; j<columns-1; j++) {
            int lmin = min(dp_table[i-1][j-1], dp_table[i-1][j]);
            int finalmin = min(lmin, dp_table[i-1][j+1]);
            dp_table[i][j] = Ematrix[i][j] + finalmin;
        }
        dp_table[i][columns-1] = Ematrix[i][columns-1] + min(dp_table[i-1][columns-2], dp_table[i-1][columns-1]);
    }

    // least energy in last row
    int deleteIndex[rows];
    int minIndex = 0;
    int minValue = dp_table[rows-1][0];
    for (int i=1; i<columns; i++) {
        if (minValue > dp_table[rows-1][i]) {
            minIndex = i;
            minValue = dp_table[rows-1][i];
        }
    }
    deleteIndex[rows-1] = minIndex;

    // Getting Vertical Seam
    for (int i=rows-2; i>=0; i--) {
        int previndex = deleteIndex[i+1];
        int best = dp_table[i][previndex];
        int idx = previndex;
        if (previndex > 0 && dp_table[i][previndex-1] < best) {
            best = dp_table[i][previndex-1];
            idx = previndex-1;
        }
        if (previndex < columns-1 && dp_table[i][previndex+1] < best) {
            best = dp_table[i][previndex+1];
            idx = previndex+1;
        }
        deleteIndex[i] = idx;
    }

    // Copying
    Mat result(rows, columns-1, image.type());
    Mat dupEnergy(rows, columns-1, Energy.type());
    for (int i = 0; i < rows; i++) {
        int skip = deleteIndex[i];
        int ycols=0;
        for (int j = 0; j < columns; j++) {
            if (j == skip) continue;
            dupEnergy.at<u_char>(i,ycols) = Energy.at<u_char>(i,j);
            result.at<Vec3b>(i, ycols) = image.at<Vec3b>(i,j);
            ycols++;
        }
    }
    result.copyTo(image);
    dupEnergy.copyTo(Energy);

    for (int i=0; i<rows; i++) {
        delete[] Ematrix[i];
        delete[] dp_table[i];
    }
    result.release();
    dupEnergy.release();
}

void horizontal_seam(Mat &Energy, Mat &image) {
    int rows = image.rows;
    int columns = image.cols;
    int **Ematrix = new int*[rows];
    for (int i=0; i<rows; i++)
        Ematrix[i] = new int[columns];

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            Ematrix[i][j] = Energy.at<uchar>(i, j);

    // Building DP matrix
    int **dp_table = new int*[rows];

    for (int i=0; i<rows; i++)
        dp_table[i] = new int[columns];

    for (int i=0; i<rows; i++)
        dp_table[i][0] = Ematrix[i][0];

    for (int j=1; j<columns; j++) {
        dp_table[0][j] = Ematrix[0][j] + min(dp_table[0][j-1], dp_table[1][j-1]);
        for (int i=1; i<rows-1; i++) {
            int lmin = min(dp_table[i-1][j-1], dp_table[i][j-1]);
            int finalmin = min(lmin, dp_table[i+1][j-1]);
            dp_table[i][j] = Ematrix[i][j] + finalmin;
        }
        dp_table[rows-1][j] = Ematrix[rows-1][j] + min(dp_table[rows-2][j-1], dp_table[rows-1][j-1]);
    }

    // least energy in last row
    int deleteIndex[columns];
    int minIndex = 0;
    int minValue = dp_table[0][columns-1];
    for (int i=1; i<rows; i++) {
        if (minValue > dp_table[i][columns-1]) {
            minIndex = i;
            minValue = dp_table[i][columns-1];
        }
    }
    deleteIndex[columns-1] = minIndex;

    // Getting horizontal Seam
    for (int i=columns-2; i>=0; i--) {
        int previndex = deleteIndex[i+1];
        int best = dp_table[previndex][i];
        int idx = previndex;
        if (previndex > 0 && dp_table[previndex-1][i] < best) {
            best = dp_table[previndex-1][i];
            idx = previndex-1;
        }
        if (previndex < rows-1 && dp_table[previndex+1][i] < best) {
            best = dp_table[previndex+1][i];
            idx = previndex+1;
        }
        deleteIndex[i] = idx;
    }

    // Copying
    Mat result(rows-1, columns, image.type());
    Mat dupEnergy(rows-1, columns, Energy.type());
    for (int i = 0; i < columns; i++) {
        int skip = deleteIndex[i];
        int xcols=0;
        for (int j = 0; j < rows; j++) {
            if (j == skip) continue;
            result.at<Vec3b>(xcols,i) = image.at<Vec3b>(j,i);
            dupEnergy.at<u_char>(xcols,i) = Energy.at<u_char>(j,i);
            xcols++;
        }
    }
    result.copyTo(image);
    dupEnergy.copyTo(Energy);

    for (int i=0; i<rows; i++) {
        delete[] Ematrix[i];
        delete[] dp_table[i];
    }
    result.release();
    dupEnergy.release();
}


int main(int argc, char* argv[]) {
    Mat image = imread(argv[1], IMREAD_COLOR);

    if (image.empty()) {
        cout << "Unable to open the image" << endl;
        return 0;
    }
    
    int height = stoi(argv[2]);
    int width = stoi(argv[3]);

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    int rows = image.rows;
    int columns = image.cols;

    int verticalcount = columns - width;
    int horizontalcount = rows - height;

    while (horizontalcount>0 || verticalcount>0) {
        if (verticalcount>0) {
            Mat Energy = EnergyMatrix(image);
            vertical_seam(Energy, image);
            verticalcount--;
        }
        if (horizontalcount >0) {
            Mat Energy = EnergyMatrix(image);
            horizontal_seam(Energy, image);
            horizontalcount--;
        }
    }
    imwrite("output_image.jpg", image);
    return 0;
}