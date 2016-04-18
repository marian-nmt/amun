#pragma once

#include "types.h"
#include "kenlm.h"

class Hypothesis {
 public:
    Hypothesis(const Hypothesis* prevHyp, size_t word, size_t prevIndex, float cost)
      : prevHyp_(prevHyp),
        prevIndex_(prevIndex),
        word_(word),
        cost_(cost)
    {}

    const Hypothesis* GetPrevHyp() const {
      return prevHyp_;
    }

    
    size_t GetWord() const {
      return word_;
    }

    size_t GetPrevStateIndex() const {
      return prevIndex_;
    }

    float GetCost() const {
      return cost_;
    }
    
    std::vector<float>& GetCostBreakdown() {
      return costBreakdown_;
    }
    
    void AddLMState(const KenlmState& state) {
      lmStates_.push_back(state);
    }
    
    const std::vector<KenlmState>& GetLMStates() const {
      return lmStates_;
    }

 private:
    const Hypothesis* prevHyp_;
    const size_t prevIndex_;
    const size_t word_;
    const float cost_;
    std::vector<KenlmState> lmStates_;
    
    std::vector<float> costBreakdown_;
};

typedef std::vector<Hypothesis*> Beam;
typedef std::pair<Sentence, Hypothesis*> Result;
typedef std::vector<Result> NBestList;