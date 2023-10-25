// ## Preparation
#include <numeric>      // std::iota
#include <algorithm>    // std::sort, std::stable_sort


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

// A simple helper function to fill a test tree: this makes the example
// stand-alone.
void fill_tree(const char *treeName, const char *fileName)
{
   ROOT::RDataFrame d(10);
   int i(0);
   d.Define("b1", [&i]() { return (double)i; })
      .Define("b2",
              [&i]() {
                 auto j = i * i;
                 ++i;
                 return j;
              })
      .Snapshot(treeName, fileName);
}

int dfMacro()
{

   // We prepare an input tree to run on
   std::vector<std::string> fileNames;
   fileNames.push_back("Ar42_Long_Decay_2.root");
   fileNames.push_back("Ar42_Long_Decay_3.root");
   auto fileName = " Ar42_Long_Decay_2.root";
   auto treeName = "event_tree";

   // fill_tree(treeName, fileName);

   ROOT::RDataFrame df(treeName, fileNames);

   // get all of the columns that we care about, find the index we want to sort
   // on, then dump this all sorted into a new TTree
   auto start_hits_x = df.Take<std::vector<double>>("hit_start_x");
   auto start_hits_y = df.Take<std::vector<double>>("hit_start_y");
   auto start_hits_z = df.Take<std::vector<double>>("hit_start_z");
   auto start_hits_t = df.Take<std::vector<double>>("hit_start_t");

   auto end_hits_x = df.Take<std::vector<double>>("hit_end_x");
   auto end_hits_y = df.Take<std::vector<double>>("hit_end_y");
   auto end_hits_z = df.Take<std::vector<double>>("hit_end_t");
   auto end_hits_t = df.Take<std::vector<double>>("hit_end_t");

   auto hit_energy = df.Take<std::vector<double>>("hit_energy_deposit");
   auto hit_length = df.Take<std::vector<double>>("hit_length");
   auto hit_id = df.Take<std::vector<int>>("hit_track_id");

   auto n_hits = df.Take<int>("number_hits");
   auto n_parts = df.Take<int>("number_particles");
   auto n_energy = df.Take<double>("energy_deposit");

   int i=0;
   std::vector<double> hit_zero;
   for(auto hit : start_hits_t)
   {
     hit_zero.push_back(hit[0]);
     if(hit[1] < hit[0])
       std::cout << "WARNING time moving backwards? "<< i << hit[1] << ", " << hit[0] <<"\n";
     i++;
   }
   std::vector<size_t> hit_sort_index = sort_indexes(hit_zero);

   i = 0;
   for(auto j : hit_sort_index){
     // std::cout << i << "- hit sort index is: "<< j << ", time: " << start_hits_t->at(j)[0] << std::endl;
     ++i;
   }

   // df.Display("hit_end_t")->Print();
   
   return 0;
}
