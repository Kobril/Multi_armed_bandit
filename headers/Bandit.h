#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <vector>

class Bandit {
public:
    int N;
    double epsilon;
    double learning_rate;
    double* q;
    double* preferences;
    int* nt;
    double* UCBvalues;
    double avg_reward;
    double* pii;

    Bandit(const int n, double e, double l);
    ~Bandit();

    void update_q(double r, int a);
    void update_avg_reward(int n, double r);
    void update_action_preferences(double r, int a);

    int number_of_actions_taken();
    void print_q() const noexcept;
    void print_action_preferences() const noexcept;
    int take_action();
    int UCB(int t, double c);
    int gradientBanditAction();
};
