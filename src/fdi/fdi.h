#ifndef FDI_H
#define FDI_H

/**
    General purpose field decomposition library

    @author Francesco Salvadore , Stefano Tagliaventi
    @version 0.1 03/05/18 
*/


#include <array>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <iterator>
#include <memory>
#include "data.h"


namespace FDI{ 


  class fdi{
    public:

      fdi(std::vector<std::vector<unsigned int>> v,std::vector<int> d, enum DecompositionMode);
      fdi(std::vector<std::vector<unsigned int>> v);
      void Init(std::vector<std::vector<unsigned int>> v);

      void PrintLevels();
      void PrintOrderedMap();
      void PrintMap();
      

  private:

      void Split(const std::string& str, std::array<int,4> &key);

      std::string Key(std::array<int,4>k);

      void Refine(std::vector<unsigned int> ddl, std::string key_to_refine,bool erase_father);

      void BuildMap();

      std::unordered_map<std::string,Geometry> m_map;

      std::vector<std::vector<unsigned int>> m_levels;
      std::vector<std::array<int,3> > m_units;
      std::vector<int> m_domain_size;
      Geometry m_initial_domain;
      enum DecompositionMode m_decomposition_mode;

      int m_nlevels;

  };



};


#endif // FDI_H





