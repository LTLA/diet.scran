#include "tatamize.h"
#include "Rcpp.h"
#include "scran/feature_selection/ModelGeneVar.hpp"
#ifdef _OPENMP
#include "omp.h"
#endif

//[[Rcpp::export(rng=false)]]
Rcpp::List model_gene_var(SEXP x, double span, int nthreads) {
#ifdef _OPENMP
    omp_set_num_threads(nthreads);
#endif

    scran::ModelGeneVar mvar;
    mvar.set_span(span);

    auto mat = extract_NumericMatrix(x);
    size_t NR = mat->nrow();

    Rcpp::NumericVector means(NR), variances(NR), fitted(NR), residuals(NR);

    mvar.run(mat, 
        static_cast<double*>(means.begin()),
        static_cast<double*>(variances.begin()),
        static_cast<double*>(fitted.begin()),
        static_cast<double*>(residuals.begin())
    );

    return Rcpp::List::create(
        Rcpp::Named("means") = means,
        Rcpp::Named("variances") = variances,
        Rcpp::Named("fitted") = fitted,
        Rcpp::Named("residuals") = residuals
    );
}
