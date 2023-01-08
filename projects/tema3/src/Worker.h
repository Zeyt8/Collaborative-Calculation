#include "Process.h"

#define NR_COORDINATORS 4

class Worker : public Process
{
private:
    int coordinator = -1;
    int* V;
public:
    Worker(int rank);
    ~Worker();
    void Start() override;
private:
    /// @brief Receive fromTag's workers from coordinator
    /// @param fromTag The tag whose workers to receive
    void ReceiveTopology(int fromTag);
};