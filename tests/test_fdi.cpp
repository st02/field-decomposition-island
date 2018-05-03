#include "CLI/CLI.hpp"
#include "fdi.h"
#include <regex>

int main(int argc,char *argv[]){
  int n_iter = 2;

  CLI::App app("test CLI");
  std::string dd="{1,1,1}";
  std::vector<int> domain={128,128,128};
  app.add_option("-t,--topology", dd, "two levels example: '{4,2,1} {2,2,1}'");
  app.add_option("-d,--domain", domain, "-d {128,128,128}");
  app.add_option("-i,--iter", n_iter, "-d {128,128,128}");
  CLI11_PARSE(app, argc, argv);
  std::vector<std::vector<unsigned int> > levels;
  std::regex pr("(\\{\\d+,\\d+,\\d+\\})");
  std::smatch pm;
  while(std::regex_search(dd,pm,pr))
  {
    std::string level = pm.str();
    dd=pm.suffix();
    std::regex pri("(\\d+)");
    std::smatch pmi;
    std::vector<unsigned int> v;
    while(std::regex_search(level,pmi,pri)){
      v.push_back(std::stoul(pmi.str()));
      level=pmi.suffix();
    }
    levels.push_back(v);
  }

  FDI::fdi mappa(levels,domain,FDI::Decompose);   
  mappa.PrintLevels();    
  mappa.PrintMap();  
  
}




