#include "mpi.h"
#include "Worker.h"
#include "Coordinator.h"
#include "Leader.h"

#define MIN_COORD 1
#define MAX_COORD 3
#define LEADER 0

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == LEADER)
    {
        Leader leader(rank, std::stoi(argv[1]), std::stoi(argv[2]));
        leader.Start();
    }
    else if (rank >= MIN_COORD && rank <= MAX_COORD)
    {
        Coordinator coordinator(rank, std::stoi(argv[2]));
        coordinator.Start();
    }
    else
    {
        Worker worker(rank);
        worker.Start();
    }

    MPI_Finalize();
    return 0;
}