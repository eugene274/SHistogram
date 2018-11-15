#include <utility>

//
// Created by eugene on 11/13/18.
//

#ifndef SHISTOGRAM_SHISTOBUILDER_H
#define SHISTOGRAM_SHISTOBUILDER_H

#include <vector>
#include <TAxis.h>
#include <unordered_map>
#include <map>
#include <TH2D.h>
#include <TError.h>

class SHistoBuilder {
 public:
  typedef int AxisIndex;
  typedef std::array<AxisIndex, 2> CorrelationIndex;

  SHistoBuilder() = default;

  explicit SHistoBuilder(std::string prefix) : prefix(std::move(prefix)) {}

  AxisIndex AddAxis(const char *name, int nbins, double xmin, double xmax, double *dp = nullptr, const char *title = "");

  bool HasCorrelation(const CorrelationIndex &correlationIndex);

  CorrelationIndex AddCorrelation(AxisIndex a1, AxisIndex a2);

  CorrelationIndex AddCorrelation(CorrelationIndex correlationIndex);

  std::vector<CorrelationIndex > AddCorrelations(std::vector<CorrelationIndex > correlations);

  AxisIndex AddHistogram(const int index);

  void AddAllHistograms();

  void AddAllCorrelations();

  void Fill(double weight = 0);

  void SetDataPointer(AxisIndex axis, double *dp) {
    dp_array[axis] = dp;
  }

  void SetPrefix(const std::string &prefix) {
    SHistoBuilder::prefix = prefix;
  }

  TH2D &operator[](const CorrelationIndex &index) {
    return *(h2d[index]);
  }

  TH1D &operator[](const AxisIndex &index) {
    return *(h1d[index]);
  }

 private:
  struct index_correlation_hash {
    size_t operator()(const CorrelationIndex &array) const {
      std::hash<int> hash_int;
      return hash_int(array[0]);
    }
  };

  std::string prefix = "";

  std::vector<TAxis *> axis_array;
  std::vector<double *> dp_array;

  std::map<AxisIndex, TH1D *> h1d;
  std::unordered_map<CorrelationIndex, TH2D *, index_correlation_hash> h2d;

};

#endif //SHISTOGRAM_SHISTOBUILDER_H
