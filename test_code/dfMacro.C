// ## Preparation
#include <numeric>      // std::iota
#include <algorithm>    // std::sort, std::stable_sort
#include <memory>

// ROOT includes
#include "TTree.h"
#include "TFile.h"
#include <ROOT/RDataFrame.hxx>



template <typename T>
std::vector<size_t> sort_indexes(const vector<T> &v) {

  // initialize original index locations
  std::vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  // sort indexes based on comparing values in v
  // using std::stable_sort instead of std::sort
  // to avoid unnecessary index re-orderings
  // when v contains elements of equal values
  std::stable_sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

  return idx;
}

int dfMacro()
{
   auto treeName = "event_tree";
   ROOT::RDataFrame df(treeName, "/home/argon/Projects/Kevin/qpixg4/output/*core-0*root");
   std::cout << "loaded the dataframe..\n";

   int entries = df.Count().GetValue();
   std::cout << "found total entries int the rdf: " << entries << std::endl;
   
   return 0;
}
