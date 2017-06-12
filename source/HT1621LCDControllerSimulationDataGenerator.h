#ifndef HT1621LCDCONTROLLER_SIMULATION_DATA_GENERATOR
#define HT1621LCDCONTROLLER_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class HT1621LCDControllerAnalyzerSettings;

class HT1621LCDControllerSimulationDataGenerator
{
public:
	HT1621LCDControllerSimulationDataGenerator();
	~HT1621LCDControllerSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, HT1621LCDControllerAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	HT1621LCDControllerAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //HT1621LCDCONTROLLER_SIMULATION_DATA_GENERATOR