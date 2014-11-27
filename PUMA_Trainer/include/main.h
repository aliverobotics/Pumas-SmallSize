/*
 *  main.h
 *  Jander
 *
 *  Created by Adalberto H. Llarena on 5/14/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>

#ifdef VIRTUAL
#include "SetupVir.h"
#else
#include "Setup.h"
#endif

#include "Communicate.h"
#include "Angles.h"
#include <pthread.h>

//#define WIN_32

#ifdef WIN_32

#include <io.h>
#include <windows.h> //para manejo de dlls
using namespace std;

#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <stdlib.h> //para rutina de cociente 
#include <string.h> //manejo de cadenas

#include "Cadenas.h"
#include "Data.h"
#include "Functions.h"
#include "Random.h"
#include "Orders.h"
#include "Neuron.h"
#include "NLayers.h"
#include "NNets.h"
#include "Learning.h"

Learning Brain;