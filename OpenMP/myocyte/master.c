//=====================================================================
//	MAIN FUNCTION
//=====================================================================
#define fp float

void master(fp timeinst, fp* initvalu, fp* parameter, fp* finavalu, int mode){

	//=====================================================================
	//	VARIABLES
	//=====================================================================

	// counters
	int i;

	// intermediate output on host
	fp JCaDyad;
	fp JCaSL;
	fp JCaCyt;

	// offset pointers
	int initvalu_offset_batch;//
	int initvalu_offset_ecc;	// 46 points
	int parameter_offset_ecc;
	int initvalu_offset_Dyad;															// 15 points
	int parameter_offset_Dyad;
	int initvalu_offset_SL;																// 15 points
	int parameter_offset_SL;
	int initvalu_offset_Cyt;																// 15 poitns
	int parameter_offset_Cyt;

			// module parameters
	fp CaDyad;	// from ECC model, *** Converting from [mM] to [uM] ***
	fp CaSL;	// from ECC model, *** Converting from [mM] to [uM] ***
	fp CaCyt;	// from ECC model, *** Converting from [mM] to [uM] ***

		// thread counters
		int th_id, nthreads;
		int th_count[4];
		int temp;

	//=====================================================================
	//	KERNELS FOR 1 WORKLOAD - PARALLEL
	//=====================================================================

	nthreads = omp_get_max_threads();

	if(mode == 0){

		// partition workload between threads
		temp = 0;
		for(i=0; i<4; i++){													// do for all 4 pieces of work
			if(temp>=nthreads){											// limit according to number of threads
				temp = 0;
			}
			th_count[i] = temp;												// assign thread to piece of work
			temp = temp +1;
		}

		// run pieces of work in parallel
		#pragma omp parallel private(th_id)
		{

			if (th_id == th_count[1]) {

				// ecc function
				initvalu_offset_ecc = 0;												// 46 points
				parameter_offset_ecc = 0;
				ecc(timeinst, initvalu, initvalu_offset_ecc, parameter, parameter_offset_ecc, finavalu);

			}

			if (th_id == th_count[2]) {

				// cam function for Dyad
				initvalu_offset_Dyad = 46;	// 15 points
				parameter_offset_Dyad = 1;
				CaDyad = initvalu[35]*1e3;// from ECC model, *** Converting from [mM] to [uM] ***
				JCaDyad = cam(timeinst,
											initvalu,
											initvalu_offset_Dyad,
											parameter,
											parameter_offset_Dyad,
											finavalu,
											CaDyad);

			}

			if (th_id == th_count[3]) {

				// cam function for SL
				initvalu_offset_SL = 61;											// 15 points
				parameter_offset_SL = 6;
				CaSL = initvalu[36]*1e3;											// from ECC model, *** Converting from [mM] to [uM] ***
				JCaSL = cam(		timeinst,
											initvalu,
											initvalu_offset_SL,
											parameter,
											parameter_offset_SL,
											finavalu,
											CaSL);

			}

			if (th_id == th_count[4]) {

				// cam function for Cyt
				initvalu_offset_Cyt = 76;												// 15 poitns
				parameter_offset_Cyt = 11;
				CaCyt = initvalu[37]*1e3;											// from ECC model, *** Converting from [mM] to [uM] ***
				JCaCyt = cam(	timeinst,
											initvalu,
											initvalu_offset_Cyt,
											parameter,
											parameter_offset_Cyt,
											finavalu,
											CaCyt);

			}

		}

	}

	//=====================================================================
	//	KERNELS FOR MANY WORKLOAD - SERIAL
	//=====================================================================

	else{

		/*printf("made it here master mode 1\n");
		fflush(stdout);*/

		// ecc function
		initvalu_offset_ecc = 0;		// 46 points
		parameter_offset_ecc = 0;
		ecc(timeinst, initvalu, initvalu_offset_ecc, parameter, parameter_offset_ecc, finavalu);

		/*printf("made it here master mode 2\n");
		fflush(stdout);*/

		// cam function for Dyad
		initvalu_offset_Dyad = 46;		// 15 points
		parameter_offset_Dyad = 1;
		CaDyad = initvalu[35]*1e3;	// from ECC model, *** Converting from [mM] to [uM] ***
		JCaDyad = cam(timeinst,
									initvalu,
									initvalu_offset_Dyad,
									parameter,
									parameter_offset_Dyad,
									finavalu,
									CaDyad);

		/*printf("made it here master mode 3\n");
		fflush(stdout);*/

		// cam function for SL
		initvalu_offset_SL = 61;	// 15 points
		parameter_offset_SL = 6;
		CaSL = initvalu[36]*1e3;			// from ECC model, *** Converting from [mM] to [uM] ***
		JCaSL = cam(timeinst,
									initvalu,
									initvalu_offset_SL,
									parameter,
									parameter_offset_SL,
									finavalu,
									CaSL);

		/*printf("made it here master mode 4\n");
		fflush(stdout);*/
		// cam function for Cyt
		initvalu_offset_Cyt = 76;												// 15 poitns
		parameter_offset_Cyt = 11;
		CaCyt = initvalu[37]*1e3;											// from ECC model, *** Converting from [mM] to [uM] ***
		JCaCyt = cam(	timeinst,
									initvalu,
									initvalu_offset_Cyt,
									parameter,
									parameter_offset_Cyt,
									finavalu,
									CaCyt);


		/*printf("made it here master mode 5\n");
		fflush(stdout);*/

	}

	//=====================================================================
	//	FINAL KERNEL
	//=====================================================================

	// final adjustments
	fin(initvalu, initvalu_offset_ecc, initvalu_offset_Dyad, initvalu_offset_SL, initvalu_offset_Cyt, parameter, finavalu, JCaDyad, JCaSL, JCaCyt);

	//=====================================================================
	//	COMPENSATION FOR NANs and INFs
	//=====================================================================

	// make sure function does not return NANs and INFs

	
	for(i=0; i<EQUATIONS; i++)
	{
		//printf("in for loop before\n");
		//fflush(stdout);
		
		int z = 0;

		z = isnan(finavalu[i]);
		z = isinf(finavalu[i]);
		
		//printf("in for loop after\n");
		//fflush(stdout);


		if (isnan(finavalu[i]) == 1){ 
		
			finavalu[i] = 0.0001;	// for NAN set rate of change to 0.0001
		}
		else if (isinf(finavalu[i]) == 1){ 
			finavalu[i] = 0.0001;	// for INF set rate of change to 0.0001
		}
	}

}
