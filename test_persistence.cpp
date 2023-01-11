#include "persistence.h"
#include <iostream>
#include <fstream>
using namespace std;

int store() {
    persistence_t bitmap;
    bitmap.bits[0] = 1345;
    bitmap.bits[1] = 7643;
    ofstream file("database.db", ios::binary | ios::trunc);
    // file.open(, ios::in | ios::out | ios::binary);
    file.write(reinterpret_cast<char *>(&bitmap), sizeof(persistence_t));
    file.close();

    return 0;
}

int load() {
    persistence_t bitmap;
    ifstream file("database.db");
    file.read(reinterpret_cast<char *>(&bitmap), sizeof(persistence_t));
    file.close();
    cout << bitmap.bits[0] << endl;
    cout << bitmap.bits[1] << endl;
    return 0;
}

int main() {
    store();
    // load();
    return 0;
}