
////////////// derived class "counter_IEC" ///////////////////////

class counter_IEC: public IEC {
	
	           
   public:
   
	   counter_IEC (const char *path);
	      
       void solve();
       
       void setBoundaryConditions();
		
};

#include "counter_IEC.C"
