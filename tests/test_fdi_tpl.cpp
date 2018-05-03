#include "CLI/CLI.hpp"
#include "fdi.hpp"
#include <regex>
#include "MyDomain.hpp"

int main(int argc,char *argv[]){
  int n_iter = 2;

  CLI::App app("test CLI");
  std::string dd="{1,1,1}";
  std::vector<int> domain={128,128,128};
  app.add_option("-t,--topology", dd, "two levels example: '{4,2,1} {2,2,1}'");
  app.add_option("-d,--domain", domain, "-d {128,128,128}");
  app.add_option("-i,--itert", n_iter, "-d {128,128,128}");
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

  


  std::shared_ptr<MyDomain> myDomain=std::make_shared<MyDomain>();
  myDomain->allocate(domain,{2.f,3.f,4.f});
  myDomain->init();
//  myDomain.print();
  FDI::fdi<MyDomain> mymap(myDomain,levels,domain,FDI::Refinement);
  mymap.PrintGeoMap();

  for(int it=0;it<n_iter;it++) {
    std::cout << "it:" << it << std::endl;

    for(auto &m: *mymap.Mumap()) 
      m.second->check_refinement(); 

    mymap.Refine();

    mymap.PrintGeoMap();
    for(auto &m: *mymap.Mumap()) 
      m.second->evolve(); 

  }
}




