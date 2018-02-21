#include "Solver.h"

using namespace sqaod;

template<class real>
void Solver<real>::setPreferences(const Preferences &prefs) {
    for (Preferences::const_iterator it = prefs.begin();
         it != prefs.end(); ++it) {
        setPreference(*it);
    }
}


template<class real>
Algorithm BFSearcher<real>::selectAlgorithm(Algorithm algo) {
    return algoBruteForceSearch;
}
    
template<class real>Algorithm BFSearcher<real>::getAlgorithm() const {
    return algoBruteForceSearch;
}


template<class real>
Preferences Annealer<real>::getPreferences() const {
    Preferences prefs;
    prefs.pushBack(Preference(pnAlgorithm, this->getAlgorithm()));
    prefs.pushBack(Preference(pnNumTrotters, this->m_));
    return prefs;
}

template<class real>
void Annealer<real>::setPreference(const Preference &pref) {
    if (pref.name == pnNumTrotters) {
        throwErrorIf(pref.nTrotters <= 0, "# trotters must be a positive integer.");
        this->m_ = pref.nTrotters;
        this->annState_ |= annNTrottersGiven;
    }
}


template<class real>
void DenseGraphSolver<real>::getProblemSize(SizeType *N) const {
    *N = N_;
}

template<class real>
void BipartiteGraphSolver<real>::getProblemSize(SizeType *N0, SizeType *N1) const {
    *N0 = N0_;
    *N1 = N1_;
}


template<class real>
Preferences DenseGraphBFSearcher<real>::getPreferences() const {
    Preferences prefs;
    prefs.pushBack(Preference(pnAlgorithm, this->getAlgorithm()));
    prefs.pushBack(Preference(pnTileSize, tileSize_));
    return prefs;
}

template<class real>
void DenseGraphBFSearcher<real>::setPreference(const Preference &pref) {
    if (pref.name == pnTileSize) {
        throwErrorIf(pref.tileSize <= 0, "tileSize must be a positive integer.");
        tileSize_ = pref.tileSize;
    }
}


template<class real>
void DenseGraphBFSearcher<real>::search() {
    this->initSearch();
    PackedBits xStep = std::min(PackedBits(tileSize_), xMax_);
    for (PackedBits xTile = 0; xTile < xMax_; xTile += xStep)
        searchRange(xTile, xTile + xStep);
    this->finSearch();
}


template<class real>
Preferences BipartiteGraphBFSearcher<real>::getPreferences() const {
    Preferences prefs;
    prefs.pushBack(Preference(pnAlgorithm, algoBruteForceSearch));
    prefs.pushBack(Preference(pnTileSize0, tileSize0_));
    prefs.pushBack(Preference(pnTileSize1, tileSize1_));
    return prefs;
}

template<class real>
void BipartiteGraphBFSearcher<real>::setPreference(const Preference &pref) {
    if (pref.name == pnTileSize0) {
        throwErrorIf(pref.tileSize <= 0, "tileSize0 must be a positive integer.");
        tileSize0_ = pref.tileSize;
    }
    if (pref.name == pnTileSize1) {
        throwErrorIf(pref.tileSize <= 0, "tileSize1 must be a positive integer.");
        tileSize1_ = pref.tileSize;
    }
}

template<class real>
void BipartiteGraphBFSearcher<real>::search() {
    this->initSearch();

    PackedBits xStep0 = std::min((PackedBits)tileSize0_, x0max_);
    PackedBits xStep1 = std::min((PackedBits)tileSize1_, x1max_);
    for (PackedBits xTile1 = 0; xTile1 < x1max_; xTile1 += xStep1) {
        for (PackedBits xTile0 = 0; xTile0 < x0max_; xTile0 += xStep0) {
            searchRange(xTile0, xTile0 + xStep0, xTile1, xTile1 + xStep1);
        }
    }

    this->finSearch();
}


/* explicit instantiation */
template struct sqaod::Solver<double>;
template struct sqaod::Solver<float>;
template struct sqaod::BFSearcher<double>;
template struct sqaod::BFSearcher<float>;
template struct sqaod::Annealer<double>;
template struct sqaod::Annealer<float>;
template struct sqaod::DenseGraphSolver<double>;
template struct sqaod::DenseGraphSolver<float>;
template struct sqaod::BipartiteGraphSolver<double>;
template struct sqaod::BipartiteGraphSolver<float>;

template struct sqaod::DenseGraphBFSearcher<double>;
template struct sqaod::DenseGraphBFSearcher<float>;
template struct sqaod::DenseGraphAnnealer<double>;
template struct sqaod::DenseGraphAnnealer<float>;
template struct sqaod::BipartiteGraphBFSearcher<double>;
template struct sqaod::BipartiteGraphBFSearcher<float>;
template struct sqaod::BipartiteGraphAnnealer<double>;
template struct sqaod::BipartiteGraphAnnealer<float>;