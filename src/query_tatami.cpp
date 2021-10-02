#include "tatami/tatami.h"
#include "Rcpp.h"

//[[Rcpp::export(rng=false)]]
Rcpp::IntegerVector tatami_dim(SEXP x) {
    Rcpp::XPtr<tatami::NumericMatrix> ptr(x);
    return Rcpp::IntegerVector::create(ptr->nrow(), ptr->ncol());
}

//[[Rcpp::export(rng=false)]]
Rcpp::NumericMatrix tatami_rows(SEXP x, Rcpp::IntegerVector rows, int first, int last) {
    Rcpp::XPtr<tatami::NumericMatrix> ptr(x);

    size_t nc = last - first;
    Rcpp::NumericMatrix output(nc, rows.size());

    double* optr = output.begin();
    auto wrk = ptr->new_workspace(true);
    for (auto r : rows) {
        ptr->row_copy(r, optr, first, last, wrk.get());
    }

    return Rcpp::transpose(output);
}

//[[Rcpp::export(rng=false)]]
Rcpp::NumericMatrix tatami_columns(SEXP x, Rcpp::IntegerVector columns, int first, int last) {
    Rcpp::XPtr<tatami::NumericMatrix> ptr(x);

    size_t nr = last - first;
    Rcpp::NumericMatrix output(nr, columns.size());

    double* optr = output.begin();
    auto wrk = ptr->new_workspace(false);
    for (auto c : columns) {
        ptr->column_copy(c, optr, first, last, wrk.get());
    }

    return output;
}
