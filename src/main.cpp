// krauler/src/main.cpp

#include "krauler/config.hpp"
#include "krauler/krauler.hpp"

int main(int argc, char* argv[]) {
    krauler::Config  config = krauler::parse_args(argc, argv);
    krauler::Krauler krauler(config);
    krauler.start();

    return 0;
}
