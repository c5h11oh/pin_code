#include "persistence.h"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>
using namespace std;

const char DATA_LOC[] = "/home/c5h11oh/cpp_playground/2023-1-Crystal/data.db";
// type of function that determines whether the pin code is good.
// returns true if the argument is a good pin code in this functions perspective.
typedef bool (*good_pin_code_func_t)(pin_code_t); 

random_device rd;
mt19937 gen(rd());

// functions that determine whether the pin code is good
    // criteria 1: pin codes of which difference between each pair of digits are the same
good_pin_code_func_t difference_in_digit_pairs = [](pin_code_t pin_code) {
    short differences[3];
    differences[0] = pin_code % 10; // the lowest digit
    pin_code /= 10;
    differences[1] = pin_code % 10;
    pin_code /= 10;
    differences[2] = pin_code % 10;
    pin_code /= 10; // the highest digit
    differences[0] = (differences[0] - differences[1] + 10) % 10;
    differences[1] = (differences[1] - differences[2] + 10) % 10;
    differences[2] = (differences[2] - pin_code       + 10) % 10;

    if (differences[0] == differences[1] && differences[1] == differences[2]) {
        return false;
    }

    return true;
};

// copy all valid pin codes to "unused pin codes" to let them ready for use. 
// called either when the good pin codes are calculated (initialize_data) 
// or when the "unused pin code count" reaches zero and we are having a new round of pin code generation.
inline void reset_data(persistence_t &data) {
    memcpy(data.unused_pin_codes, data.good_pin_codes, sizeof(pin_code_t) * PIN_CODE_RANGE);
    data.unused_pin_codes_count = data.good_pin_codes_count;
}

// given definitions of good pin code (functions), generate good pin codes in data.
void initialize_data(persistence_t &data, const vector<good_pin_code_func_t>& functions) {
    data.good_pin_codes_count = 0;

    // // 1. record all bad pin codes
    // unordered_set<unsigned short> bad_pin_codes;
    // {
    //     char bits[4];
    //     for (auto i = 0; i < 10; ++i) {
    //         bits[3] = i;
    //         for (auto j = 0; j < 10; ++j) {
    //             bits[2] = (bits[3] + j) % 10;
    //             bits[1] = (bits[2] + j) % 10;
    //             bits[0] = (bits[1] + j) % 10;
    //             unsigned short bad_pin_code =
    //                 bits[3] * 1000 + bits[2] * 100 + bits[1] * 10 + bits[0];
    //             bad_pin_codes.insert(bad_pin_code);
    //         }
    //     }
    // }
    // // one can amend more criterias here.

    // evaluate and save all valid pin codes to "data"
    for (pin_code_t i = 0; i < 10000; ++i) {
        // todo: change to logical-and all results from vector
        bool valid_pin_code = true;
        for (const auto& func : functions) {
            valid_pin_code &= func(i);
        }  
        // if (bad_pin_codes.count(i)) {
        //     continue;
        // }

        if (valid_pin_code) {
            data.good_pin_codes[data.good_pin_codes_count++] = i;
        }
    }

    // copy the computed results to "unused_pin_codes"
    reset_data(data);
}

// randomly pick a pin code. remove the selected pin code from data.unused_pin_codes. 
pin_code_t pick_pin_code_lottery(persistence_t &data) {
    
    uniform_int_distribution<> dis(0, data.unused_pin_codes_count - 1);

    auto index = dis(gen);
    pin_code_t picked_pin_code = data.unused_pin_codes[index];
    data.unused_pin_codes[index] = data.unused_pin_codes[--data.unused_pin_codes_count];
    return picked_pin_code;
}

int main() {
    persistence_t data;

    fstream file(DATA_LOC, fstream::in | fstream::out);

    // check if the persistent file exists
    if (file.is_open() == false) {
        // the file does not exist. create the file.
        file.close();
        file.open(DATA_LOC, fstream::out);
    }

    // todo: incorrect file size
    file.seekg(ios::end);
    auto filesize = file.tellg();
    file.seekg(ios::beg);

    if (filesize >= sizeof(persistence_t)) {
        file.read(reinterpret_cast<char *>(&data), sizeof(persistence_t));
        if (data.unused_pin_codes_count == 0) {
            reset_data(data);
        }
    } else {
        vector<good_pin_code_func_t> functions;
        // push_back functions that determines good pin codes here
        functions.push_back(difference_in_digit_pairs);
        
        // generate good pin codes
        initialize_data(data, functions);
    }

    // randomly select an unused pin code and emit it
    cout << setfill('0') << setw(4) << pick_pin_code_lottery(data) << endl;

    // persist the modified data, so users does not get the same pin code again before exhausting other pin codes.
    file.write(reinterpret_cast<char *>(&data), sizeof(persistence_t));

    return 0;
}