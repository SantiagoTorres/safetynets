#ifndef UTIL_H
#define UTIL_H

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>

#include <sstream>
#include <vector>

using namespace std;

struct runtime {
    double unverifiable;
    double prover;
    double verifier;
};

runtime update_time(runtime t, runtime nt);
runtime set_time(runtime t, double ut, double pt, double vt);

vector <int*> read_architecture_from_file(const char* filename);


#endif // UTIL_H
