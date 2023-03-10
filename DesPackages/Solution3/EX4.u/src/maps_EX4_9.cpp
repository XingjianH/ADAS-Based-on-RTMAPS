////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_EX4_9.h"	// Includes the header of this component


// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSEX4_9)
//MAPS_INPUT("iName",MAPS::FilterInteger32,MAPS::FifoReader)
MAPS_INPUT("points_in", MAPS::FilterFloat64, MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSEX4_9)
//MAPS_OUTPUT("oName",MAPS::Integer32,NULL,NULL,1)
MAPS_OUTPUT("points_out", MAPS::Float64, NULL, NULL, 3000000)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSEX4_9)
//MAPS_PROPERTY("pName",128,false,false)
MAPS_PROPERTY("radieu_min", 128, false, false)
MAPS_PROPERTY("radieu_max", 128, false, false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSEX4_9)
//MAPS_ACTION("aName",MAPSFiltrage3D::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (Filtrage3D) behaviour
MAPS_COMPONENT_DEFINITION(MAPSEX4_9, "EX4_9", "1.0", 128,
	MAPS::Threaded, MAPS::Threaded,
	-1, // Nb of inputs. Leave -1 to use the number of declared input definitions
	-1, // Nb of outputs. Leave -1 to use the number of declared output definitions
	-1, // Nb of properties. Leave -1 to use the number of declared property definitions
	-1) // Nb of actions. Leave -1 to use the number of declared action definitions

//Initialization: Birth() will be called once at diagram execution startup.			  
void MAPSEX4_9::Birth()
{
	// Reports this information to the RTMaps console. You can remove this line if you know when Birth() is called in the component lifecycle.
	ReportInfo("Passing through Birth() method");
}

//ATTENTION: 
//	Make sure there is ONE and ONLY ONE blocking function inside this Core method.
//	Consider that Core() will be called inside an infinite loop while the diagram is executing.
//	Something similar to: 
//		while (componentIsRunning) {Core();}
//
//	Usually, the one and only blocking function is one of the following:
//		* StartReading(MAPSInput& input); //Data request on a single BLOCKING input. A "blocking input" is an input declared as FifoReader, LastOrNextReader, Wait4NextReader or NeverskippingReader (declaration happens in MAPS_INPUT: see the beginning of this file). A SamplingReader input is non-blocking: StartReading will not block with a SamplingReader input.
//		* StartReading(int nCount, MAPSInput* inputs[], int* inputThatAnswered, int nCountEvents = 0, MAPSEvent* events[] = NULL); //Data request on several BLOCKING inputs.
//		* SynchroStartReading(int nb, MAPSInput** inputs, MAPSIOElt** IOElts, MAPSInt64 synchroTolerance = 0, MAPSEvent* abortEvent = NULL); // Synchronized reading - waiting for samples with same or nearly same timestamps on several BLOCKING inputs.
//		* Wait(MAPSTimestamp t); or Rest(MAPSDelay d); or MAPS::Sleep(MAPSDelay d); //Pauses the current thread for some time. Can be used for instance in conjunction with StartReading on a SamplingReader input (in which case StartReading is not blocking).
//		* Any blocking grabbing function or other data reception function from another API (device driver,etc.). In such case, make sure this function cannot block forever otherwise it could freeze RTMaps when shutting down diagram.
//**************************************************************************/
//	In case of no blocking function inside the Core, your component will consume 100% of a CPU.
//  Remember that the StartReading function used with an input declared as a SamplingReader is not blocking.
//	In case of two or more blocking functions inside the Core, this is likely to induce synchronization issues and data loss. (Ex: don't call two successive StartReading on FifoReader inputs.)
/***************************************************************************/
void MAPSEX4_9::Core()
{
	// Reports this information to the RTMaps console. You can remove this line if you know when Core() is called in the component lifecycle.
	ReportInfo("Passing through Core() method");

	MAPSIOElt* ioEltIn = StartReading(Input("points_in"));
	if (ioEltIn == NULL)
		return;
	//int valueIn = ioEltIn->Float64();
	//int result = valueIn;
	MAPSIOElt* ioEltOut = StartWriting(Output("points_out"));
	int vectorsizeout = MIN(ioEltIn->VectorSize(), 32000);

	for (int i = 0; i < vectorsizeout; i += 3) {
		int x = ioEltIn->Float64(i);
		int y = ioEltIn->Float64(i+1);
		int z = ioEltIn->Float64(i+2);
		int distance = sqrt(x*x + y*y + z*z);

		if ((distance > (int)GetIntegerProperty("radieu_min")) && (distance < (int)GetIntegerProperty("radieu_max"))) {
			ioEltOut->Float64(i) = ioEltIn->Float64(i);
			ioEltOut->Float64(i+1) = ioEltIn->Float64(i+1);
			ioEltOut->Float64(i+2) = ioEltIn->Float64(i+2);
		}


	}
	ioEltOut->VectorSize() = vectorsizeout;
	ioEltOut->Timestamp() = ioEltIn->Timestamp();

	//Writing in the ioEltOut object is complete.
	//The only thing left is to release it and make it available to downstream components :
	StopWriting(ioEltOut);


	// Sleeps during 500 milliseconds (500000 microseconds).
	//This line will most probably have to be removed when you start programming your component.
	// Replace it with another blocking function. (StartReading?)
	//Rest(500000);
}

//De-initialization: Death() will be called once at diagram execution shutdown.
void MAPSEX4_9::Death()
{
	// Reports this information to the RTMaps console. You can remove this line if you know when Death() is called in the component lifecycle.
	ReportInfo("Passing through Death() method");
}
