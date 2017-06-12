#ifndef HT1621LCDCONTROLLER_ANALYZER_SETTINGS
#define HT1621LCDCONTROLLER_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class HT1621LCDControllerAnalyzerSettings : public AnalyzerSettings
{
public:
	HT1621LCDControllerAnalyzerSettings();
	virtual ~HT1621LCDControllerAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mReadChannel;
	Channel mCSChannel;
	Channel mDataChannel;
    // TODO
	Channel mWriteChannel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mReadChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mCSChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mDataChannelInterface;
    // TODO
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mWriteChannelInterface;
};

#endif //HT1621LCDCONTROLLER_ANALYZER_SETTINGS
