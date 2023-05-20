#include "Coordinator.h"

class Leader : public Coordinator
{
private:
    int N;
public:
    Leader(int rank, int N, int error) : Coordinator(rank, error)
    {
        this->N = N;
    }
    void Start() override;
private:
    void PrintV();
};