#include "Rcpp.h"
#include "knncolle.h"
#include "run_tsne.h"
#ifdef _OPENMP
#include "omp.h"
#endif

//[[Rcpp::export(rng=false)]]
SEXP initialize_tsne(SEXP nnptr, double perplexity, int interpolate, int max_depth, int nthreads) {
#ifdef _OPENMP
    omp_set_num_threads(nthreads);
#endif
    KnncollePtr nns(nnptr);

    MyTsne runner;
    runner.set_perplexity(perplexity).set_max_depth(max_depth);
    
    if (interpolate) {
        if (interpolate > 0) {
            runner.set_interpolation(interpolate);
        } else if (nns->nobs() > 50000) { // deduce it automatically.
            runner.set_interpolation(200);
        }
    }

    auto status = runner.initialize(nns.get());

    std::vector<float> embedding(2 * nns->nobs());
    qdtsne::initialize_random(embedding.data(), nns->nobs());
   
    return Rcpp::XPtr<InitializedTsne>(new InitializedTsne(std::move(runner), std::move(status), std::move(embedding), nns->nobs()));
}

//[[Rcpp::export(rng=false)]]
SEXP run_tsne(SEXP init, int nthreads) {
#ifdef _OPENMP
    omp_set_num_threads(nthreads);
#endif
    Rcpp::XPtr<InitializedTsne> ptr(init);
    ptr->run();
    return ptr->yield();
}
