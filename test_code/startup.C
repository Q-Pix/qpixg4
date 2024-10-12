#include "TTree.h"

// use with hadd as: LD_PRELOAD=startup_C.so hadd output.root input_*

int startup() {
  TTree::SetMaxTreeSize( 1000000000000LL ); // 1 TB
  return 0;
}

namespace {
  static int i = startup();
}
