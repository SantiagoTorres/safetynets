/*
 * util module
 *
 * This module contains utility functions for timekeeping, input-and output
 * and such.
 */
#include "util.h"

using namespace std;

runtime update_time(runtime t, runtime nt)
{
    t.unverifiable += nt.unverifiable;
    t.prover += nt.prover;
    t.verifier += nt.verifier;
    return t;
}

runtime set_time(runtime t, double ut, double pt, double vt)
{
    t.unverifiable = ut;
    t.prover = pt;
    t.verifier = vt;
    return t;
}

vector <int*> read_architecture_from_file(const char* filename)
{

    ifstream archfile(filename);

    string line;
    vector <int*> layers;

    // get batch size
    double batch;
    getline(archfile, line);
    stringstream(line) >> batch;
    batch = ceil(log2(batch));

    // read input size
    double prevl, currl;
    getline(archfile, line);
    stringstream(line) >> prevl;
    prevl = ceil(log2(prevl));

    // read layer sizes
    while (getline(archfile, line))
    {
        stringstream(line) >> currl;
        currl = ceil(log2(currl));
        int *n = new int[3];
        n[0] = batch;
        n[1] = prevl;
        n[2] = currl;
        layers.push_back(n);
        prevl = currl;
    }
    archfile.close();

    return layers;
}
