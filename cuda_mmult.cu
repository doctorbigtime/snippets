#include <iostream>
#include <cuda.h>

__global__ void mmult(double* A, double* B, double* C, int N)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if((row < N) && (col < N))
    {
        double dot = .0;
        for(int i = 0; i < N; ++i)
            dot += A[row*N + i] * B[col + i*N];
        C[row*N + col] = dot;
    }
}

#define CUDA_CHECK(stmt) do { \
    cudaError_t err = stmt; \
    if(err != cudaSuccess) { \
        std::cerr << "Failed to run statement: " << #stmt << std::endl; \
        return -1; \
    } \
} while(0)


extern bool do_cuda_mmult(double* A, double* B, double* C, int N)
{
    double *deviceA, *deviceB, *deviceC;
    int bytes = sizeof(double) * N * N;
    CUDA_CHECK(cudaMalloc((void**)&deviceA, bytes));
    CUDA_CHECK(cudaMalloc((void**)&deviceB, bytes));
    CUDA_CHECK(cudaMalloc((void**)&deviceC, bytes));

    CUDA_CHECK(cudaMemcpy(deviceA, A, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(deviceB, B, bytes, cudaMemcpyHostToDevice));
    //cudaMemcpy(deviceC, C, bytes, cudaMemcpyHostToDevice);

    static unsigned int constexpr TILE_WIDTH = 8;
    dim3 dimGrid{ (N-1)/TILE_WIDTH+1, (N-1)/TILE_WIDTH+1, 1u };
    dim3 dimBlock{ TILE_WIDTH, TILE_WIDTH, 1 };

    mmult<<< dimGrid, dimBlock >>>(deviceA, deviceB, deviceC, N);

    cudaThreadSynchronize();
    CUDA_CHECK(cudaMemcpy(C, deviceC, bytes, cudaMemcpyDeviceToHost));
    return true;
}

