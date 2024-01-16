#ifndef DATA_STRUCTURES_NFA_H
#define DATA_STRUCTURES_NFA_H
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <utility>

using std::set, std::map, std::pair, std::string, std::vector;

template<typename State_type, typename Token_type>
class NFA {
public:
    NFA(set<State_type> Q, set<Token_type> Sigma, map<pair<State_type, Token_type>, State_type> delta, State_type s,
        set<State_type> F, Token_type epsilon);

    [[nodiscard]] bool accept(const vector<Token_type>& tokens) const;

private:
    const set<State_type> Q;
    const set<Token_type> Sigma;
    const map<pair<State_type, Token_type>, const set<State_type>> delta;
    const State_type s;
    const Token_type epsilon;
    const set<State_type> F;
};
#endif //DATA_STRUCTURES_NFA_H
