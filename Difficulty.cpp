#include "Difficulty.h"

int Easy::Kytuan(const string& word) const {
    int length = word.length();
    return(length * 20 + 99) / 100;
}
int Medium::Kytuan(const string& word) const {
    int length = word.length();
    return(length * 50 + 99) / 100;
}
int Hard::Kytuan(const string& word) const {
    int length = word.length();
    return(length * 80 + 99) / 100;
}