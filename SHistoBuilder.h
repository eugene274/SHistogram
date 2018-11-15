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

  SHistoBuilder(const SHistoBuilder &other) {
    prefix = other.prefix + "_copy";
    axis_array = other.axis_array;

    for (auto h1d_entry : other.h1d) {
      auto new_name = GetH1Name(h1d_entry.first);
      auto *new_h1 = h1d_entry.second->Clone(new_name);
      h1d_entry.second = (TH1D *) new_h1;
      h1d.insert(h1d_entry);
    }

    for (auto h2d_entry : other.h2d) {
      auto new_name = GetH2Name(h2d_entry.first);
      auto *new_h2 = h2d_entry.second->Clone(new_name);
      h2d_entry.second = (TH2D *) new_h2;
      h2d.insert(h2d_entry);
    }

    dp_array.reserve(axis_array.size());

  }

  AxisIndex AddAxis(const char *name,
                    int nbins,
                    double xmin,
                    double xmax,
                    double *dp = nullptr,
                    const char *title = "");

  bool HasCorrelation(const CorrelationIndex &correlationIndex);

  CorrelationIndex AddCorrelation(AxisIndex a1, AxisIndex a2);

  CorrelationIndex AddCorrelation(CorrelationIndex correlationIndex);

  std::vector<CorrelationIndex> AddCorrelations(std::vector<CorrelationIndex> correlations);

  AxisIndex AddHistogram(const int index);

  void AddAllHistograms();

  void AddAllCorrelations();

  void Fill(double weight = 0);

  void SetDataPointer(AxisIndex axis, double *dp) {
    dp_array[axis] = dp;
  }

  void SetPrefix(const std::string &prefix) {
    SHistoBuilder::prefix = prefix;

    for (auto h1d_entry : h1d) {
      h1d_entry.second->SetName(GetH1Name(h1d_entry.first));
    }

    for (auto h2d_entry : h2d) {
      h2d_entry.second->SetName(GetH2Name(h2d_entry.first));
    }
  }

  TH2D &operator[](const CorrelationIndex &index) {
    return *(h2d[index]);
  }

  TH1D &operator[](const AxisIndex &index) {
    return *(h1d[index]);
  }

  SHistoBuilder &operator=(const SHistoBuilder &other) {
    if (this == &other) {
      return *this;
    }

    // do a copy
    prefix = other.prefix + "_copy";
    axis_array = other.axis_array;

    for (auto h1d_entry : other.h1d) {
      auto new_name = GetH1Name(h1d_entry.first);
      auto *new_h1 = h1d_entry.second->Clone(new_name);
      h1d_entry.second = (TH1D *) new_h1;
      h1d.insert(h1d_entry);
    }

    for (auto h2d_entry : other.h2d) {
      auto new_name = GetH2Name(h2d_entry.first);
      auto *new_h2 = h2d_entry.second->Clone(new_name);
      h2d_entry.second = (TH2D *) new_h2;
      h2d.insert(h2d_entry);
    }

    dp_array.reserve(axis_array.size());

    return *this;
  }

 private:
  const char *GetH1Name(const AxisIndex &axisIndex) {
    return Form("%s_%s",
                prefix.c_str(), axis_array[axisIndex]->GetName()
    );
  }

  const char *GetH2Name(const CorrelationIndex &correlationIndex) {
    return Form("%s_%sVs%s",
                prefix.c_str(), axis_array[correlationIndex[1]]->GetName(), axis_array[correlationIndex[0]]->GetName()
    );
  }

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
