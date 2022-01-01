
/////////////////////// derived class "cross_IEC" ///////////////////////////

class cross_IEC: public IEC {
	
	           
   public:
   
	   cross_IEC (const char *path);
	      
       void solve();
       
       void setBoundaryConditions();
		
};

#include "cross_IEC.C"
