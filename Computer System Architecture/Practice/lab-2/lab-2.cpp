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
    const size_t num_of_oper_ = 10000;
    std::string pmodel_;
    size_t ins_count_;
    std::string filename_;
    std::string operand_type_;
    std::string task_;
    std::string opt_;
    double *time_;
    double av_time_;
    double *abs_error_;
    double *rel_error_;
    double *task_perf_;

public:
    Benchmark(
        const std::string pmodel = "???",
        const size_t ins_count = 10,
        const std::string operand_type = "double",
        const std::string task = "sin",
        const std::string opt = "None",
        const std::string filename = "res.csv")
        : pmodel_(pmodel), ins_count_(ins_count), filename_(filename),
          operand_type_(operand_type), task_(task), opt_(opt) {
        time_ = new double[ins_count_];
        av_time_ = 0;
        abs_error_ = new double[ins_count_];
        rel_error_ = new double[ins_count_];
        task_perf_ = new double[ins_count_];
    }

    ~Benchmark() {
        delete[] time_;
        delete[] abs_error_;
        delete[] rel_error_;
        delete[] task_perf_;
    }

    void launch() {
        for (size_t i = 0; i < ins_count_; ++i) {
            time_[i] = execute_task();
            av_time_ += time_[i];
        }
        av_time_ /= ins_count_;
        for (size_t i = 0; i < ins_count_; ++i) {
            abs_error_[i] = fabs(av_time_ - time_[i]);
            rel_error_[i] = (abs_error_[i] / time_[i]) * 100;
            task_perf_[i] = num_of_oper_ / time_[i];
        }
    }

    void write() {
        std::ofstream outfile(filename_, std::ios::app);
        if (outfile) {
            // outfile <<
            // "PModel,Task,OpType,Opt,InsCount,Timer,Time,LNum,AvTime,"
            //            "AbsError,RelError,TaskPerf\n";
            for (size_t i = 0; i < ins_count_; ++i) {
                outfile << pmodel_ << "," << task_ << "," << operand_type_
                        << "," << opt_ << "," << ins_count_ << ",gettimeofday,"
                        << time_[i] << "," << (i + 1) << "," << av_time_ << ","
                        << abs_error_[i] << "," << rel_error_[i] << "%,"
                        << task_perf_[i] << "\n";
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

    double execute_task() {
        double tmp = 0;
        double res_time = 0;
        if (task_ == "sin") {
            if (operand_type_ == "double") {
                for (size_t i = 0; i < num_of_oper_; ++i) {
                    tmp = wtime();
                    sin(custom_rand<double>());
                    tmp = wtime() - tmp;
                    res_time += tmp;
                }
            } else {
                for (size_t i = 0; i < num_of_oper_; ++i) {
                    tmp = wtime();
                    sin(custom_rand<int>());
                    tmp = wtime() - tmp;
                    res_time += tmp;
                }
            }
        } else if (task_ == "cos") {
            if (operand_type_ == "double") {
                for (size_t i = 0; i < num_of_oper_; ++i) {
                    tmp = wtime();
                    cos(custom_rand<double>());
                    tmp = wtime() - tmp;
                    res_time += tmp;
                }
            } else {
                for (size_t i = 0; i < num_of_oper_; ++i) {
                    tmp = wtime();
                    cos(custom_rand<int>());
                    tmp = wtime() - tmp;
                    res_time += tmp;
                }
            }
        } else {
            if (operand_type_ == "double") {
                for (size_t i = 0; i < num_of_oper_; ++i) {
                    tmp = wtime();
                    log(custom_rand<double>());
                    tmp = wtime() - tmp;
                    res_time += tmp;
                }
            } else {
                for (size_t i = 0; i < num_of_oper_; ++i) {
                    tmp = wtime();
                    log(custom_rand<int>());
                    tmp = wtime() - tmp;
                    res_time += tmp;
                }
            }
        }
        return res_time;
    }
};

int main(int argc, char **argv) {
    srand(time(nullptr));
    CLI::App app("Benchmark");
    size_t count;
    std::string task;
    std::string op_type;
    std::string opt;
    std::string model;
    std::string filename;
    app.add_option(
           "-n,--num",
           count,
           "number of executions\nin the range 10 to 1000000")
        ->check(CLI::Range(10, 1000000000));
    app.add_option("-t,--task", task, "set task\nvalues: sin | cos | log |")
        ->check(CLI::IsMember({"sin", "cos", "log"}));
    app.add_option("-o,--optype", op_type, "select operand type: int or double")
        ->check(CLI::IsMember({"int", "double"}));
    app.add_option("--opt", opt, "select optimisation")
        ->check(CLI::IsMember({"None", "O0", "O1", "O2", "O3"}));
    app.add_option("-m,--model", model, "processor model");
    app.add_option("-f,--file", filename, "filename to output results");
    CLI11_PARSE(app, argc, argv);
    Benchmark benchmark(model, count, op_type, task, opt, filename);
    benchmark.launch();
    benchmark.write();
    return 0;
}