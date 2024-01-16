#ifndef DATA_STRUCTURES_DFA_H
#define DATA_STRUCTURES_DFA_H
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <utility>
#include "nfa.h"

using std::set, std::map, std::pair, std::string, std::vector;

template<typename State_type, typename Token_type>
class DFA {
public:
    DFA(NFA<State_type, Token_type> N);
    DFA(set<State_type> Q, set<Token_type> Sigma, map<pair<State_type, Token_type>, State_type> delta, State_type s,
        set<State_type> F);

    [[nodiscard]] bool accept(const vector<Token_type>& tokens) const;

    [[nodiscard]] DFA minimize() const;

private:
    const set<State_type> Q;
    const set<Token_type> Sigma;
    const map<pair<State_type, Token_type>, State_type> delta;
    const State_type s;
    const set<State_type> F;
};
#endif //DATA_STRUCTURES_DFA_H
