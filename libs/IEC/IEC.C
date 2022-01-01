
/////////////////////////////////// New function ///////////////////////////

IEC* IEC::New(const char *path)
{
	string coolerType;

	ifstream file(path);

	string searchForCoolerType = "coolerType";

	string lineOfText;

	for(;;)
	{
		getline(file, lineOfText);

		if (file.eof()) break;

		if (lineOfText.find(searchForCoolerType, 0) != string::npos)
		  file >> coolerType;
    }

    file.close();

    IEC* myObject = 0;

    if (coolerType=="counter")
    {
	    myObject = new counter_IEC(path);
	    return myObject;
    }

	else if (coolerType=="cross")
    {
	    myObject = new cross_IEC(path);
	    return myObject;
    }

    else if (coolerType=="regenerative")
    {
	    myObject = new regenerative_IEC(path);
	    return myObject;
    }

}

//////////////// constructor ///////////////////////

IEC::IEC (const char *path)
{
	readInputData(path);

	A_s = L*width;
	D_H = (2.0*L*distance_between_channels)/(L + distance_between_channels);
    h_c = (Nu * k_air)/D_H ;
    c_v = 1884.0;     // J/kg.K  at 300 K
    c_pw = 4200.0;    // J/kg.K
    c_ps = 1005.0 + c_v * humidity_inlet_secondaryAir;    // J/kg.K  at 300 K
    NTU = (h_c * A_s)/(m_dot_secondaryAir * c_ps);
    Le = 1.0;
    wet_coeff = 1.0;
    C_F = 1.0*m_dot_primaryAir/m_dot_secondaryAir;
    C_w = (m_dot_water * c_pw)/(m_dot_secondaryAir * c_ps);
    R = 1.0;
    h_0 = 2500.79 * 1.0e3;       // J/g * 1000 = J/kg
    R_va = 1.0*c_v/c_ps;
    R_wa = 1.0*c_pw/c_ps;

    T_primaryAir = new double *[numberOfNodes];
    T_secondaryAir = new double *[numberOfNodes];
    T_water = new double *[numberOfNodes];
    humidity_secondaryAir = new double *[numberOfNodes];
    for(int i = 0; i <numberOfNodes; i++)
    {
		T_primaryAir[i] = new double[numberOfNodes];
		T_secondaryAir[i] = new double[numberOfNodes];
		T_water[i] = new double[numberOfNodes];
		humidity_secondaryAir[i] = new double[numberOfNodes];
	}

	printInputData();
}

//////////////////////////////////   destructor  ///////////////////////////////////////////////////

IEC::~IEC ()
{
	for(int i = 0; i < numberOfNodes; ++i)
	{
         delete[] T_primaryAir[i];
         delete[] T_secondaryAir[i];
         delete[] T_water[i];
         delete[] humidity_secondaryAir[i];
	}

     delete[] T_primaryAir;
     delete[] T_secondaryAir;
     delete[] T_water;
     delete[] humidity_secondaryAir;
}

///////////// private function ////////////////////

double IEC::humidity_v(double T)
{
    double  p_atm, p_g;

    p_g = pow(10,(8.07131-(1730.63/(233.426+T) ) ) )*0.133322;

    p_atm = 1.0e2;

    return 0.622*(p_g/(p_atm - p_g));
}

//////////////////////////////// public functions /////////////////////////////////////

void IEC::writeAllInformationOfCooler()
{
	int i,j;

	#ifdef _WIN32

	if (mkdir("results") != -1)
        cout << "Directory of results was created." << endl;
        
	#endif
	
	#ifdef linux
	
	if (opendir("results"))
		system("rm -r results");

	if (mkdir("results", 0777) != -1)
        cout << "Directory of results was created." << endl;
        
	#endif

	ofstream MyFile1("results/T_primaryAirInYdirectionAtX=1.txt");

    MyFile1<<"Y (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (j=0;j<numberOfNodes;j++)
        MyFile1 << std::fixed << std::setprecision(6) << j*dy << '\t' << '\t' << T_primaryAir[numberOfNodes-1][j] << endl;

    MyFile1.close();

    ofstream MyFile2("results/T_primaryAirInXdirectionAtY=0.txt");

	MyFile2<<"X (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (i=0;i<numberOfNodes;i++)
        MyFile2 << std::fixed << std::setprecision(6) << i*dx << '\t' << '\t' << T_primaryAir[i][0] << endl;

    MyFile2.close();

    ofstream MyFile3("results/T_primaryAirInXdirectionAtY=1.txt");

	MyFile3<<"X (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (i=0;i<numberOfNodes;i++)
        MyFile3 << std::fixed << std::setprecision(6) << i*dx << '\t' << '\t' << T_primaryAir[i][numberOfNodes-1] << endl;

    MyFile3.close();

    ofstream MyFile4("results/T_primaryAirInYdirectionAtX=0.txt");

	MyFile4<<"Y (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (j=0;j<numberOfNodes;j++)
        MyFile4 << std::fixed << std::setprecision(6) << j*dy << '\t' << '\t' << T_primaryAir[0][j] << endl;

    MyFile4.close();

    ofstream MyFile5("results/T_secondaryAirInYdirectionAtX=0.txt");

	MyFile5<<"Y (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (j=0;j<numberOfNodes;j++)
        MyFile5 << std::fixed << std::setprecision(6) << j*dy << '\t' << '\t' << T_secondaryAir[numberOfNodes-1][j] << endl;

    MyFile5.close();

    ofstream MyFile6("results/T_secondaryAirInXdirectionAtY=1.txt");

	MyFile6<<"X (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (i=0;i<numberOfNodes;i++)
        MyFile6 << std::fixed << std::setprecision(6) << i*dx << '\t' << '\t' << T_secondaryAir[numberOfNodes-1-i][numberOfNodes-1] << endl;

    MyFile6.close();

    ofstream MyFile7("results/T_secondaryAirInYdirectionAtX=1.txt");

	MyFile7<<"Y (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (j=0;j<numberOfNodes;j++)
        MyFile7 << std::fixed << std::setprecision(6) << j*dy << '\t' << '\t' << T_secondaryAir[0][j] << endl;

    MyFile7.close();

    ofstream MyFile8("results/T_secondaryAirInXdirectionAtY=0.txt");

	MyFile8<<"X (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (i=0;i<numberOfNodes;i++)
        MyFile8 << std::fixed << std::setprecision(6) << i*dx << '\t' << '\t' << T_secondaryAir[numberOfNodes-1-i][0] << endl;

    MyFile8.close();

    ofstream MyFile9("results/T_waterInYdirectionAtX=0.txt");

	MyFile9<<"Y (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (j=0;j<numberOfNodes;j++)
        MyFile9 << std::fixed << std::setprecision(6) << j*dy << '\t' << '\t' << T_water[0][j] << endl;

    MyFile9.close();

    ofstream MyFile10("results/T_waterInXdirectionAtY=1.txt");

	MyFile10<<"X (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (i=0;i<numberOfNodes;i++)
        MyFile10 << std::fixed << std::setprecision(6) << i*dx << '\t' << '\t' << T_water[i][numberOfNodes-1] << endl;

    MyFile10.close();

    ofstream MyFile11("results/T_waterInXdirectionAtY=0.txt");

	MyFile11<<"X (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (i=0;i<numberOfNodes;i++)
        MyFile11 << std::fixed << std::setprecision(6) << i*dx << '\t' << '\t' << T_water[i][0] << endl;

    MyFile11.close();

    ofstream MyFile12("results/T_waterInYdirectionAtX=1.txt");

	MyFile12<<"Y (dimensionless)"<<'\t'<<"temperature (C)" << endl << endl;
    for (j=0;j<numberOfNodes;j++)
        MyFile12 << std::fixed << std::setprecision(6) << j*dy << '\t' << '\t' << T_water[numberOfNodes-1][j] << endl;

    MyFile12.close();

    ofstream MyFile13("results/humidity_secondaryAirInYdirectionAtX=0.txt");

	MyFile13<<"Y (dimensionless)"<<'\t'<<"humidity ratio (g/kg_dryAir)" << endl << endl;
    for (j=0;j<numberOfNodes;j++)
        MyFile13 << std::fixed << std::setprecision(6) << j*dy << '\t' << '\t' << humidity_secondaryAir[numberOfNodes-1][j] << endl;

    MyFile13.close();

    ofstream MyFile14("results/humidity_secondaryAirInXdirectionAtY=1.txt");

	MyFile14<<"X (dimensionless)"<<'\t'<<"humidity ratio (g/kg_dryAir)" << endl << endl;
    for (i=0;i<numberOfNodes;i++)
        MyFile14 << std::fixed << std::setprecision(6) << i*dx << '\t' << '\t' << humidity_secondaryAir[numberOfNodes-1-i][numberOfNodes-1] << endl;

    MyFile14.close();

    ofstream MyFile15("results/humidity_secondaryAirInYdirectionAtX=1.txt");

	MyFile15<<"Y (dimensionless)"<<'\t'<<"humidity ratio (g/kg_dryAir)" << endl << endl;
    for (j=0;j<numberOfNodes;j++)
        MyFile15 << std::fixed << std::setprecision(6) << j*dy << '\t' << '\t' << humidity_secondaryAir[0][j] << endl;

    MyFile15.close();

    ofstream MyFile16("results/humidity_secondaryAirInXdirectionAtY=0.txt");

	MyFile16<<"X (dimensionless)"<<'\t'<<"humidity ratio (g/kg_dryAir)" << endl << endl;
    for (i=0;i<numberOfNodes;i++)
        MyFile16 << std::fixed << std::setprecision(6) << i*dx << '\t' << '\t' << humidity_secondaryAir[numberOfNodes-1-i][0] << endl;

    MyFile16.close();

    cout << "result data was written." << endl << endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void IEC::readInputData(const char *path)
{
	ifstream file(path);

	string searchForChannelLength = "channelLength";
	string searchForChannelWidth = "channelWidth";
	string searchForDistanceBetweenChannels = "distanceBetweenChannels";
	string searchForMdotPrimaryAir = "mDotPrimaryAir";
	string searchForMdotSecondaryAir = "mDotSecondaryAir";
	string searchForMdotWater = "mDotWater";
	string searchForNusseltNumber = "nusseltNumber";
	string searchForThermalConductivityOfAir = "thermalConductivityOfAir";
	string searchForTinletForPrimaryAir = "inletTemperatureForPrimaryAir";
	string searchForTinletForSecondaryAir = "inletTemperatureForSecondaryAir";
	string searchForTinletForWater = "waterTemperatureGuessedAtInlet";
	string searchForHumidityInletForSecondaryAir = "inletHumidityForSecondaryAir";
	string searchForCoolerType = "coolerType";
	string searchForNumberOfNodes = "numberOfNodesInBothDirection";
	string searchForMaxError = "maximumErrorForSolving";


	string lineOfText;

	for(;;)
	{
		getline(file, lineOfText);

		if (file.eof()) break;

		if (lineOfText.find(searchForCoolerType, 0) != string::npos)
		  file >> coolerType;

		if (lineOfText.find(searchForChannelLength, 0) != string::npos)
		  file >> L;

		if (lineOfText.find(searchForChannelWidth, 0) != string::npos)
		  file >> width;

		if (lineOfText.find(searchForDistanceBetweenChannels, 0) != string::npos)
		  file >> distance_between_channels;

		if (lineOfText.find(searchForMdotPrimaryAir, 0) != string::npos)
		  file >> m_dot_primaryAir;

		if (lineOfText.find(searchForMdotSecondaryAir, 0) != string::npos)
		  file >> m_dot_secondaryAir;

		if (lineOfText.find(searchForMdotWater, 0) != string::npos)
		  file >> m_dot_water;

		if (lineOfText.find(searchForNusseltNumber, 0) != string::npos)
		  file >> Nu;

		if (lineOfText.find(searchForThermalConductivityOfAir, 0) != string::npos)
		  file >> k_air;

		if (lineOfText.find(searchForTinletForPrimaryAir, 0) != string::npos)
		  file >> T_inlet_primaryAir;

		if (lineOfText.find(searchForTinletForWater, 0) != string::npos)
		  file >> T_inlet_water;

		if (lineOfText.find(searchForHumidityInletForSecondaryAir, 0) != string::npos)
		  file >> humidity_inlet_secondaryAir;

		if (lineOfText.find(searchForNumberOfNodes, 0) != string::npos)
		  file >> numberOfNodes;

		if (lineOfText.find(searchForMaxError, 0) != string::npos)
		  file >> max_Error;
    }

    file.close();

    if ( (coolerType == "counter") || (coolerType == "cross") )
    {

		ifstream file_(path);

		string searchForTinletForSecondaryAir = "inletTemperatureForSecondaryAir";

		string lineOfText;

		for(;;)
		{
			getline(file_, lineOfText);

			if (file_.eof()) break;

			if (lineOfText.find(searchForTinletForSecondaryAir, 0) != string::npos)
				file_ >> T_inlet_secondaryAir;

		}

		file_.close();
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void IEC::printInputData()
{
	cout << "input data for the simulation: " << endl << endl;
	cout << "cooler type is: " << coolerType << endl;
	cout << "channel length is: " << L << endl;
	cout << "channel width is: " << width << endl;
	cout << "distance between channels is: " << distance_between_channels << endl;
	cout << "m dot for primary air is: " << m_dot_primaryAir << endl;
	cout << "m dot for secondary air is: " << m_dot_secondaryAir << endl;
	cout << "m dot for water air is: " << m_dot_water << endl;
	cout << "Nusselt number is: " << Nu << endl;
	cout << "thermal conductivity of air is: " << k_air << endl;
	cout << "inlet temperature for primary air is: " << T_inlet_primaryAir << endl;

	if ( (coolerType == "counter") || (coolerType == "cross") )
		cout << "inlet temperature for secondary air is: " << T_inlet_secondaryAir << endl;

	cout << "water temperature guessed at inlet is: " << T_inlet_water << endl;
	cout << "inlet humidity for secondary air is: " << humidity_inlet_secondaryAir << endl;
	cout << "number of nodes in both direction is: " << numberOfNodes << endl;
	cout << "maximum error for solving: " << max_Error << endl << endl;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

double** IEC::get_T_primaryAir()
{
	return T_primaryAir;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

double** IEC::get_T_secondaryAir()
{
	return T_secondaryAir;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

double** IEC::get_T_water()
{
	return T_water;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

double** IEC::get_humidity_secondaryAir()
{
	return humidity_secondaryAir;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int IEC::get_numberOfNodes()
{
	return numberOfNodes;
}

