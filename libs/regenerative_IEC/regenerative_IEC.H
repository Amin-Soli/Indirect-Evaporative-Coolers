
////////////// derived class "regenerative_IEC" ///////////////////////

class regenerative_IEC: public IEC {
	
	           
   public:
   
	   regenerative_IEC (const char *path);
	      
       void solve();
       
       void setBoundaryConditions();
		
};

#include "regenerative_IEC.C"
