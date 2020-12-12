#pragma once
#include <boost/timer/timer.hpp>
#include "gpu/mblas/tensor_functions.h"
#include "gpu/mblas/tensor_wrapper.h"
#include "gpu/mblas/handles.h"
#include "gpu/dl4mt/cell.h"
#include "cellstate.h"

namespace amunmt {
namespace GPU {

template <class Weights>
class SlowGRU: public Cell {
  public:
    SlowGRU(const Weights& model)
    : w_(model) {}

    virtual void GetNextState(CellState& NextState,
                      const CellState& State,
                      const mblas::Tensor& Context) const {
      using namespace mblas;

      //std::cerr << std::endl;

      const unsigned cols = GetStateLength().output;

      // @TODO: Optimization
      // @TODO: Launch streams to perform GEMMs in parallel
      // @TODO: Join matrices and perform single GEMM --------
      Prod(RU_, Context, *w_.W_);
      Prod(H_,  Context, *w_.Wx_);
      // -----------------------------------------------------

      // @TODO: Join matrices and perform single GEMM --------
      Prod(Temp1_, *(State.output), *w_.U_);
      Prod(Temp2_, *(State.output), *w_.Ux_);
      //std::cerr << "Temp2_=" << Temp2_.Debug(1) << std::endl;
      // -----------------------------------------------------

      // @TODO: Organize into one kernel ---------------------
      //std::cerr << "1RU_=" << RU_.Debug(1) << std::endl;
      //std::cerr << "w_.B_=" << w_.B_.Debug(1) << std::endl;
      BroadcastVec(thrust::placeholders::_1 + thrust::placeholders::_2, RU_, *w_.B_); // Broadcasting row-wise
      //std::cerr << "2RU_=" << RU_.Debug(1) << std::endl;

      //std::cerr << "Temp1_=" << Temp1_.Debug(1) << std::endl;
      Element(Logit(thrust::placeholders::_1 + thrust::placeholders::_2), RU_, Temp1_);
      //std::cerr << "3RU_=" << RU_.Debug(1) << std::endl;

      //std::cerr << "cols=" << cols << std::endl;
      Slice(R_, RU_, 0, cols);
      //std::cerr << "R_=" << R_.Debug(1) << std::endl;

      Slice(U_, RU_, 1, cols);
      //std::cerr << "U_=" << U_.Debug(1) << std::endl;

      BroadcastVec(thrust::placeholders::_1 + thrust::placeholders::_2, H_,    *w_.Bx1_); // Broadcasting row-wise
      //std::cerr << "H_=" << H_.Debug(1) << std::endl;

      //std::cerr << "1Temp2_=" << Temp2_.Debug(1) << std::endl;
      //std::cerr << "w_.Bx2_=" << w_.Bx2_.Debug(1) << std::endl;
      BroadcastVec(thrust::placeholders::_1 + thrust::placeholders::_2, Temp2_, *w_.Bx2_); // Broadcasting row-wise
      //std::cerr << "2Temp2_=" << Temp2_.Debug(1) << std::endl;

      //std::cerr << "1H_=" << H_.Debug(1) << std::endl;
      //std::cerr << "R_=" << R_.Debug(1) << std::endl;
      //std::cerr << "Temp2__=" << Temp2_.Debug(1) << std::endl;
      Element(Tanh(thrust::placeholders::_1 + thrust::placeholders::_2 * thrust::placeholders::_3), H_, R_, Temp2_);
      //std::cerr << "2H_=" << H_.Debug(1) << std::endl;

      //std::cerr << "1U_=" << U_.Debug(1) << std::endl;
      //std::cerr << "H_=" << H_.Debug(1) << std::endl;
      //std::cerr << "State=" << State.Debug(1) << std::endl;
      Element((1.0 - thrust::placeholders::_1) * thrust::placeholders::_2 + thrust::placeholders::_1 * thrust::placeholders::_3, U_, H_, *(State.output));
      //std::cerr << "2U_=" << H_.Debug(1) << std::endl;
      // -----------------------------------------------------

      Swap(*(NextState.output), U_);
      if(State.cell->size() > 0) {
        Swap(*(NextState.cell), *(State.cell));
      }
    }

    virtual CellLength GetStateLength() const {
      return CellLength(0, w_.U_->dim(0));
    }

    virtual std::string Debug(unsigned verbosity = 1) const
    {
      std::stringstream strm;
      return "GRU";
    }

  private:
    // Model matrices
    const Weights& w_;

    // reused to avoid allocation
    mutable mblas::Tensor RU_;
    mutable mblas::Tensor R_;
    mutable mblas::Tensor U_;
    mutable mblas::Tensor H_;
    mutable mblas::Tensor Temp1_;
    mutable mblas::Tensor Temp2_;

    SlowGRU(const SlowGRU&) = delete;
};

///////////////////////////////////////////////////////////////////////////////////////////////

__global__ void gElementwiseOps(mblas::TensorWrapper<float> outWrap,
                                const mblas::TensorWrapper<float> stateWrap,
                                const mblas::TensorWrapper<float> ruhWrap,
                                const mblas::TensorWrapper<float> tempWrap,
                                const mblas::TensorWrapper<float> bWrap,
                                const mblas::TensorWrapper<float> bx1Wrap,
                                const mblas::TensorWrapper<float> bx2Wrap);

template <class Weights>
class FastGRU: public Cell {

  public:
    FastGRU(const Weights& model)
    : w_(model)
    {
      /*for(int i = 0; i < 4; ++i) {
        cudaStreamCreate(&s_[i]);
        cublasCreate(&h_[i]);
        cublasSetStream(h_[i], s_[i]);
      }*/

      using namespace mblas;
      Transpose(WWx_, *w_.W_);
      //std::cerr << std::endl;
      //std::cerr << "w_.W_=" << w_.W_.Debug(1) << std::endl;
      //std::cerr << "1WWx_=" << WWx_.Debug(1) << std::endl;

      Tensor WxT;
      Transpose(WxT, *w_.Wx_);
      //std::cerr << "w_.Wx_=" << w_.Wx_.Debug(1) << std::endl;
      //std::cerr << "WxT=" << WxT.Debug(1) << std::endl;

      Concat(WWx_, WxT);
      //std::cerr << "2WWx_=" << WWx_.Debug(1) << std::endl;

      Transpose(WWx_);
      //std::cerr << "3WWx_=" << WWx_.Debug(1) << std::endl;

      Transpose(UUx_, *w_.U_);
      Tensor UxT;
      Transpose(UxT, *w_.Ux_);
      Concat(UUx_, UxT);
      Transpose(UUx_);

      //std::cerr << std::endl;
    }

    virtual void GetNextState(CellState& NextState,
                      const CellState& State,
                      const mblas::Tensor& Context) const {
      using namespace mblas;

      //std::cerr << std::endl;
      //std::cerr << "1RUH_=" << RUH_.Debug(1) << std::endl;
      //std::cerr << "Context=" << Context.Debug(1) << std::endl;
      //std::cerr << "WWx_" << WWx_.Debug(1) << std::endl;

      Prod(RUH_, Context, WWx_);

      //std::cerr << "2RUH_=" << RUH_.Debug(1) << std::endl;

      if (w_.Gamma_1_->size()) {
        Normalization(RUH_, RUH_, *w_.Gamma_1_, 1e-9);
      }

      Prod(Temp_, *(State.output), UUx_);
      //std::cerr << "State=" << State.Debug(1) << std::endl;
      //std::cerr << "UUx_" << UUx_.Debug(1) << std::endl;
      //std::cerr << "Temp_=" << Temp_.Debug(1) << std::endl;

      if (w_.Gamma_2_->size()) {
        Normalization(Temp_, Temp_, *w_.Gamma_2_, 1e-9);
      }

      ElementwiseOps(*(NextState.output), *(State.output), RUH_, Temp_);
      if(State.cell->size() > 0) {
        Swap(*(NextState.cell), *(State.cell));
      }
    }


    void ElementwiseOps(mblas::Tensor& NextState,
                        const mblas::Tensor& State,
                        const mblas::Tensor& RUH,
                        const mblas::Tensor& Temp) const
    {
      //BEGIN_TIMER("ElementwiseOps");

      assert(State.dim(2) == 1);
      assert(State.dim(3) == 1);
      assert(RUH.dim(2) == 1);
      assert(RUH.dim(3) == 1);
      assert(Temp.dim(2) == 1);
      assert(Temp.dim(3) == 1);

      NextState.NewSize(State.dim(0), State.dim(1), 1, 1);
      //std::cerr << "NextState=" << NextState.Debug() << std::endl;

      mblas::TensorWrapper<float> nextWrap(NextState);
      const mblas::TensorWrapper<float> stateWrap(State);
      const mblas::TensorWrapper<float> ruhWrap(RUH);
      const mblas::TensorWrapper<float> tempWrap(Temp);
      const mblas::TensorWrapper<float> bWrap(*w_.B_);
      const mblas::TensorWrapper<float> bx1Wrap(*w_.Bx1_);
      const mblas::TensorWrapper<float> bx2Wrap(*w_.Bx2_);

      /*
      std::cerr << "nextWrap=" << nextWrap.Debug() << std::endl;
      std::cerr << "stateWrap=" << stateWrap.Debug() << std::endl;
      std::cerr << "ruhWrap=" << ruhWrap.Debug() << std::endl;
      std::cerr << "tempWrap=" << tempWrap.Debug() << std::endl;
      std::cerr << "bWrap=" << bWrap.Debug() << std::endl;
      std::cerr << "bx1Wrap=" << bx1Wrap.Debug() << std::endl;
      std::cerr << "bx2Wrap=" << bx2Wrap.Debug() << std::endl;
      std::cerr << "\n";
      */
      const unsigned cols = State.dim(1);
      const unsigned rows = State.dim(0);

      int threads = std::min(MAX_THREADS, (int)cols);
      int blocks  = rows;

      gElementwiseOps<<<blocks, threads, 0, mblas::CudaStreamHandler::GetStream()>>>
        (nextWrap, stateWrap, ruhWrap, tempWrap,
            bWrap, bx1Wrap, bx2Wrap);
      HANDLE_ERROR(cudaGetLastError());

      //PAUSE_TIMER("ElementwiseOps");

    }

    virtual CellLength GetStateLength() const {
      return CellLength(0, w_.U_->dim(0));
    }

    virtual std::string Debug(unsigned verbosity = 1) const
    {
      std::stringstream strm;
      strm << w_.Debug(verbosity);
      return strm.str();
    }

  private:
    // Model matrices
    const Weights& w_;

    // reused to avoid allocation
    mutable mblas::Tensor WWx_;
    mutable mblas::Tensor UUx_;

    mutable mblas::Tensor RUH_;
    mutable mblas::Tensor Temp_;

    FastGRU(const FastGRU&) = delete;
};

template<class T>
//using GRU = SlowGRU<T>;
using GRU = FastGRU<T>;

}
}


