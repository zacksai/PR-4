#include "digitblob.h"
#include <iostream>
// Add more includes if necessary
#include <vector>

using namespace std;

// TO DO - Complete this function
bool Location::operator<(const Location &other) const {
    // return true if lhs col is < other col
    return col < other.col;

}

// TO DO - Complete this function
DigitBlob::DigitBlob() {
    img_ = NULL;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // ul_'s Location default constructor already initializes it to -1,-1

    // Initialize h_ and w_ and any other data members
    h_ = w_ = 0;
    digit_ = 'Z';

}

// TO DO - Complete this function
DigitBlob::DigitBlob(uint8_t **img, Location upperleft, int height, int width) {
    img_ = img;
    digit_ = '!'; // dummy value

    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // Initialize ul_, h_ and w_ and any other data members
    ul_ = upperleft;
    h_ = height;
    w_ = width;

}

// TO DO - Complete this function if necessary
DigitBlob::~DigitBlob() {
    // Add code if it is necessary

}

// TO DO - Complete this function
void DigitBlob::classify() {
    calc_bit_quads();
    calc_euler_number();
    // Call helper functions to calculate features


    // TO DO: use the results of helper functions to calculate features
    //    We suggest starting with the Euler number, but you are free to
    //    change our structure



}

// Complete - Do not alter
char DigitBlob::getClassification() const {
    return digit_;
}

// TO DO - Complete this function
void DigitBlob::printClassificationResults() const {
    cout << "Digit blob at " << ul_.row << "," << ul_.col << " h=" << h_ << " w=" << w_ << endl;
    cout << "Bit quads: 1, 2, D, 3, 4:";
    cout << " " << bq1_ << " " << bq2_ << " " << bqd_;
    cout << " " << bq3_ << " " << bq4_ << endl;
    cout << "Euler number is " << euler_ << endl;
    // TO DO: Add any other couts to print classification test data





    cout << "****Classified as: " << digit_ << "\n\n" << endl;

}

// Complete - Do not alter
const Location &DigitBlob::getUpperLeft() const {
    return ul_;
}

// Complete - Do not alter
int DigitBlob::getHeight() const {
    return h_;
}

// Complete - Do not alter
int DigitBlob::getWidth() const {
    return w_;
}

// Complete - Do not alter
bool DigitBlob::operator<(const DigitBlob &other) {
    // Use Location's operator< for DigitBlob operator<
    return ul_ < other.ul_;
}

// Complete - Do not alter
void DigitBlob::calc_euler_number() {
    euler_ = (bq1_ - bq3_ - 2 * bqd_) / 4;
}

// TO DO - Complete this function to set bq1_, bq2_, etc.
void DigitBlob::calc_bit_quads() {

    // iterate through the digit blob till the last blocks
    for (int r = ul_.row - 1; r < ul_.row + h_; r++) {
        for (int c = ul_.col - 1; c < ul_.col + w_; c++) {

            // determine if neighboring pixels are black
            bool bl[4];
            bl[0] = (img_[r][c] == 0);
            bl[1] = (img_[r][c + 1] == 0);
            bl[2] = (img_[r + 1][c + 1] == 0);
            bl[3] = (img_[r + 1][c] == 0);

            // tally black pixels
            int n = 0;
            for (bool i: bl) {
                if (i) n++;
            }

            // increment bit quads based on number of black pixels
            if (n == 0) bq0_++;
            else if (n == 1) bq1_++;
            else if (n == 2) {
                // diagonal vs adjacent
                if (bl[0] == bl[2] || bl[1] == bl[3]) bqd_++;
                else bq2_++;
            } else if (n == 3) bq3_++;
            else if (n == 4) bq4_++;
        }
    } // repeat till last blocks

}

// Add more private helper function implementations below

