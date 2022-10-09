#include "CLI11.hpp"
#include <iostream>
#include <math.h>
#include <sys/time.h>
#include <time.h>

template <typename T> T custom_rand() {
    const T min = 0;
    const T max = RAND_MAX;
    double f = (double)random() / RAND_MAX;
    return (T)(min + f * (max - min));
}

class Benchmark {
private:
    size_t count_;
    std::string memory_type_;
    size_t block_size_;
    std::string element_type_;
    std::string filename_;
    size_t buffer_size_;
    // size_t launch_num_;
    double *write_time_;
    double average_write_time_;
    double write_bandwidth_;
    double *abs_error_write_;
    double *rel_error_write_;
    double *read_time_;
    double average_read_time_;
    double read_bandwidth_;
    double *abs_error_read_;
    double *rel_error_read_;

public:
    Benchmark(
        const size_t count = 10,
        const std::string memory_type = "RAM",
        const size_t block_size = 1024,
        const std::string element_type = "double",
        const std::string filename = "res.csv")
        : count_(count), memory_type_(memory_type), block_size_(block_size),
          element_type_(element_type), filename_(filename) {
        write_time_ = new double[count_];
        average_write_time_ = 0;
        write_bandwidth_ = 0;
        abs_error_write_ = new double[count_];
        rel_error_write_ = new double[count_];
        read_time_ = new double[count_];
        average_read_time_ = 0;
        read_bandwidth_ = 0;
        abs_error_read_ = new double[count_];
        rel_error_read_ = new double[count_];
    }

    ~Benchmark() {
        delete[] write_time_;
        delete[] abs_error_write_;
        delete[] rel_error_write_;
        delete[] read_time_;
        delete[] abs_error_read_;
        delete[] rel_error_read_;
    }

    void launch() {
        if (memory_type_ == "RAM") {
            buffer_size_ = 1;
            if (element_type_ == "double") {
                execute_task<double>("None");
            } else {
                execute_task<int>("None");
            }
        } else if (memory_type_ == "HDD" || memory_type_ == "SSD") {
            buffer_size_ = block_size_;
            if (element_type_ == "double") {
                execute_task<double>("test.txt");
            } else {
                execute_task<int>("test.txt");
            }
        } else {
            buffer_size_ = block_size_;
            auto path = get_command_output(
                "lsblk | grep /media | awk '{printf $NF\"/test.txt\"}'");
            if (element_type_ == "double") {
                execute_task<double>(path);
            } else {
                execute_task<int>(path);
            }
        }
    }

    void write() {
        std::ofstream outfile(filename_, std::ios_base::out);
        if (outfile) {
            outfile
                << "MemoryType,BlockSize,ElementType,BufferSize,LaunchNum,"
                   "Timer,WriteTime,AverageWriteTime,WriteBandwidth,AbsError("
                   "write),RelError(write),ReadTime,AverageReadTime,"
                   "ReadBandwidth,AbsError(read),RelError(read)\n";
            for (size_t i = 0; i < count_; ++i) {
                outfile << memory_type_ << "," << block_size_ << ","
                        << element_type_ << "," << buffer_size_ << ","
                        << (i + 1) << ",gettimeofday," << write_time_[i] << ","
                        << average_write_time_ << "," << write_bandwidth_ << ","
                        << abs_error_write_[i] << "," << rel_error_write_[i]
                        << "," << read_time_[i] << "," << average_read_time_
                        << "," << read_bandwidth_ << "," << abs_error_read_[i]
                        << "," << rel_error_read_[i] << "\n";
            }
        }
        outfile.close();
    }

private:
    double wtime() {
        struct timeval t;
        gettimeofday(&t, NULL);
        return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
    }

    template <typename T> void execute_task(const std::string &path) {
        int size = block_size_ / sizeof(T);
        if (path == "None") {
            for (size_t i = 0; i < count_; ++i) {
                T *arr = new T[size];
                for (size_t j = 0; j < size; ++j) {
                    auto tmp = custom_rand<T>();
                    double t = wtime();
                    arr[j] = tmp;
                    write_time_[i] += wtime() - t;
                }
                for (size_t j = 0; j < size; ++j) {
                    double t = wtime();
                    auto tmp = arr[j];
                    read_time_[i] += wtime() - t;
                }
                average_write_time_ += write_time_[i];
                average_read_time_ += read_time_[i];
                delete[] arr;
            }
        } else {
            FILE *file;
            for (size_t i = 0; i < count_; ++i) {
                T *arr = new T[size];
                if ((file = fopen(path.c_str(), "w")) != nullptr) {
                    double t = wtime();
                    fwrite(arr, sizeof(T), size, file);
                    write_time_[i] += wtime() - t;
                    fclose(file);
                } else {
                    std::cout << "Cannot open" << path << " for write\n";
                    exit(1);
                }
                if ((file = fopen(path.c_str(), "r")) != nullptr) {
                    double t = wtime();
                    fread(arr, sizeof(T), size, file);
                    read_time_[i] += wtime() - t;
                    fclose(file);
                } else {
                    std::cout << "Cannot open" << path << " for write\n";
                    exit(1);
                }
                average_write_time_ += write_time_[i];
                average_read_time_ += read_time_[i];
                delete[] arr;
            }
        }
        average_write_time_ /= count_;
        write_bandwidth_ = block_size_ / average_write_time_;
        average_read_time_ /= count_;
        read_bandwidth_ = block_size_ / average_read_time_;
        for (size_t i = 0; i < count_; ++i) {
            abs_error_write_[i] = fabs(average_write_time_ - write_time_[i]);
            rel_error_write_[i] = (abs_error_write_[i] / write_time_[i]) * 100;
            abs_error_read_[i] = fabs(average_read_time_ - read_time_[i]);
            rel_error_read_[i] = (abs_error_read_[i] / read_time_[i]) * 100;
        }
    }

    static std::string get_command_output(const char *cmd) {
        char buffer[128];
        std::string result = "";
        FILE *pipe = popen(cmd, "r");
        if (pipe != nullptr) {
            while (fgets(buffer, sizeof buffer, pipe) != nullptr)
                result += buffer;
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
    std::string element_type;
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
    ;
    app.add_option(
           "-e,--element-type",
           element_type,
           "select element type: int or double")
        ->check(CLI::IsMember({"int", "double"}));
    app.add_option("-f,--file", filename, "filename to output results");
    CLI11_PARSE(app, argc, argv);
    Benchmark benchmark(count, memory_type, block_size, element_type, filename);
    benchmark.launch();
    benchmark.write();
    return 0;
}