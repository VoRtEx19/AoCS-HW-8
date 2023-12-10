#include <iostream>

#include <ctime>
#include <pthread.h>

const unsigned long long arrSizes[]{100000000, 500000000, 100000000000};
unsigned int currArrSize;

double *A;
double *B;

const int threadNums[]{1, 2, 4, 8, 1000};
int currThreadNum;

void *func(void *param) {
    unsigned int shift = currArrSize / currThreadNum;
    unsigned long p = (*(int *) param) * shift;
    auto *sum = new double;
    *sum = 0;
    for (unsigned int i = p; i < p + shift; i++) {
        *sum += A[i] * B[i];
    }
    return (void *) sum;
}

int main() {
    std::cout
            << "# elems \t# threads\tFull time\tResult\t\tThread creation time"
            << std::endl;


    for (auto &arrSize: arrSizes) {
        currArrSize = arrSize;

        double res = 0.0;

        A = new double[currArrSize];
        if (A == nullptr) {
            std::cout << "Incorrect size of vector = " << currArrSize << "\n";
            return 1;
        }
        for (int i = 0; i < currArrSize; ++i) {
            A[i] = double(i + 1);
        }
        B = new double[currArrSize];
        if (B == nullptr) {
            std::cout << "Incorrect size of vector = " << currArrSize << "\n";
            return 1;
        }
        for (int i = 0; i < currArrSize; ++i) {
            B[i] = double(currArrSize - i);
        }

        for (auto &threadNum: threadNums) {
            currThreadNum = threadNum;
            pthread_t thread[currThreadNum];
            int number[currThreadNum];

            clock_t start_time = clock();

            for (int i = 0; i < currThreadNum; i++) {
                number[i] = i;
                pthread_create(&thread[i], nullptr, func, (void *) (number + i));
            }

            clock_t thread_started_time = clock();

            double *sum;
            for (unsigned long long i: thread) {
                pthread_join(i, (void **) &sum);
                res += *sum;
                delete sum;
            }

            clock_t end_time = clock();

            std::cout << currArrSize << '\t' << currThreadNum << "\t\t" << end_time - start_time << "\t\t"
                      << res
                      << '\t' << thread_started_time - start_time << std::endl;
        }

        delete[] A;
        delete[] B;
    }

    return 0;
}
