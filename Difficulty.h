#pragma once
#include<iostream>
#include<string>
#include<algorithm>
using namespace std;

class Difficulty {
    public:
        virtual int Kytuan(const string& word) const = 0;
        virtual ~Difficulty() {};
};
class Easy : public Difficulty {
    public:
        int Kytuan(const string& word) const override;
};
class Medium : public Difficulty {
    public:
        int Kytuan(const string& word) const override;
};
class Hard : public Difficulty {
    public:
        int Kytuan(const string& word) const override;
};