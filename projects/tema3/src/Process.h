#pragma once
#include "mpi.h"
#include <vector>

class Process
{
protected:
    int rank;
    std::vector<int> topology[4];
public:
    Process(int rank);
    ~Process();
    virtual void Start() = 0;
protected:
    void PrintTopology();
    void SendMessage(void* message, int size, MPI_Datatype data_type, int destination, int tag);
};