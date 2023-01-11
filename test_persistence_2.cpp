#include "persistence.h"
#include <fstream>
#include <iostream>
using namespace std;

int main() {
    persistence_t bitmap;
    ifstream file("database.db");
    file.read(reinterpret_cast<char *>(&bitmap), sizeof(persistence_t));
    file.close();
    cout << bitmap.bits[0] << endl;
    cout << bitmap.bits[1] << endl;
    return 0;
}