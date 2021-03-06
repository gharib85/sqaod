/* -*- c++ -*- */
#pragma once

#include <sqaodc/common/Common.h>
#include <sqaodc/cuda/Device.h>
#include <sqaodc/cuda/DeviceMatrix.h>
#include <sqaodc/cuda/DeviceArray.h>
#include <sqaodc/cuda/DeviceDenseGraphBatchSearch.h>
#include <sqaodc/cuda/DeviceRandom.h>
#include <sqaodc/cuda/DeviceRandomBuffer.h>


namespace sqaod_cuda {

namespace sq = sqaod;

template<class real>
class WAR_VC_MULTIPLE_INHERITANCE CUDABipartiteGraphAnnealer : public sqaod::cuda::BipartiteGraphAnnealer<real> {
    typedef sq::MatrixType<real> HostMatrix;
    typedef sq::VectorType<real> HostVector;
    typedef sq::BitSetPairArray BitSetPairArray;
    typedef sq::BitSet BitSet;
    typedef sq::SizeType SizeType;
    typedef DeviceMatrixType<real> DeviceMatrix;
    typedef DeviceVectorType<real> DeviceVector;
    typedef DeviceScalarType<real> DeviceScalar;
    typedef DeviceBipartiteGraphFormulas<real> DeviceFormulas;
    
public:
    CUDABipartiteGraphAnnealer();

    CUDABipartiteGraphAnnealer(Device &device);

    ~CUDABipartiteGraphAnnealer();

    void deallocate();
    
    void assignDevice(sqaod::cuda::Device &device);
    
    void assignDevice(Device &device);

    virtual sq::Algorithm selectAlgorithm(sq::Algorithm algo);
    
    void seed(unsigned long long seed);

    /* void getProblemSize(SizeType *N0, SizeType *N1) const; */

    void setQUBO(const HostVector &b0, const HostVector &b1, const HostMatrix &W,
                 sq::OptimizeMethod om = sq::optMinimize);

    void setHamiltonian(const HostVector &h0, const HostVector &h1, const HostMatrix &J,
                        real c = real(0.));

    sq::Preferences getPreferences() const;

    /* void setPreference(const Preference &pref); */

    const HostVector &get_E() const;

    const sq::BitSetPairArray &get_x() const;

    void set_q(const sq::BitSetPair &qPair);

    void set_qset(const sq::BitSetPairArray &qPairs);

    /* Ising machine / spins */

    void getHamiltonian(HostVector *h0, HostVector *h1, HostMatrix *J, real *c) const;

    const BitSetPairArray &get_q() const;

    void randomizeSpin();

    void prepare();

    void calculate_E();

    void makeSolution();

    real getSystemE(real G, real beta) const;

    void annealOneStep(real G, real beta) {
        (this->*annealMethod_)(G, beta);
    }

    void annealOneStepSQA(real G, real beta);

    void annealOneStepSA(real G, real beta);

    /* public for debug */
    void calculate_Jq(DeviceMatrix *d_Jq, const DeviceMatrix &d_J, MatrixOp op,
                      const DeviceMatrix &d_qFixed);

    /* public for debug */
    void tryFlipSQA(DeviceMatrix *d_qAnneal, const DeviceMatrix &d_Jq, int N, int m, 
                    const DeviceVector &d_h, const real *d_realRand, real G, real beta);

    /* public for debug */
    void tryFlipSA(DeviceMatrix *d_qAnneal, const DeviceMatrix &d_Jq, int N, int m, 
                   const DeviceVector &d_h, const real *d_realRand, real Tnorm);

private:
    void deallocateProblem();
    void deallocateInternalObjects();

    enum {
        /* FIXME: parameterise */
        maxRandBufCapacity = 128 << 20,
        maxNRunsPerRandGen = 10,
    };

    void syncBits();

    typedef void (CUDABipartiteGraphAnnealer<real>::*AnnealMethod)(real G, real beta);
    AnnealMethod annealMethod_;

    DeviceRandom d_random_;
    DeviceRandomBuffer d_randReal_;
    DeviceVector d_h0_, d_h1_;
    DeviceMatrix d_J_;
    DeviceScalar d_c_;
    DeviceMatrix d_matq0_, d_matq1_;
    DeviceBitMatrix h_q0_, h_q1_;
    sq::SizeType nRunsPerRandGen_;

    sq::NullBase *dotSpins0_;
    sq::NullBase *dotSpins1_;
    DeviceScalar h_spinDotSum_;
    
    DeviceMatrix d_Jq0_;
    DeviceMatrix d_Jq1_;
    
    DeviceVector h_E_; /* host mem */
    HostVector E_;
    BitSetPairArray bitsPairX_;
    BitSetPairArray bitsPairQ_;
    
    DeviceStream *devStream_;
    DeviceFormulas devFormulas_;
    DeviceCopy devCopy_;
    DeviceObjectAllocator *devAlloc_;

    typedef CUDABipartiteGraphAnnealer<real> This;
    typedef sq::BipartiteGraphAnnealer<real> Base;
    using Base::selectDefaultAlgorithm;
    using Base::selectDefaultSAAlgorithm;
    using Base::om_;
    using Base::N0_;
    using Base::N1_;
    using Base::m_;
    using Base::algo_;
    /* annealer state */
    using Base::solRandSeedGiven;
    using Base::solPrepared;
    using Base::solProblemSet;
    using Base::solQSet;
    using Base::solEAvailable;
    using Base::solSolutionAvailable;
    using Base::setState;
    using Base::clearState;
    using Base::isRandSeedGiven;
    using Base::isEAvailable;
    using Base::isSolutionAvailable;
    using Base::throwErrorIfProblemNotSet;
    using Base::throwErrorIfNotPrepared;
    using Base::throwErrorIfQNotSet;
};

}
