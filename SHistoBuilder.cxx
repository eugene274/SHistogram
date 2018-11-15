//
// Created by eugene on 11/13/18.
//

#include "SHistoBuilder.h"

int SHistoBuilder::AddAxis(const char *name, int nbins, double xmin, double xmax, double *dp, const char *title) {
  auto *axis = new TAxis(nbins, xmin, xmax);
  axis->SetName(name);
  axis->SetTitle(title);

  axis_array.push_back(axis);
  dp_array.push_back(dp);

  return (int) axis_array.size() - 1;
}
bool SHistoBuilder::HasCorrelation(const SHistoBuilder::CorrelationIndex &correlationIndex) {
  return h2d.find(correlationIndex) != h2d.end();
}

SHistoBuilder::CorrelationIndex SHistoBuilder::AddCorrelation(AxisIndex a1, AxisIndex a2) {

  CorrelationIndex correlationIndex({a1, a2});

  auto *a1_ax = axis_array[a1];
  auto *a2_ax = axis_array[a2];

  if (HasCorrelation(correlationIndex)) {
    Warning("AddCorrelation",
            Form("CorrelationIndex of %s and %s is already exits. Skipping...",
                 a1_ax->GetName(),
                 a2_ax->GetName()
            ));
    return correlationIndex;
  }
  const char *h_name = Form("%s_%sVs%s", prefix.c_str(), a2_ax->GetName(), a1_ax->GetName());
  const char *h_title = Form("%s vs %s", a2_ax->GetTitle(), a1_ax->GetTitle());
  auto *hist = new TH2D(h_name, h_title,
                        a1_ax->GetNbins(), a1_ax->GetXmin(), a1_ax->GetXmax(),
                        a2_ax->GetNbins(), a2_ax->GetXmin(), a2_ax->GetXmax()
  );

  h2d.insert({correlationIndex, hist});

  hist->SetXTitle(a1_ax->GetTitle());
  hist->SetYTitle(a2_ax->GetTitle());

  return correlationIndex;
}
void SHistoBuilder::AddAllCorrelations() {
  for (int i = 0; i < axis_array.size(); ++i) {
    for (int j = 0; j < axis_array.size(); ++j) {
      if (i == j) continue;

      AddCorrelation(i, j);
    }
  }
}
void SHistoBuilder::Fill() {
  for (auto h1d_entry : h1d) {
    AxisIndex ax_id = h1d_entry.first;

    double val = *(dp_array[ax_id]);
    h1d_entry.second->Fill(val);
  }

  for (auto h2d_entry : h2d) {
    AxisIndex ax1_id = h2d_entry.first[0];
    AxisIndex ax2_id = h2d_entry.first[1];
    auto *hist = h2d_entry.second;

    double ax1_val = *(dp_array[ax1_id]);
    double ax2_val = *(dp_array[ax2_id]);

    hist->Fill(ax1_val, ax2_val);
  }
}

SHistoBuilder::AxisIndex SHistoBuilder::AddHistogram(const int index) {
  AxisIndex axisIndex = index;

  auto *axis = axis_array[axisIndex];

  if (h1d.find(axisIndex) != h1d.end()) {
    Warning("AddHistogram", Form("AxisIndex of %s is already exits. Skipping...", axis->GetName()));
    return axisIndex;
  }

  const char *h_name = Form("%s_%s", prefix.c_str(), axis->GetName());
  auto *hist = new TH1D(h_name, axis->GetTitle(), axis->GetNbins(), axis->GetXmin(), axis->GetXmax());
  hist->SetXTitle(axis->GetTitle());
  h1d.insert({axisIndex, hist});

  return axisIndex;
}

void SHistoBuilder::AddAllHistograms() {
  for (int i = 0; i < axis_array.size(); ++i) {
    AddHistogram(i);
  }
}

std::vector<SHistoBuilder::CorrelationIndex> SHistoBuilder::AddCorrelations(std::vector<SHistoBuilder::CorrelationIndex> correlations) {

  for (auto correlation : correlations) {
    AddCorrelation(correlation);
  }

  return correlations;
}
SHistoBuilder::CorrelationIndex SHistoBuilder::AddCorrelation(SHistoBuilder::CorrelationIndex correlationIndex) {
  return AddCorrelation(correlationIndex[0], correlationIndex[1]);
}
