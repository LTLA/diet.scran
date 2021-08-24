#include "Rcpp.h"
#include "scran/PerCellQCMetrics.h"

// [[Rcpp::export(rng=false)]]
Rcpp::List compute_qc_metrics(Rcpp::RObject x, Rcpp::List subsets) {
    auto mat = tatamize(x);

    std::vector<const int*> in_sub_ptrs;
    std::vector<Rcpp::LogicalVector> in_subsets;

    for (auto sIt = subsets.begin(); sIt != subsets.end(); ++sIt) {
        in_subsets.emplace_back(*sIt);
    }
    for (const auto& s : in_subsets) {
        in_sub_ptrs.push_back(s.begin());
    }

    // Creating output containers.
    size_t nc = mat->ncol();
    Rcpp::NumericVector sums(nc);
    Rcpp::NumericVector detected(nc);
    std::vector<Rcpp::LogicalVector> out_subsets;
    std::vector<int*> out_sub_ptrs;
    for (size_t s = 0; s < subsets.size(); ++s) {
        out_subsets.emplace_back(nc);
    }
    for (auto& s : out_subsets) {
        out_sub_ptrs.push_back(s.begin());
    }

    // Running QC code.
    scran::PerCellQCMetrics qc;
    qc.run(mat, 
        std::move(in_sub_ptrs), 
        static_cast<double*>(sums.begin()),
        static_cast<double*>(detected.begin()),
        std::move(out_sub_ptrs)
    );

    return Rcpp::List::create(
        sums,
        detected,
        Rcpp::List(out_subsets.begin(), out_subsets.end())
    );
}
