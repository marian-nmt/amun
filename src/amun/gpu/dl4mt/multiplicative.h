#pragma once
#include <boost/timer/timer.hpp>
#include "gpu/mblas/tensor_functions.h"
#include "gpu/mblas/tensor_wrapper.h"
#include "gpu/mblas/handles.h"
#include "gpu/dl4mt/cell.h"
#include "cellstate.h"
#include "gpu/dl4mt/lstm.h"
#include "gpu/dl4mt/model.h"

namespace amunmt {
namespace GPU {

template <template<class> class CellType, class InnerWeights>
class Multiplicative: public Cell {
  public:
 Multiplicative(const Weights::MultWeights<InnerWeights>& model)
      : innerCell_(model), w_(model)
    {}
    virtual void GetNextState(CellState& NextState,
                              const CellState& State,
                              const mblas::Tensor& Context) const {
      using namespace mblas;
      // TODO: the weight matrix naming probably is inconsistent
      /* HANDLE_ERROR( cudaStreamSynchronize(mblas::CudaStreamHandler::GetStream())); */
      /* std::cerr << "Multipliative::GetNextState1" << std::endl; */

      Copy(*tempState_.cell, *State.cell);
      Prod(*tempState_.output, *State.output, *w_.Um_);
      BroadcastVec(thrust::placeholders::_1 + thrust::placeholders::_2, *tempState_.output, *w_.Bmu_);
      Prod(x_mult_, Context, *w_.Wm_);
      BroadcastVec(thrust::placeholders::_1 + thrust::placeholders::_2, x_mult_, *w_.Bm_);
      Element(thrust::placeholders::_1 * thrust::placeholders::_2, *tempState_.output, x_mult_);
      innerCell_.GetNextState(NextState, tempState_, Context);
    }
    virtual CellLength GetStateLength() const {
      return innerCell_.GetStateLength();
    }

    virtual std::string Debug(unsigned verbosity = 1) const
    {
      return "Multiplicative";
    }

  private:
    CellType<InnerWeights> innerCell_;
    const Weights::MultWeights<InnerWeights>& w_;
    mutable mblas::Tensor x_mult_;
    mutable CellState tempState_;
};
}
}
