#include <iostream>
#include <string>
#include "bigint.h"

using namespace std;

// Write your implementation below


// convert string to BigInt
BigInt::BigInt(string s, int base) {

    // store base
    base_ = base;

    // iterate backwards through string (lsb to msb)
    for (int i = s.length() - 1; i > -1; i--) {

        // add values from pos 0 to end of vector
        if ((int) s[i] > 57) { // letters
            items_.push_back(((int) s[i]) - 55);
        } else { // numbers
            items_.push_back(((int) s[i]) - 48);
        }
    }
    removeLeadingZeroes();
}

// get string representation
string BigInt::to_string() const {

    // store return value
    string retVal;

    // add all digits to return value
    for (int i = items_.size() - 1; i > -1; i--) {

        // Add appropriate alphanumeric
        if (items_[i] > 10) {
            retVal += (char) (items_[i] - 10 + 'A');
        } else retVal += std::to_string(items_[i]);
    }

    // return the value
    return retVal;

}

// add another BigInt to this one
void BigInt::add(BigInt b) {

    // calculate size difference
    int difference = items_.size() - b.items_.size();
    int sum, otherItem, carry = 0, base = get_base();


    if (difference > 0) { // this bigint bigger

        // iterate through BIGGER bigint
        for (unsigned int i = 0; i < items_.size(); ++i) {

            // initialize other value based on validity
            if (i >= b.items_.size()) otherItem = 0;
            else otherItem = b.items_[i];

            // calculate new sum with previous carry
            sum = items_[i] + otherItem + carry;

            // if sum requires tallying, update carry & remainder
            if (sum >= base) {
                carry = 1;
                items_[i] = sum % base;
            } else {
                items_[i] = sum;
                carry = 0;
            }

            // if last item has carry, update the next value
            if ((i == items_.size() - 1) && (carry != 0)) {
                items_.push_back(1);
                break; // end loop since size increased
            }
        } // repeat for next digit

    } else { // other bigint bigger or equal

        // iterate through BIGGER or EQUAL bigint
        for (unsigned int i = 0; i < b.items_.size(); ++i) {

            // initialize other value based on validity
            if (i >= items_.size()) otherItem = 0;
            else otherItem = items_[i];

            // calculate new sum with previous carry, reset carry
            sum = b.items_[i] + otherItem + carry;

            // if sum requires tallying, update carry & remainder
            if (sum >= base) {
                carry = 1;
                // update existing or push new value
                if (i >= items_.size()) items_.push_back(sum % base);
                else items_[i] = sum % base;

            } else {
                // update existing or push new value
                if (i >= items_.size()) items_.push_back(sum);
                else items_[i] = sum;
                carry = 0;
            }

            // if last item has carry, update the next value
            if ((i == b.items_.size() - 1) && (carry != 0)) {
                items_.push_back(1);
                break; // end loop since size increased
            }
        } // repeat for next digit
    }
}

// accessor for private data member
int BigInt::get_base() const {
    return base_;
}

void BigInt::removeLeadingZeroes() {

    // iterate through the back 0's unless size = 0
    while ( (items_.back() == 0) && (items_.size() != 1) ){
        // delete item at back
        items_.pop_back();
    }

}

BigInt BigInt::operator+(const BigInt &rhs) const {

    // copy this, add rhs to it, return it
    BigInt temp = *this;
    temp.add(rhs);
    return temp;
}

bool BigInt::operator<(const BigInt &rhs) const {

    // copy current item into temporary variable
    BigInt temp = *this;

    // check size difference
    if (temp.items_.size() < rhs.items_.size()){
        return true;
    } else if (temp.items_.size() > rhs.items_.size()){
        return false;
    } else{

        // for equal size, iterate from MSB to LSB
        for (unsigned int i = 0; i < temp.items_.size(); ++i) {

            // store MSB position
            unsigned int val = temp.items_.size() - i - 1;

            // compare and return appropriately
            if (temp.items_[val] < rhs.items_[val]) {
                return true;
            } else if (temp.items_[val] > rhs.items_[val]) {
                return false;
            }
        }
    }

    // default return statement
    return false;
}

BigInt BigInt::operator-(const BigInt &rhs) const {

    BigInt temp = *this;

    // calculate size difference
    int difference = items_.size() - rhs.items_.size();
    int sum, otherItem, carry = 0;


    if (difference >= 0) { // this bigint bigger
        cout << "difference > 0" << endl;

        // iterate through BIGGER bigint
        for (unsigned int i = 0; i < items_.size(); ++i) {
            cout << " in da looop " << endl;

            // initialize other value based on validity
            if (i >= rhs.items_.size()) otherItem = 0;
            else otherItem = rhs.items_[i];

            cout << "Subtracting: " << items_[i] << " - " << otherItem << " + " << carry << endl;

            // calculate new difference with previous carry
            sum = items_[i] - otherItem + carry;
            cout << "sum = " << sum << endl;

            // handle negative sum
            if (sum < 0) {
                cout << "sum < 0" << endl;
                sum = (sum + 10) % 10;
                carry = -1;
                cout << "sum, carry:" << sum << "," << carry << endl;
                temp.items_[i] = sum;
                // handle positive sum
            } else {
                cout << "sum > 0" << endl;
                temp.items_[i] = sum;
            }
        }
    } else { // other bigint bigger or equal
        return rhs - temp;
    }

    temp.removeLeadingZeroes();
    return temp;
}

/*
int main() {


    BigInt b1("9"), b2("7");
    cout << b1.to_string() << " - " << b2.to_string() << ": " << endl;
    cout << endl << endl << (b1 - b2).to_string() << endl; // should print 203

    BigInt b3("9"), b4("7");
    cout << b3.to_string() << " - " << b4.to_string() << ": " << endl;
    cout << endl << endl << (b3 - b4).to_string() << endl; // should print 203

    BigInt b5("198"), b6("73");
    cout << b5.to_string() << " - " << b6.to_string() << ": " << endl;
    cout << endl << endl << (b5 - b6).to_string() << endl; // should print 203

    BigInt b7("9"), b8("9");
    cout << b7.to_string() << " - " << b8.to_string() << ": " << endl;
    cout << endl << endl << (b7 - b8).to_string() << endl; // should print 203

    BigInt b9("93"), b10("93");
    cout << b9.to_string() << " - " << b10.to_string() << ": " << endl;
    cout << endl << endl << (b9 - b10).to_string() << endl; // should print 203

    BigInt b11("123"), b12("98");
    cout << b11.to_string() << " - " << b12.to_string() << ": " << endl;
    cout << endl << endl << (b11 - b12).to_string() << endl; // should print 203

    BigInt b13("30000000000000000000000"), b14("21234567890123456789012");
    cout << b13.to_string() << " - " << b14.to_string() << ": " << endl;
    cout << endl << endl << (b13 - b14).to_string() << endl; // should print 203

    return 0;
}
*/