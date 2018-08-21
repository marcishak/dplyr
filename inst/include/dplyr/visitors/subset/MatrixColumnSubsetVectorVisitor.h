#ifndef dplyr_MatrixColumnSubsetVisitor_H
#define dplyr_MatrixColumnSubsetVisitor_H

#include <tools/default_value.h>

namespace dplyr {

template <int RTYPE>
class MatrixColumnSubsetVisitor : public SubsetVectorVisitor {
public:

  typedef typename Rcpp::traits::storage_type<RTYPE>::type STORAGE;
  typedef typename Matrix<RTYPE>::Column Column;
  typedef typename Matrix<RTYPE>::ConstColumn ConstColumn;

  MatrixColumnSubsetVisitor(const Matrix<RTYPE>& data_) : data(data_) {}

  inline SEXP subset(const Rcpp::IntegerVector& index) const {
    return subset_int(index);
  }

  inline SEXP subset(const std::vector<int>& index) const {
    return subset_int(index);
  }

  inline SEXP subset(const SlicingIndex& index) const {
    return subset_int(index);
  }

  inline SEXP subset(const ChunkIndexMap& index) const {
    int n = index.size();
    int nc = data.ncol();
    Matrix<RTYPE> res(n, data.ncol());
    for (int h = 0; h < nc; h++) {
      ChunkIndexMap::const_iterator it = index.begin();
      Column column = res.column(h);
      ConstColumn source_column = data.column(h);

      for (int i = 0; i < n; i++, ++it) {
        column[i] = source_column[ it->first ];
      }
    }
    return res;
  }

  inline SEXP subset(EmptySubset) const {
    return Matrix<RTYPE>(0, data.ncol());
  }

  inline int size() const {
    return data.nrow();
  }

private:

  template <typename Container>
  inline SEXP subset_int(const Container& index) const {
    int n = index.size(), nc = data.ncol();
    Matrix<RTYPE> res(n, nc);
    for (int h = 0; h < nc; h++) {
      Column column = res.column(h);
      ConstColumn source_column = data.column(h);
      for (int k = 0; k < n; k++) {
        int idx = index[k];
        column[k] = (idx < 0) ? default_value<RTYPE>() : (STORAGE)source_column[ index[k] ];
      }
    }
    return res;
  }

  Matrix<RTYPE> data;
};

}

#endif
