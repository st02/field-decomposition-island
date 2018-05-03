#include "fdi.h"
#include <sstream>

using namespace FDI;

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

void fdi::Init(std::vector<std::vector<unsigned int>> v){
  m_nlevels= v.size();  
  m_units.resize(m_nlevels);

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

fdi::fdi(std::vector<std::vector<unsigned int>> v,std::vector<int> d={1,1,1}, enum DecompositionMode decomp=Decompose):m_levels(v),m_decomposition_mode(decomp){
  m_initial_domain.coords={0,0,0,0};
  for(int i=0;i<3;i++)
    m_initial_domain.intervals[i]={0,d[i]-1,d[i]};
  Init(v);
}

fdi::fdi(std::vector<std::vector<unsigned int>> v):m_levels(v){
  Init(v);
}

void fdi::PrintLevels()
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

void fdi::Split(const std::string& str, std::array<int,4> &key)
{
  std::vector<std::string> k;
  std::istringstream iss(str);
  std::copy(std::istream_iterator<std::string>(iss),
      std::istream_iterator<std::string>(),
      std::back_inserter(k));
  for(int i=0;i<4;i++)
  {
    key[i] = std::stoi(k[i]);
  }
}

std::string fdi::Key(std::array<int,4>k)
{
  std::stringstream ks;
  ks << k[0] << " " << k[1] << " " << k[2] << " " << k[3]; 
  return ks.str(); 
}

void fdi::Refine(std::vector<unsigned int> ddl,
    std::string key_to_refine="0 0 0 0",bool erase_father=true)
{
  std::array<int,4> key;
  Split(key_to_refine,key); 
  Geometry parent = m_map[key_to_refine];

  for(int i=0 ;i<ddl[0];i++)
    for(int j=0 ;j<ddl[1];j++)
      for(int k=0 ;k<ddl[2];k++)
      {
        std::array<int,4> new_key;
        int level = new_key[0] = key[0] + 1;
        new_key[1] = key[1] + (i) * m_units[m_nlevels-1][0] / m_units[level-1][0];
        new_key[2] = key[2] + (j) * m_units[m_nlevels-1][1] / m_units[level-1][1];
        new_key[3] = key[3] + (k) * m_units[m_nlevels-1][2] / m_units[level-1][2];
        Geometry s_temp;
        s_temp.coords    = {new_key[0],new_key[1],new_key[2],new_key[3]};
        int levcoords[3]={i,j,k};
        int start[3];
        int end[3];
        int size[3];
        for(int c=0 ;c<3;c++){
          int sz  = parent.intervals[c][2]/ddl[c];
          int rem = parent.intervals[c][2]%ddl[c];
          if(levcoords[c] < rem){
            size[c] = sz + 1;      
            start[c] = parent.intervals[c][0] + sz * levcoords[c] + levcoords[c];
          }else{
            size[c] = sz;      
            start[c] = parent.intervals[c][0] + sz * levcoords[c] + rem;
          }
          end[c] = start[c] + size[c]-1;

          s_temp.intervals[c][0] = start[c];
          s_temp.intervals[c][1] = end[c];
          s_temp.intervals[c][2] = size[c];
        }
        m_map[Key(new_key)] = s_temp;
        
      }
  if(erase_father)
    m_map.erase(key_to_refine);
}

void fdi::PrintMap(){
  for( const auto& n : m_map ) 
    std::cout << "Key:[" << n.first << "] Value:" << n.second << std::endl;
}

void fdi::PrintOrderedMap(){


  /*
     std::string k=Key({m_nlevels,0,0,0});
     for(int i = 0 ; i < m_units[m_nlevels-1][1] ;i++){
     for(int j = 0 ; j < m_units[m_nlevels-1][0] ;j++){
     std::string k=Key({m_nlevels,i,j,0});
     if(m_map.count(k))
     std::cout << m_map[k]<<" ";
     }
     std::cout <<std::endl;
     }*/
}

void fdi::BuildMap(){
  m_map["0 0 0 0"] = m_initial_domain;
  //    refine(m_map,m_v[0],"0 0 0 0"); 
  //    refine(m_map,m_v[1],"1 8 0 0"); 
  int lv=0;
  for(auto &v: m_levels){
    std::vector<std::string > keys;
    for( const auto& n : m_map ) {
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








