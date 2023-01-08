#include "Process.h"
#include <iostream>

Process::Process(int rank)
{
    this->rank = rank;
}

Process::~Process()
{
}

void Process::PrintTopology()
{
    std::cout << rank << " -> ";
    for (int i = 0; i < 4; i++)
    {
        std::cout << i << ":";
        for (uint32_t j = 0; j < topology[i].size(); j++)
        {
            std::cout << topology[i][j];
            if (j != topology[i].size() - 1)
            {
                std::cout << ",";
            }
        }
        std::cout << " ";
    }
    std::cout << std::endl << std::flush;
}

void Process::SendMessage(void* message, int size, MPI_Datatype data_type, int destination, int tag)
{
    MPI_Send(message, size, data_type, destination, tag, MPI_COMM_WORLD);
    std::cout << "M(" << rank << "," << destination << ")" << std::endl << std::flush;
}
