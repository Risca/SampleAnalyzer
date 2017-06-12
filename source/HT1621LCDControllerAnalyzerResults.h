#ifndef HT1621LCDCONTROLLER_ANALYZER_RESULTS
#define HT1621LCDCONTROLLER_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class HT1621LCDControllerAnalyzer;
class HT1621LCDControllerAnalyzerSettings;

class HT1621LCDControllerAnalyzerResults : public AnalyzerResults
{
public:
	HT1621LCDControllerAnalyzerResults( HT1621LCDControllerAnalyzer* analyzer, HT1621LCDControllerAnalyzerSettings* settings );
	virtual ~HT1621LCDControllerAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	HT1621LCDControllerAnalyzerSettings* mSettings;
	HT1621LCDControllerAnalyzer* mAnalyzer;
};

#endif //HT1621LCDCONTROLLER_ANALYZER_RESULTS
