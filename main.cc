#include <stdio.h>
#include <cstring>
#include <iostream>
#include <vector>

const int MaxSupportedLength = 10;

struct Args {
    int MinLength;
    int MaxLength;
    const char * MD5;

    void Validate();
    void Print(std::ostream& out);
    static Args Parse(int argc, char **argv);
};

void Args::Validate() {
    if (this->MinLength < 0) {
        throw std::invalid_argument("MinLength < 0");
    }
    if (this->MaxLength > MaxSupportedLength) {
        throw std::invalid_argument("MaxLength > max");
    }
    if (this->MD5 == nullptr || *this->MD5 == '\0') {
        throw std::invalid_argument("MD5 is missing or empty");
    }
}

void Args::Print(std::ostream& out) {
    out << "MinLength: " << this->MinLength << std::endl;
    out << "MaxLength: " << this->MaxLength << std::endl;
    out << "MD5: " << this->MD5 << std::endl;
}

int parseInt(char *str) {
    size_t pos;
    int result = std::stoi(str, &pos);
    if (str[pos] != '\0') {
        throw std::invalid_argument("bad integer");
    }
    return result;
}

Args Args::Parse(int argc, char **argv) {
    Args result = {0};

    for (int i; i < argc; ++i) {
        if (strcmp(argv[i], "--min-length") == 0) {
            ++i;
            try {
                result.MinLength = parseInt(argv[i]);
            } catch (...) {
                std::throw_with_nested(std::runtime_error("failed to parse --min-length"));
            }
        } else if (strcmp(argv[i], "--max-length") == 0) {
            ++i;
            try {
                result.MaxLength = parseInt(argv[i]);
            } catch (...) {
                std::throw_with_nested(std::runtime_error("failed to parse --max-length"));
            }
        } else {
        }
    }

    return result;
}

const char* DefaultAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

class Permutation {
  public:
    Permutation(size_t len, const char* alphabet = DefaultAlphabet);
    void Get(char* out) const;
    // Advance moves the permutation n-points forward and returns true if it overflowed
    bool Advance(size_t n);
  private:
    const char* alphabet_;
    size_t alphabet_size_;
    size_t len_;
    std::vector<size_t> value_;
};

Permutation::Permutation(size_t len, const char* alphabet)
    : len_(len)
    , alphabet_(alphabet)
    , value_(len, 0) {
    this->alphabet_size_ = strlen(alphabet);
}

void Permutation::Get(char* out) const {
    for (size_t i = 0; i < this->len_; ++i) {
        out[i] = this->alphabet_[this->value_[i]];
    }
}

bool Permutation::Advance(size_t n) {
    auto i = 0;
    while (n != 0 && i < this->len_) {
        size_t digit = n % this->alphabet_size_;
        n /= this->alphabet_size_;

        this->value_[i] += digit;
        if (this->value_[i] > this->alphabet_size_) {
            this->value_[i] -= this->alphabet_size_;
            ++n;
        }

        ++i;
    }
    if (i == this->len_) {
        // We've reached the last digit and n is still not zero.
        // This means we've overflowed.
        std::fill(this->value_.begin(), this->value_.end(), 0);
        return true;
    }
    return false;
}

void print_exception(const std::exception& e, int level =  0)
{
    std::cerr << std::string(level, ' ') << "exception: " << e.what() << '\n';
    try {
        std::rethrow_if_nested(e);
    } catch(const std::exception& e) {
        print_exception(e, level+1);
    } catch(...) {}
}

int main(int argc, char **argv) {
    try {
        auto args = Args::Parse(argc, argv);
        args.Print(std::cout);
        args.Validate();
    } catch (const std::exception& e) {
        print_exception(e);
    }
    return 0;
}

