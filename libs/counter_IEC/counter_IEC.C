
//////////////// constructor ///////////////////////

counter_IEC::counter_IEC (const char *path): IEC(path)
{}
	
//////////// public functions /////////////////////

void counter_IEC::solve()
{
	int i, j, iteration = 0;
	double delta_T_water = 0.0, T_water_average_outlet, sum;
	
	dx = 1.0/(numberOfNodes-1);
    dy = dx;
	
	/////////// algorithm solution /////////////
	
	cout << "results of each iteration in the algorithm solution:" << endl << endl;
	
	do {
		
		iteration++;
		
		for (i=0;i<numberOfNodes;i++)
             T_water[i][0] = T_water[i][0] + delta_T_water;
             
        for (j=0;j<numberOfNodes;j++)
        {

            for (i=0;i<numberOfNodes-1;i++)
                T_primaryAir[i+1][j] =  ( dx * R * NTU / C_F ) * T_water[i][j]
                            +
                               ( 1.0 - ( dx * R * NTU / C_F  ) ) * T_primaryAir[i][j];

            for (i=0;i<numberOfNodes-1;i++)
            {
                T_secondaryAir[i+1][j] =  ( dx * NTU * ( 1.0/(1.0 + humidity_secondaryAir[i][j]*R_va) ) * ( 1.0 + R_va * wet_coeff *
                               ( humidity_v(T_water[numberOfNodes-1-i][j]) - humidity_secondaryAir[i][j] ) / Le ) ) * T_water[numberOfNodes-1-i][j]
                            +
                               ( 1.0 - dx * NTU * ( 1.0/(1.0 + humidity_secondaryAir[i][j]*R_va) ) * ( 1.0 + R_va * wet_coeff *
                               ( humidity_v(T_water[numberOfNodes-1-i][j]) - humidity_secondaryAir[i][j] ) / Le ) )  * T_secondaryAir[i][j] ;

                humidity_secondaryAir[i+1][j] =  ( dx * NTU * wet_coeff * humidity_v(T_water[numberOfNodes-1-i][j]) / Le)
                                   +
                                      ( 1.0 - ( dx * NTU * wet_coeff / Le  ) ) * humidity_secondaryAir[i][j] ;

            }


            if (j!=numberOfNodes-1)
                for (i=0;i<numberOfNodes;i++)

                    T_water[i][j+1] =  ( dy * NTU * R / C_w) * T_primaryAir[i][j]
                                +
                                   ( dy * NTU / C_w) * T_secondaryAir[numberOfNodes-1-i][j]
                                -
                                   ( dy * NTU * wet_coeff * ( humidity_v(T_water[i][j]) - humidity_secondaryAir[numberOfNodes-1-i][j] ) *
                                    h_0 / ( C_w * Le * c_ps) )
                                +
                                   ( 1.0 -  dy * NTU * ( R/C_w + 1.0/C_w + wet_coeff * ( R_va - R_wa ) *
                                   ( humidity_v(T_water[i][j]) - humidity_secondaryAir[numberOfNodes-1-i][j] ) / ( C_w * Le) ) ) * T_water[i][j];


        }
        
        sum = 0.0;
        for (i=0;i<numberOfNodes;i++)
           sum += T_water[i][numberOfNodes-1];

        T_water_average_outlet = 1.0*sum/numberOfNodes;

        delta_T_water = T_water_average_outlet - T_water[0][0];

        cout << "iteration = " << iteration << '\t' << '\t' << "delta_T_water = " << delta_T_water << endl;
 				
	}
	while (fabs(delta_T_water) > max_Error);
	
	cout << endl << "average of water temperature at outlet = " << T_water_average_outlet << endl;
    cout << "water temperature at inlet = " << T_water[0][0] << endl;
    cout << "number of iteration to solve = " << iteration << endl << endl;
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void counter_IEC::setBoundaryConditions()
{
	for (int j=0;j<numberOfNodes;j++)
    {
        T_primaryAir[0][j] = T_inlet_primaryAir;
        T_secondaryAir[0][j] = T_inlet_secondaryAir;
        humidity_secondaryAir[0][j] = humidity_inlet_secondaryAir;
    }

    for (int i=0;i<numberOfNodes;i++)
        T_water[i][0] = T_inlet_water;
        
    cout << "boundary conditions are set, and ready to solve." << endl << endl;
}

