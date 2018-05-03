#ifndef FDI_HPP
#define FDI_HPP
#include <array>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <iterator>
#include <memory>
#include "data.h"

namespace FDI{


  std::ostream & operator<<(std::ostream &os ,const  Geometry &s){
    os<< "[ ";  
    std::copy (std::begin(s.coords), std::end(s.coords), std::ostream_iterator<int>(os, " ") ); 
    os <<"] ";
    for(int i=0;i<3;i++){
      os<<"{ " ;
      std::copy (std::begin(s.intervals[i]), std::end(s.intervals[i]), std::ostream_iterator<int>(os, "\t") ); 
      os<<" } " ;
    }
    return os;
  }

  template<typename D>
    class fdi{
      public:

        fdi(std::shared_ptr<D> domain,
                std::vector<std::vector<unsigned int>> v,
                std::vector<int> d={1,1,1}, 
                enum DecompositionMode decomp=Refinement):
                m_initial_domain(domain),m_levels(v),m_decomposition_mode(decomp){
                  Init(v);
                  (*m_map)["0 0 0 0"] = m_initial_domain;
          }

        fdi(std::vector<std::vector<unsigned int>> v,
            std::vector<int> d={1,1,1}, 
            enum DecompositionMode decomp=Decompose);

        void Init(std::vector<std::vector<unsigned int>> v);

        void PrintLevels();

        void PrintGeoMap(){
          for( auto n : *m_map ) 
              std::cout << "Key:[" << n.first << "] Value:" << n.second->geom << std::endl;
        }

        void Refine(std::vector<unsigned int> ddl, std::string key_to_refine="0 0 0 0",bool erase_father=true);

        void Refine(){
          int lv=0;
          for(auto &v: m_levels){
            std::vector<std::string > keys;
            for(auto n : *m_map ) {
              if(n.second->refine_needed)
                keys.push_back(n.first);
            }
            lv++;
            for(auto &k:keys)
              Refine(v,k,true);
          }
        }

        std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<D> > > Mumap(){
          return m_map;
        }

      private:

        void Split(const std::string& str, std::array<int,4> &key){
          std::vector<std::string> k;
          std::istringstream iss(str);
          std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(k));
          for(int i=0;i<4;i++)
            key[i] = std::stoi(k[i]);
        }

        std::string Key(std::array<int,4>k) {
          std::stringstream ks;
          ks << k[0] << " " << k[1] << " " << k[2] << " " << k[3]; 
          return ks.str(); 
        }

        void BuildMap();

        std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<D>> > m_map;

        std::vector<std::vector<unsigned int>> m_levels;
        std::vector<std::array<int,3> > m_units;
        std::vector<int> m_domain_size;
        std::shared_ptr<D> m_initial_domain;
        enum DecompositionMode m_decomposition_mode;
        int m_nlevels;
    };

  template<typename D>
    fdi<D>::fdi(std::vector<std::vector<unsigned int>> v,std::vector<int> d, enum DecompositionMode decomp):m_levels(v),m_decomposition_mode(decomp){
      m_initial_domain.geom.coords={0,0,0,0};
      for(int i=0;i<3;i++)
        m_initial_domain.geom.intervals[i]={0,d[i]-1,d[i]};
      Init(v);
    }

  template<typename D>
    void fdi<D>::Init(std::vector<std::vector<unsigned int>> v){
      m_nlevels= v.size();  
      m_units.resize(m_nlevels);

      m_map=std::make_shared<std::unordered_map<std::string,std::shared_ptr<D>>>();

      for(auto &d:m_units)
        d={1,1,1};

      int i=0;
      for(auto &d:v)
      {
        if(i==0){
          for(int j=0;j<3;j++ )
            m_units[i][j] = d[j];
        } else {
          for(int j=0;j<3;j++ )
            m_units[i][j] *= m_units[i-1][j]*d[j];
        }
        i++; 
      }
      if(m_decomposition_mode==Decompose)
        BuildMap();
    }


  template<typename D>
    void fdi<D>::PrintLevels()
    {
      int nlevels = m_levels.size();
      std::cout << "number of levels = "<<nlevels<<std::endl;
      for(auto &v:m_levels)
      {
        for(auto &l:v)
          std::cout << " " << l;
        std::cout << std::endl;
      } 
      std::cout << "units "<<std::endl;
      for(auto &v:m_units){
        for(auto &i:v)
          std::cout <<  i << " ";
        std::cout << std::endl;   
      }
    }

  template<typename D>
    void fdi<D>::Refine(std::vector<unsigned int> ddl, std::string key_to_refine,bool erase_father)
    {
      std::array<int,4> key;
      Split(key_to_refine,key); 
      std::shared_ptr<D> parent = (*m_map)[key_to_refine];

      for(int i=0 ;i<ddl[0];i++)
        for(int j=0 ;j<ddl[1];j++)
          for(int k=0 ;k<ddl[2];k++)
          {
            std::array<int,4> new_key;
            int level = new_key[0] = key[0] + 1;
            new_key[1] = key[1] + (i) * m_units[m_nlevels-1][0] / m_units[level-1][0];
            new_key[2] = key[2] + (j) * m_units[m_nlevels-1][1] / m_units[level-1][1];
            new_key[3] = key[3] + (k) * m_units[m_nlevels-1][2] / m_units[level-1][2];
            std::shared_ptr<D> s_temp=std::make_shared<D>();
            s_temp->geom.coords    = {new_key[0],new_key[1],new_key[2],new_key[3]};
            int levcoords[3]={i,j,k};
            int start[3];
            int end[3];
            int size[3];
            for(int c=0 ;c<3;c++){
              int sz  = parent->geom.intervals[c][2]/ddl[c];
              int rem = parent->geom.intervals[c][2]%ddl[c];
              if(levcoords[c] < rem){
                size[c] = sz + 1;      
                start[c] = parent->geom.intervals[c][0] + sz * levcoords[c] + levcoords[c];
              }else{
                size[c] = sz;      
                start[c] = parent->geom.intervals[c][0] + sz * levcoords[c] + rem;
              }
              end[c] = start[c] + size[c]-1;

              s_temp->geom.intervals[c][0] = start[c];
              s_temp->geom.intervals[c][1] = end[c];
              s_temp->geom.intervals[c][2] = size[c];
            }
            (*m_map)[Key(new_key)] = s_temp;
          }
      if(erase_father)
        m_map->erase(key_to_refine);
    }



  template<typename D>
    void fdi<D>::BuildMap(){
      (*m_map)["0 0 0 0"] = m_initial_domain;
      int lv=0;
      for(auto &v: m_levels){
        std::vector<std::string > keys;
        for(auto  n : *m_map ) {
          std::array<int,4> key;
          Split(n.first,key); 
          if(key[0]==lv)
            keys.push_back(n.first);
        }
        lv++;
        for(auto &k:keys)
          Refine(v,k,false);
      }
    }

};

#endif // FDI_HPP

