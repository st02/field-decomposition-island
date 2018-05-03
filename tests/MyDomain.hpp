#include<iostream>

class MyDomain{

  public:
    float ***u;
    float ***v;
    float ***w;
    float ***p;
    float max_grad_allowed=1.F;
    bool refine_needed=false;
    FDI::Geometry geom;
    ///  
    void allocate(std::vector<int> dims,std::array<float,3> length){
      for(int i=0;i<3;i++)
      {
        geom.intervals[i][0]=0;
        geom.intervals[i][1]=dims[i]-1;
        geom.intervals[i][2]=dims[i];
      }

      allocate3d(&u,dims,length);
      allocate3d(&v,dims,length);
      allocate3d(&w,dims,length);
      allocate3d(&p,dims,length);
    }
    void allocate3d(float **** v,std::vector<int> dims,std::array<float,3> length){
      size_t size= dims[0]*dims[1]*dims[2];
      float *data =(float*) malloc(size*sizeof(float)); 
      *v = (float ***)malloc(dims[0]*sizeof(float ***));
      for(int i=0;i<dims[0];i++) 
      {
        (*v)[i] = (float **) malloc(dims[1]*sizeof(float**));
        for(int j=0;j<dims[1];j++) 
        {
          (*v)[i][j] = data+dims[1]*dims[2]*i + dims[2]*j; 
        } 
      }
    }
    void init(){
      for(int i=0;i<geom.intervals[0][2];i++) 
        for(int j=0;j<geom.intervals[1][2];j++){ 
          for(int k=1;k<geom.intervals[2][2];k++) 
          {
            u[i][j][k] = 0.F;
            v[i][j][k] = 0.F;
            w[i][j][k] = 0.F;
            p[i][j][k] = 0.F;
          }
          u[i][j][0] = 1.F;
          v[i][j][0] = 2.F;
          w[i][j][0] = 3.F;
          p[i][j][0] = 4.F;
        }
      }
    void print(){
      for(int i=0;i<geom.intervals[0][2];i++) 
        for(int j=0;j<geom.intervals[1][2];j++){
          printf("u[%d][%d] ",i,j);
          for(int k=0;k<geom.intervals[2][2];k++){ 
            printf(" %f ",u[i][j][k]);
          }
          printf("\n");
        }
    }
    void evolve(){
      for(int i=1;i<geom.intervals[0][2]-2;i++) 
        for(int j=1;j<geom.intervals[1][2]-1;j++) 
          for(int k=1;k<geom.intervals[2][2]-1;k++) 
            u[i][j][k] = (u[i+1][j][k]+u[i-1][j][k]+u[i][j+1][k]+u[i][j-1][k]+u[i][j][k+1]+u[i][j][k-1])/6.F;
    }
    void check_refinement(){
      float max_grad=0.F;
      float cur_grad;
      for(int i=1;i<geom.intervals[0][2]-1;i++) 
        for(int j=1;j<geom.intervals[1][2]-1;j++) 
          for(int k=1;k<geom.intervals[2][2]-1;k++) 
          {
            cur_grad = (u[i+1][j][k]-u[i-1][j][k])*(u[i+1][j][k]-u[i-1][j][k])+
              (u[i][j+1][k]-u[i][j-1][k])*(u[i][j+1][k]-u[i][j-1][k])+
              (u[i][j][k+1]-u[i][j][k-1])*(u[i][j][k+1]-u[i][j][k-1]);
            if(cur_grad > max_grad) max_grad = cur_grad;
          }
      std::cout << "max_grad: " << max_grad << std::endl;
      if(max_grad > max_grad_allowed)
        refine_needed = true;
    }
};


