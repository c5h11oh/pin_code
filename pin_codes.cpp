#include "pin_codes.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>
using namespace std;

const filesystem::path DATA_LOC("data.db");

// functions that determine whether the pin code is good
// criteria 1: pin codes of which difference between each pair of digits are the same
good_pin_code_func_t difference_in_digit_pairs = [](pin_code_t pin_code) {
    short differences[PIN_CODE_DIGITS_COUNT];
    for (int i = 0; i < PIN_CODE_DIGITS_COUNT; ++i) {
        differences[i] = pin_code % 10;
        pin_code /= 10;
    }

    for (int i = 0; i < PIN_CODE_DIGITS_COUNT - 1; ++i) {
        differences[i] = (differences[i] - differences[i + 1] + 10) % 10;
    }

    for (int i = 0; i < PIN_CODE_DIGITS_COUNT - 2; ++i) {
        if (differences[i] != differences[i + 1]) {
            return true;
        }
    }

    return false;
};

// criteria 2: no three digits are the same
good_pin_code_func_t three_identical_digits = [](pin_code_t pin_code) {
    vector<char> counter(10, 0);
    for (int i = 0; i < PIN_CODE_DIGITS_COUNT; ++i) {
        if (++counter[pin_code % 10] == 3) {
            return false;
        }
        pin_code /= 10;
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
void initialize_data(persistence_t &data, const vector<good_pin_code_func_t> &functions) {
    data.good_pin_codes_count = 0;

    // evaluate and save all valid pin codes to "data"
    for (pin_code_t i = 0; i < PIN_CODE_RANGE; ++i) {
        bool valid_pin_code = true;
        for (const auto &func : functions) {
            valid_pin_code &= func(i);
        }

        if (valid_pin_code) {
            data.good_pin_codes[data.good_pin_codes_count++] = i;
        }
    }

    // copy the computed results to "unused_pin_codes"
    reset_data(data);
}

// randomly pick a pin code. remove the selected pin code from data.unused_pin_codes.
pin_code_t pick_pin_code_lottery(persistence_t &data) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, data.unused_pin_codes_count - 1);

    auto index = dis(gen);
    pin_code_t picked_pin_code = data.unused_pin_codes[index];
    data.unused_pin_codes[index] = data.unused_pin_codes[--data.unused_pin_codes_count];
    return picked_pin_code;
}

int main() {
    persistence_t data;
    fstream file;
    unsigned int filesize = 0;

    if (filesystem::exists(DATA_LOC)) {
        try {
            filesize = filesystem::file_size(DATA_LOC);
        } catch (filesystem::filesystem_error &e) {
            cerr << "When checking persistent data file size, an error occured.\n"
                 << e.what() << endl;
            return 1;
        }
        file.open(DATA_LOC, fstream::in | fstream::out | fstream::binary);
    } else {
        file.open(DATA_LOC, fstream::out | fstream::binary);
    }

    if (filesize >= sizeof(persistence_t)) {
        file.read(reinterpret_cast<char *>(&data), sizeof(persistence_t));
        if (data.unused_pin_codes_count == 0) {
            reset_data(data);
        }
    } else {
        vector<good_pin_code_func_t> functions;
        // push_back functions that determines good pin codes here
        functions.push_back(difference_in_digit_pairs);
        functions.push_back(three_identical_digits);

        // generate good pin codes
        initialize_data(data, functions);
    }

    // randomly select an unused pin code and emit it
    pin_code_t picked_pin_code = pick_pin_code_lottery(data);
    
    // persist the modified data, so users does not get the same pin code again before exhausting other pin codes.
    file.seekp(0);
    file.write(reinterpret_cast<char *>(&data), sizeof(persistence_t));
    file.close();
    
    // outputs the picked pin code
    cout << setfill('0') << setw(4) << picked_pin_code << endl;

    return 0;
}
