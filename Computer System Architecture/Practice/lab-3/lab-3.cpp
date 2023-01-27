#include "CLI11.hpp"
#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
#include <sys/time.h>
#include <time.h>

class Benchmark {
private:
    char buffer_write_[1024 * 1024];
    char buffer_read_[1024 * 1024];
    size_t count_;
    std::string memory_type_;
    size_t block_size_;
    std::string filename_;
    size_t buffer_size_;
    double *write_time_;
    double *average_write_time_;
    double *write_bandwidth_;
    double *abs_error_write_;
    double *rel_error_write_;
    double *read_time_;
    double *average_read_time_;
    double *read_bandwidth_;
    double *abs_error_read_;
    double *rel_error_read_;

    const size_t step_ = 4194304;

public:
    Benchmark(
        const size_t count = 10,
        const std::string memory_type = "RAM",
        const size_t block_size = 1024,
        const std::string filename = "res.csv")
        : count_(count), memory_type_(memory_type), block_size_(block_size),
          filename_(filename) {
        write_time_ = new double[count_];
        average_write_time_ = new double[count_];
        write_bandwidth_ = new double[count_];
        abs_error_write_ = new double[count_];
        rel_error_write_ = new double[count_];
        read_time_ = new double[count_];
        average_read_time_ = new double[count_];
        read_bandwidth_ = new double[count_];
        abs_error_read_ = new double[count_];
        rel_error_read_ = new double[count_];
        for (size_t i = 0; i < count_; ++i) {
            average_read_time_[i] = 0;
            average_write_time_[i] = 0;
        }
    }

    ~Benchmark() {
        delete[] write_time_;
        delete[] abs_error_write_;
        delete[] rel_error_write_;
        delete[] read_time_;
        delete[] abs_error_read_;
        delete[] rel_error_read_;
        delete[] write_bandwidth_;
        delete[] read_bandwidth_;
        delete[] average_read_time_;
        delete[] average_write_time_;
    }

    void launch() {
        buffer_size_ = block_size_ < 1024 * 1024 ? block_size_ : 1024 * 1024;
        if (memory_type_ == "RAM") {
            /*
            L1 cache: 128 Kb
            L2 cache: 1 MiB
            L3 cache: 6 MiB
            */
            execute_task("None");
        } else if (memory_type_ == "HDD" || memory_type_ == "SSD") {
            execute_task("hdd_or_ssd_testik.bin");
        } else {
            auto path =
                get_command_output("lsblk | grep /media | awk '{printf "
                                   "$NF\"/flash_enjoyer_testik.bin\"}'");
            execute_task(path);
        }
    }

    void write() {
        std::ofstream outfile(filename_, std::ios::app);
        if (outfile) {
            // outfile
            //     << "MemoryType,BlockSize,ElementType,BufferSize,LaunchNum,"
            //        "Timer,WriteTime,AverageWriteTime,WriteBandwidth,AbsError("
            //        "write),RelError(write),ReadTime,AverageReadTime,"
            //        "ReadBandwidth,AbsError(read),RelError(read)\n";
            for (size_t i = 0; i < count_; ++i) {
                outfile << memory_type_ << "," << block_size_ << ",char,"
                        << buffer_size_ << "," << (i + 1) << ",clock_gettime,"
                        << write_time_[i] << "," << average_write_time_[i]
                        << "," << write_bandwidth_[i] << ","
                        << abs_error_write_[i] << "," << rel_error_write_[i]
                        << "," << read_time_[i] << "," << average_read_time_[i]
                        << "," << read_bandwidth_[i] << ","
                        << abs_error_read_[i] << "," << rel_error_read_[i]
                        << "\n";
            }
        }
        outfile.close();
    }

private:
    void execute_task(const std::string &path) {
        if (path == "None") {
            for (size_t i = 0; i < count_; ++i) {
                timespec start_write, end_write, start_read, end_read;
                clock_gettime(CLOCK_MONOTONIC, &start_write);
                for (size_t j = 0; j < block_size_; j += buffer_size_) {
                    for (size_t k = 0; k < buffer_size_; ++k) {
                        buffer_write_[k] = 0;
                    }
                }
                clock_gettime(CLOCK_MONOTONIC, &end_write);
                write_time_[i] =
                    static_cast<double>(end_write.tv_sec - start_write.tv_sec) +
                    static_cast<double>(
                        end_write.tv_nsec - start_write.tv_nsec) *
                        1E-9;
                clock_gettime(CLOCK_MONOTONIC, &start_read);
                for (size_t j = 0; j < block_size_; j += buffer_size_) {
                    for (size_t k = 0; k < buffer_size_; ++k) {
                        buffer_read_[k] = buffer_write_[k];
                    }
                }
                clock_gettime(CLOCK_MONOTONIC, &end_read);
                read_time_[i] =
                    static_cast<double>(end_read.tv_sec - start_read.tv_sec) +
                    static_cast<double>(end_read.tv_nsec - start_read.tv_nsec) *
                        1E-9;
                for (size_t j = 0; j < i + 1; ++j) {
                    average_write_time_[i] += write_time_[j];
                    average_read_time_[i] += read_time_[j];
                }
            }
        } else {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(33, 126);
            for (size_t i = 0; i < buffer_size_; ++i) {
                buffer_write_[i] = dist(gen);
            }
            for (size_t i = 0; i < count_; ++i) {
                std::fstream file(path, std::ios_base::out);
                file.close();
                file.open(path, std::ios_base::out | std::ios_base::in);
                if (!file.is_open()) {
                    std::cout << "Can't open " << path << "\n";
                    exit(1);
                }
                timespec start_write, end_write, start_read, end_read;
                clock_gettime(CLOCK_MONOTONIC, &start_write);
                for (size_t j = 0; j < block_size_; j += buffer_size_) {
                    for (size_t k = 0; k < buffer_size_; ++k) {
                        file.put(buffer_write_[k]);
                    }
                }
                clock_gettime(CLOCK_MONOTONIC, &end_write);
                write_time_[i] =
                    static_cast<double>(end_write.tv_sec - start_write.tv_sec) +
                    static_cast<double>(
                        end_write.tv_nsec - start_write.tv_nsec) *
                        1E-9;
                clock_gettime(CLOCK_MONOTONIC, &start_read);
                for (size_t j = 0; j < block_size_; j += buffer_size_) {
                    for (size_t k = 0; k < buffer_size_; ++k) {
                        file.get(buffer_read_[k]);
                    }
                }
                clock_gettime(CLOCK_MONOTONIC, &end_read);
                read_time_[i] =
                    static_cast<double>(end_read.tv_sec - start_read.tv_sec) +
                    static_cast<double>(end_read.tv_nsec - start_read.tv_nsec) *
                        1E-9;
                for (size_t j = 0; j < i + 1; ++j) {
                    average_write_time_[i] += write_time_[j];
                    average_read_time_[i] += read_time_[j];
                }
                /* для экспериментов в лабораторной */
                // block_size_ += 4 * 1024 * 1024;
            }
            std::filesystem::remove(path);
        }
        for (size_t i = 0; i < count_; ++i) {
            // block_size_ = (4 * 1024 * 1024 + i * (4 * 1024 * 1024));
            average_write_time_[i] /= (i + 1);
            average_read_time_[i] /= (i + 1);
            write_bandwidth_[i] =
                (double)block_size_ / average_write_time_[i] / (1024 * 1024);
            read_bandwidth_[i] =
                (double)block_size_ / average_read_time_[i] / (1024 * 1024);
            abs_error_write_[i] = fabs(average_write_time_[i] - write_time_[i]);
            rel_error_write_[i] = (abs_error_write_[i] / write_time_[i]) * 100;
            abs_error_read_[i] = fabs(average_read_time_[i] - read_time_[i]);
            rel_error_read_[i] = (abs_error_read_[i] / read_time_[i]) * 100;
        }
    }

    static std::string get_command_output(const char *cmd) {
        char buffer[128];
        std::string result = "";
        FILE *pipe = popen(cmd, "r");
        if (pipe != nullptr) {
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                result += buffer;
            }
            pclose(pipe);
        } else {
            throw std::runtime_error("popen() failed!");
        }
        return result;
    }
};

int main(int argc, char **argv) {
    srand(time(nullptr));
    CLI::App app("Benchmark");
    size_t count;
    std::string memory_type;
    size_t block_size;
    std::string filename;
    app.add_option(
           "-n,--num",
           count,
           "number of executions\nin the range 10 to 1000000")
        ->check(CLI::Range(10, 1000000000));
    app.add_option(
           "-m,--memory-type",
           memory_type,
           "select memory type: RAM|HDD|SSD|flash")
        ->check(CLI::IsMember({"RAM", "HDD", "SSD", "flash"}));
    app.add_option(
           "-b,--block-size",
           block_size,
           "block size\nin the range 1 to 1073741824")
        ->check(CLI::Range(1, 1024 * 1024 * 1024));
    app.add_option("-f,--file", filename, "filename to output results");
    CLI11_PARSE(app, argc, argv);
    Benchmark benchmark(count, memory_type, block_size, filename);
    benchmark.launch();
    benchmark.write();
    return 0;
}