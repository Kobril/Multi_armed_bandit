#include "Bandit.h"

Bandit::Bandit(const int n, double e, double l) {
    N = n;
    epsilon = e;
    learning_rate = l;
    q = new double[N]();
    preferences = new double[N]();
    nt = new int[N]();
    UCBvalues = new double[N]();
    pii = new double[N]();
    avg_reward = 0;
}

Bandit::~Bandit() {
    delete[] q;
    delete[] preferences;
    delete[] nt;
    delete[] UCBvalues;
    delete[] pii;
}

void Bandit::update_q(double r, int a) {
    q[a] += learning_rate * (r - q[a]);
}

void Bandit::update_avg_reward(int n, double r) {
    if (n == 0) {
        avg_reward += 1 * (r - avg_reward);
    }
    else {
        avg_reward += 1 / static_cast<double>(n) * (r - avg_reward);
    }
}

void Bandit::update_action_preferences(double r, int a) {
    for (int i = 0; i < N; i++) {
        if (i == a) {
            preferences[i] += learning_rate * (r - avg_reward) * (1 - pii[i]);
        }
        else {
            preferences[i] -= learning_rate * (r - avg_reward) * pii[i];
        }
    }
}

int Bandit::number_of_actions_taken() {
    int result = 0;
    for (int i = 0; i < N; i++) {
        result += nt[i];
    }
    return result;
}

void Bandit::print_q() const noexcept {
    std::cout << "\n";
    for (int i = 0; i < N; i++) {
        std::cout << "i=" << i << ", q[" << i << "]=" << q[i] << "\n";
    }
    std::cout << "\n";
}

void Bandit::print_action_preferences() const noexcept {
    std::cout << "\n";
    for (int i = 0; i < N; i++) {
        std::cout << "i=" << i << ", act_pref[" << i << "]=" << preferences[i] << "\n";
    }
    std::cout << "\n";
}

int Bandit::take_action() {
    int action = 0;
    double rand_num = static_cast<double>(rand()) / RAND_MAX;
    std::cout << rand_num << "\n";

    if (rand_num < epsilon) {  // random action
        action = rand() % N;
    }
    else {  // greedy action
        action = std::distance(q, std::max_element(q, q + N));
    }

    nt[action] += 1;

    return action;
}

int Bandit::UCB(int t, double c) {
    int action = 0;

    for (int j = 0; j < N; j++) {
        if (nt[j] != 0) {
            UCBvalues[j] = q[j] + c * sqrt(log(static_cast<double>(t)) / nt[j]);
        }
        else {
            UCBvalues[j] = 10000;
        }
    }
    action = std::distance(UCBvalues, std::max_element(UCBvalues, UCBvalues + N));


    nt[action] += 1;

    return action;
}

int Bandit::gradientBanditAction() {
    double denom = 0;
    int action;

    std::vector<double> pi;

    for (int i = 0; i < N; i++) {
        denom += exp(preferences[i]);
    }

    for (int i = 0; i < N; i++) {
        pii[i] = exp(preferences[i]) / denom;
        pi.push_back(exp(preferences[i]) / denom);
    }

    std::random_device rd;
    std::mt19937 generator(rd());

    std::discrete_distribution<int> distribution(pi.begin(), pi.end());
    action = distribution(generator);

    nt[action] += 1;
    return action;
}
