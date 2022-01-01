
////////////////  abstract class /////////////////

class IEC {
	
	protected:
	
	///////////// protected members //////////////////////
	
	int numberOfNodes;
	
	double dx, dy;
	
    double L, width, distance_between_channels, m_dot_primaryAir, m_dot_secondaryAir, m_dot_water, Nu, max_Error,
           k_air, T_inlet_secondaryAir, T_inlet_primaryAir, T_inlet_water, humidity_inlet_secondaryAir;
           
    double NTU, R_va, R_wa, Le, R, C_F, C_w, wet_coeff, c_ps, c_pw, c_v, h_0, D_H, h_c, A_s; 
    
    double **T_secondaryAir, **T_primaryAir, **T_water, **humidity_secondaryAir;
    
    string coolerType; 
    
	       
   public:
   
	  IEC (const char *path);
	  
	  ~IEC ();
	  
      virtual void solve() = 0; 
      
      virtual void setBoundaryConditions() = 0; 
      
      virtual void readInputData(const char *path);
      
      virtual void printInputData();
      
      virtual void writeAllInformationOfCooler();
	  
      static IEC* New(const char *path);
      
      double** get_T_primaryAir();
      
      double** get_T_secondaryAir();
      
      double** get_T_water();
      
      double** get_humidity_secondaryAir();
      
      int get_numberOfNodes();
      
      double humidity_v(double T);

};

#include "../counter_IEC/counter_IEC.H"
#include "../cross_IEC/cross_IEC.H"
#include "../regenerative_IEC/regenerative_IEC.H"
#include "IEC.C"
