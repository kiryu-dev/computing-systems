#include "CLI11.hpp"
#include <iostream>
#include <math.h>
#include <omp.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

struct ThreadData {
    double *A_ = nullptr;
    double *B_ = nullptr;
    double *C_ = nullptr;
    double time_ = 0.0;
    int size_ = 0;
    pthread_t thread_id_ = 0;
    int threadno_ = 0;
    int nthreads_ = 0;
};

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double get_rand(unsigned int *seed) { return (double)rand_r(seed) / RAND_MAX; }

void write(
    const std::string &filename,
    int size,
    int threads,
    double time,
    const std::string &mode) {
    std::ofstream outfile(filename, std::ios::app);
    if (outfile) {
        outfile << mode << "," << size << "," << threads << "," << time << "\n";
    }
    outfile.close();
}

void *dgemm_pthread(void *thread_arg) {
    ThreadData *t_data = (ThreadData *)thread_arg;
    std::cout << "Thread " << t_data->threadno_ << " is working now\n";
    t_data->time_ = -wtime();
    int items_per_thread = t_data->size_ / t_data->nthreads_;
    int lb = t_data->threadno_ * items_per_thread;
    int ub = (t_data->threadno_ == t_data->nthreads_ - 1)
        ? (t_data->size_ - 1)
        : (lb + items_per_thread - 1);
    unsigned int seed = t_data->threadno_;
    for (int i = lb; i <= ub; ++i) {
        for (int j = 0; j < t_data->nthreads_; ++j) {
            *(t_data->A_ + i * t_data->size_ + j) = get_rand(&seed);
            *(t_data->B_ + i * t_data->size_ + j) = get_rand(&seed);
            *(t_data->C_ + i * t_data->size_ + j) = 0.0;
        }
    }
    for (int i = lb; i <= ub; ++i) {
        for (int k = 0; k < t_data->size_; ++k) {
            for (int j = 0; j < t_data->size_; ++j) {
                *(t_data->C_ + i * t_data->size_ + j) +=
                    *(t_data->A_ + i * t_data->size_ + k) *
                    *(t_data->B_ + k * t_data->size_ + j);
            }
        }
    }
    t_data->time_ += wtime();
    std::cout << "Thread " << t_data->threadno_
              << " has finished calculations\n";
    return nullptr;
}

void matrix_vector_product_omp(
    double *a, double *b, double *c, int size, int threads) {
#pragma omp parallel num_threads(threads)
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        std::cout << "Thread " << threadid << " is working now\n";
        int items_per_thread = size / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (size - 1)
                                            : (lb + items_per_thread - 1);
        unsigned int seed = omp_get_thread_num();
        for (int i = lb; i <= ub; ++i) {
            for (int j = 0; j < size; ++j) {
                *(a + i * size + j) = get_rand(&seed);
                *(b + i * size + j) = get_rand(&seed);
                *(c + i * size + j) = 0.0;
            }
        }
        for (int i = lb; i <= ub; ++i) {
            for (int k = 0; k < size; ++k) {
                for (int j = 0; j < size; ++j) {
                    *(c + i * size + j) +=
                        *(a + i * size + k) * *(b + k * size + j);
                }
            }
        }
        std::cout << "Thread " << threadid << " has finished calculations\n";
    }
}

void dgemm(double *a, double *b, double *c, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            a[i * size + j] = rand() / RAND_MAX;
            b[i * size + j] = rand() / RAND_MAX;
            c[i * size + j] = 0.0;
        }
    }
    for (int i = 0; i < size; ++i) {
        for (int k = 0; k < size; ++k) {
            for (int j = 0; j < size; ++j) {
                c[i * size + j] += a[i * size + k] * b[k * size + j];
            }
        }
    }
}

int main(int argc, char **argv) {
    srand(time(nullptr));
    CLI::App app("Parallel DGEMM");
    int size;
    int threads;
    std::string filename;
    std::string mode;
    app.add_option(
           "-s,--size",
           size,
           "number of rows/columns\nin the range 1 to 1000000")
        ->check(CLI::Range(1, 1000000));
    app.add_option("-t,--threads", threads, "number of threads")
        ->check(CLI::Range(1, 8));
    app.add_option("-f,--file", filename, "filename to output results");
    app.add_option("-m,--mode", mode, "how to use multithreading")
        ->check(CLI::IsMember({"posix", "openmp", "default"}));
    CLI11_PARSE(app, argc, argv);
    auto *A = new double[size * size];
    auto *B = new double[size * size];
    auto *C = new double[size * size];
    double t;
    if (mode == "openmp") {
        t = -wtime();
        matrix_vector_product_omp(A, B, C, size, threads);
        t += wtime();
    } else if (mode == "posix") {
        ThreadData *t_data = new ThreadData[threads];
        for (int i = 0; i < threads; ++i) {
            t_data[i].threadno_ = i;
            t_data[i].nthreads_ = threads;
            t_data[i].A_ = A;
            t_data[i].B_ = B;
            t_data[i].C_ = C;
            t_data[i].size_ = size;
        }
        for (int i = 1; i < threads; ++i) {
            if (pthread_create(
                    &t_data[i].thread_id_, NULL, dgemm_pthread, &t_data[i]) !=
                0) {
                std::cout << "error: thread can't be created\n";
                return -1;
            }
        }
        dgemm_pthread(&t_data[0]);
        for (int i = 1; i < threads; ++i) {
            pthread_join(t_data[i].thread_id_, nullptr);
        }
        t = t_data->time_;
        delete[] t_data;
    } else if (mode == "default") {
        t = -wtime();
        dgemm(A, B, C, size);
        t += wtime();
        threads = 1;
    }
    write(filename, size, threads, t, mode);
    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}