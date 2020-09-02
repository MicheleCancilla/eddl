/*
* EDDL Library - European Distributed Deep Learning Library.
* Version: 0.7
* copyright (c) 2020, Universidad Politécnica de Valencia (UPV), PRHLT Research Centre
* Date: April 2020
* Author: PRHLT Research Centre, UPV, (rparedes@prhlt.upv.es), (jon@prhlt.upv.es)
* All rights reserved
*/

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <thread>

#include <stdexcept>
#include "eddl/net/compserv.h"

CompServ::CompServ()
{

}

// for local
CompServ::CompServ(int t, const vector<int> g, const vector<int> &f,int lsb, int mem) {
    type = "local";
    isshared=false;

    if (t==-1) local_threads = std::thread::hardware_concurrency();  // Avoid eigen dependency
    else local_threads = t;

    local_gpus = vector<int>(g.begin(), g.end());
    local_fpgas = vector<int>(f.begin(), f.end());

    this->lsb=lsb;

    if (lsb<0) {
      throw std::runtime_error("Error creating CS with lsb<0 in CompServ::CompServ");
    }

    mem_level=mem;
    if ((mem<0)||(mem>2)) {
      fprintf(stderr,"Error creating CS with incorrect memory saving level param in CompServ::CompServ");
      exit(EXIT_FAILURE);
    }
    else {
      if (mem==0) fprintf(stderr,"CS with full memory setup\n");
      if (mem==1) fprintf(stderr,"CS with mid memory setup\n");
      if (mem==2) fprintf(stderr,"CS with low memory setup\n");
    }

}

CompServ * CompServ::share() {
  CompServ *n=new CompServ();

  n->type=type;
  n->local_threads=local_threads;
  n->local_gpus=local_gpus;
  n->local_fpgas=local_fpgas;
  n->lsb=lsb;
  n->isshared=true;
  n->mem_level=mem_level;

  return n;
}



// for Distributed
CompServ::CompServ(string filename) {
     //TODO: Implement
}
