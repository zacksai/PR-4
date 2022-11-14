#include "digitblob.h"
#include "numimg.h"
#include "bmplib.h"
#include <algorithm> // for std::sort
#include <deque>
#include <iomanip>
#include <iostream>

using namespace std;

// TO DO: Complete this function
NumImg::NumImg(const char *bmp_filename) {
    //  Note: readGSBMP dynamically allocates a 2D array 
    //    (i.e. array of pointers (1 per row/height) where each  
    //    point to an array of unsigned char (uint8_t) pixels)
    // TO DO:
    // call readGSBMP to initialize img_, h_, and w_;
    img_ = readGSBMP(bmp_filename, h_, w_);

    // Leave this check
    if (img_ == NULL) {
        throw std::logic_error("Could not read input file");
    }

    // Convert to Black and White using a fixed threshold 
    for (int i = 0; i < h_; i++) {
        for (int j = 0; j < w_; j++) {
            if (img_[i][j] > 150) {
                img_[i][j] = 255;
            } else {
                img_[i][j] = 0;
            }
        }
    }
}

// TO DO: Complete this function
NumImg::~NumImg() {
    // Add code here if necessary

}

// TO DO: Complete this function
size_t NumImg::findAndCreateDigitBlobs() {

    // create return value
    size_t num_found = 0;

    // create explored array
    bool **expl = new bool *[w_];
    for (int i = 0; i < w_; ++i) {
        expl[i] = new bool[h_];
    }

    // initialize boolean array
    for (int i = 0; i < w_; ++i) {
        for (int j = 0; j < h_; ++j) {
            expl[i][j] = false;
        }
    }

    // 0 = black, 255 = white
    // iterate through the pixels till top left of each is found
    for (int i = 0; i < h_; ++i) { // rows
        for (int j = 0; j < w_; ++j) { // columns

            // black pixel found
            if (img_[i][j] == 0) {

                // Mark pixel as explored, create digit blob
                expl[i][j] = true;
                DigitBlob temp = createDigitBlob(expl, i, j);

                // handle stray pixels
                if (temp.getHeight() > 1 || temp.getWidth() > 1) {

                    // add blob to blobs vector, update i, j, and num_found
                    blobs_.push_back(temp);
                    i = temp.getUpperLeft().row + temp.getHeight();
                    j = temp.getUpperLeft().col + temp.getWidth();
                    num_found++;
                }
            }
        }
    }

    // sort digit blobs
    // sortDigitBlobs();


    // deallocate memory
    for (int i = 0; i < w_; ++i) {
        delete[] expl[i];
    }
    delete[] expl;

    // return # blobs found
    return num_found;
}

// Complete - Do not alter
std::string NumImg::classify(bool withDebug) {
    std::string res;
    for (size_t i = 0; i < blobs_.size(); i++) {
        blobs_[i].classify();
        if (withDebug) {
            blobs_[i].printClassificationResults();
        }
        char c = blobs_[i].getClassification();
        res += c;
    }
    return res;
}

// Complete - Do not alter
void NumImg::printBoundingBoxes() const {
    cout << setw(2) << "i" << setw(6) << "ULRow" << setw(6) << "ULCol" << setw(4) << "Ht." << setw(4) << "Wi." << endl;
    for (size_t i = 0; i < blobs_.size(); i++) {
        const DigitBlob &b = blobs_[i];
        cout << setw(2) << i << setw(6) << b.getUpperLeft().row << setw(6) << b.getUpperLeft().col
             << setw(4) << b.getHeight() << setw(4) << b.getWidth() << endl;
        // cout << "Blob " << i << " ul=(" << b.getUpperLeft().row << "," << b.getUpperLeft().col 
        //     << ") h=" << b.getHeight() << " w=" << b.getWidth() << endl;
    }

}

// Complete - Do not alter
const DigitBlob &NumImg::getDigitBlob(size_t i) const {
    if (i >= blobs_.size()) {
        throw std::out_of_range("Index to getDigitBlob is out of range");
    }
    return blobs_[i];
}

// Complete - Do not alter
size_t NumImg::numDigitBlobs() const {
    return blobs_.size();
}

// Complete - Do not alter
void NumImg::sortDigitBlobs() {
    // std::sort(blobs_.begin(), blobs_.end());
}

// Complete - Do not alter
void NumImg::drawBoundingBoxesAndSave(const char *filename) {
    for (size_t i = 0; i < blobs_.size(); i++) {
        Location ul = blobs_[i].getUpperLeft();
        int h = blobs_[i].getHeight();
        int w = blobs_[i].getWidth();
        for (int i = ul.row - 1; i < ul.row + h + 1; i++) {
            img_[i][ul.col - 1] = 128;
            img_[i][ul.col + w] = 128;
        }
        for (int j = ul.col - 1; j < ul.col + w + 1; j++) {
            img_[ul.row - 1][j] = 128;
            img_[ul.row + h][j] = 128;
        }
    }
    writeGSBMP(filename, img_, h_, w_);
}

// TO DO:
// Add other (helper) function definitions here

DigitBlob NumImg::createDigitBlob(bool **explored, int pr, int pc) {
    // Arrays to help produce neighbors easily in a loop
    // by encoding the **change** to the current location.
    // Goes in order N, NW, W, SW, S, SE, E, NE
    int neighbor_row[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int neighbor_col[8] = {0, -1, -1, -1, 0, 1, 1, 1};

    // Add your code here

    // Store width and height variables
    int maxC = pc, minC = pc, maxR = pr, minR = pr;

    // store first location to search using top row and column
    Location curr = Location(pr, pc);

    // initialize deque and add first location
    deque<Location> q;
    q.push_back(curr);

    // Iterate through the locations in the queue
    while (!q.empty()) {

        // Move to next location in the queue
        curr = q.front();

        // iterate through each neighbor pixel
        for (int i = 0; i < 8; ++i) {

            // Store next neighbor
            int r = curr.row + neighbor_row[i];
            int c = curr.col + neighbor_col[i];

            // check that neighbor is not explored & black
            if ((!explored[r][c]) && (img_[r][c] == 0)) {

                // mark this neighbor as explored
                explored[r][c] = true;

                // add this neighbor to the queue to continue searching
                q.push_back(Location(r, c));

                // update widths and heights if nec
                if (r < minR) minR = r;
                if (r > maxR) maxR = r;
                if (c < minC) minC = c;
                if (c > maxC) maxC = c;
            }

        } // repeat for all 8 neighbors

        // Remove location just explored
        q.pop_front();

    }

    // Return new DigitBlob with initialized values
    return DigitBlob(img_, Location(minR, minC), maxR - minR + 1, maxC - minC + 1);

}
