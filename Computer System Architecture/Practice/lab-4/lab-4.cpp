#include "CLI11.hpp"
#include <iostream>
#include <math.h>
#include <sys/time.h>
#include <time.h>

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void write(
    const std::string &filename, size_t size, size_t block_size, double time) {
    std::ofstream outfile(filename, std::ios::app);
    if (outfile) {
        std::string dgemm_ver_name;
        if (block_size == 0) {
            dgemm_ver_name = "dgemm";
        } else if (block_size == 1) {
            dgemm_ver_name = "dgemm_opt_1";
        } else {
            dgemm_ver_name = "dgemm_opt_2";
        }
        outfile << dgemm_ver_name << "," << size << "," << block_size << ","
                << time << "\n";
    }
    outfile.close();
}

void dgemm(double *a, double *b, double *c, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            for (size_t k = 0; k < size; ++k) {
                c[i * size + j] += a[i * size + k] * b[k * size + j];
            }
        }
    }
}

void dgemm_opt_1(double *a, double *b, double *c, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t k = 0; k < size; ++k) {
            for (size_t j = 0; j < size; ++j) {
                c[i * size + j] += a[i * size + k] * b[k * size + j];
            }
        }
    }
}

void dgemm_opt_2(
    double *a, double *b, double *c, size_t size, size_t block_size) {
    size_t i, j, k;
    size_t i0, j0, k0;
    double *a0, *b0, *c0;
    for (i = 0; i < size; i += block_size) {
        for (j = 0; j < size; j += block_size) {
            for (k = 0; k < size; k += block_size) {
                for (i0 = 0, c0 = (c + i * size + j), a0 = (a + i * size + k);
                     i0 < block_size;
                     ++i0, c0 += size, a0 += size) {
                    for (k0 = 0, b0 = (b + k * size + j); k0 < block_size;
                         ++k0, b0 += size) {
                        for (j0 = 0; j0 < block_size; ++j0) {
                            c0[j0] += a0[k0] * b0[j0];
                        }
                    }
                }
            }
        }
    }
}

void init_matrix(double *a, double *b, double *c, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            a[i * n + j] = rand() % 10;
            b[i * n + j] = rand() % 10;
            c[i * n + j] = 0.0;
        }
    }
}

int main(int argc, char **argv) {
    srand(time(nullptr));
    CLI::App app("DGEMM");
    size_t size;
    size_t block_size;
    std::string filename;
    app.add_option(
           "-s,--size",
           size,
           "number of rows/columns\nin the range 10 to 1000000")
        ->check(CLI::Range(10, 1000000000));
    app.add_option("-b,--block-size", block_size, "block size")
        ->check(
            CLI::IsMember({0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024}));
    app.add_option("-f,--file", filename, "filename to output results");
    CLI11_PARSE(app, argc, argv);
    auto *A = new double[size * size];
    auto *B = new double[size * size];
    auto *C = new double[size * size];
    init_matrix(A, B, C, size);
    auto t = wtime();
    if (block_size == 0) {
        dgemm(A, B, C, size);
    } else if (block_size == 1) {
        dgemm_opt_1(A, B, C, size);
    } else {
        dgemm_opt_2(A, B, C, size, block_size);
    }
    t = wtime() - t;
    write(filename, size, block_size, t);
    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}