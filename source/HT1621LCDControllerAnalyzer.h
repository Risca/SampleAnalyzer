#ifndef HT1621LCDCONTROLLER_ANALYZER_H
#define HT1621LCDCONTROLLER_ANALYZER_H

#include <Analyzer.h>
#include "HT1621LCDControllerAnalyzerResults.h"
#include "HT1621LCDControllerSimulationDataGenerator.h"

class HT1621LCDControllerAnalyzerSettings;
class ANALYZER_EXPORT HT1621LCDControllerAnalyzer : public Analyzer2
{
public:
	HT1621LCDControllerAnalyzer();
	virtual ~HT1621LCDControllerAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: // functions
    void Setup();
    void GetOp();
    void GetCommand();
    void GetWord(AnalyzerChannelData* clock, U32 bits, U64& result);
    bool WouldAdvancingTheClockToggleEnable(AnalyzerChannelData* clock);

protected: //vars
	std::auto_ptr< HT1621LCDControllerAnalyzerSettings > mSettings;
	std::auto_ptr< HT1621LCDControllerAnalyzerResults > mResults;
	AnalyzerChannelData* mRD;
	AnalyzerChannelData* mCS;
	AnalyzerChannelData* mData;
	AnalyzerChannelData* mWR;

	HT1621LCDControllerSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

    U64 mCurrentSample;
    enum {
        OP_READ = 0x6,
        OP_WRITE = 0x5,
        OP_COMMAND = 0x4,
        OP_UNKNOWN
    } mCurrentOp;
	std::vector<U64> mArrowLocations;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //HT1621LCDCONTROLLER_ANALYZER_H
