#include "Leader.h"

void Leader::Start()
{
    Coordinator::Start();
    V = new int[N];
    for (int i = 0; i < N; i++)
    {
        V[i] = N - i - 1;
    }
    int chunk = N * workers.size() / totalWorkers;
    // Send rest of V to left coordinator
    SendMessage(V + chunk, N - chunk, MPI_INT, leftCoordinator, 0);
    // Send V to workers
    ScatterV(chunk);
    // Receive V from workers
    int index = 0;
    for (uint32_t i = 0; i < workers.size(); i++)
    {
        MPI_Status status;
        MPI_Probe(workers[i], 0, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_INT, &count);
        MPI_Recv(V + index, count, MPI_INT, workers[i], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        index += count;
    }
    // Receive V from other coordinators
    for (int i = NR_COORDINATORS - 1; i > 0; i--)
    {
        MPI_Status status;
        MPI_Probe(leftCoordinator, i, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_INT, &count);
        MPI_Recv(V + index, count, MPI_INT, leftCoordinator, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        index += count;
    }
    PrintV();
}

void Leader::PrintV()
{
    std::cout << "Rezultat: ";
    for (int i = 0; i < N; i++)
    {
        std::cout << V[i] << " ";
    }
}
