#include "HT1621LCDControllerAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


HT1621LCDControllerAnalyzerSettings::HT1621LCDControllerAnalyzerSettings()
:	mReadChannel( UNDEFINED_CHANNEL ),
	mCSChannel( UNDEFINED_CHANNEL ),
	mDataChannel( UNDEFINED_CHANNEL ),
	mWriteChannel( UNDEFINED_CHANNEL )
{
	mReadChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mReadChannelInterface->SetTitleAndTooltip( "Read clock", "HT1621 LCD - read clock" );
	mReadChannelInterface->SetChannel( mReadChannel );

	mCSChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mCSChannelInterface->SetTitleAndTooltip( "Chip Select", "HT1621 LCD - chip select" );
	mCSChannelInterface->SetChannel( mCSChannel );

	mDataChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mDataChannelInterface->SetTitleAndTooltip( "Data", "HT1621 LCD - data" );
	mDataChannelInterface->SetChannel( mDataChannel );

	mWriteChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mWriteChannelInterface->SetTitleAndTooltip( "Write clock (TODO)", "HT1621 LCD - write clock (TODO)" );
	mWriteChannelInterface->SetChannel( mWriteChannel );

	AddInterface( mReadChannelInterface.get() );
	AddInterface( mCSChannelInterface.get() );
	AddInterface( mDataChannelInterface.get() );
	AddInterface( mWriteChannelInterface.get() );

//	AddExportOption( 0, "Export as text/csv file" );
//	AddExportExtension( 0, "text", "txt" );
//	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mReadChannel, "HT1621 LCD - RD", false );
	AddChannel( mCSChannel, "HT1621 LCD - CS", false );
	AddChannel( mDataChannel, "HT1621 LCD - Data", false );
	AddChannel( mWriteChannel, "HT1621 LCD - WR (TODO)", false );
}

HT1621LCDControllerAnalyzerSettings::~HT1621LCDControllerAnalyzerSettings()
{
}

bool HT1621LCDControllerAnalyzerSettings::SetSettingsFromInterfaces()
{
   	mReadChannel = mReadChannelInterface->GetChannel();
	mCSChannel = mCSChannelInterface->GetChannel();
	mDataChannel = mDataChannelInterface->GetChannel();
	mWriteChannel = mWriteChannelInterface->GetChannel();
	ClearChannels();

	Channel ArrayOfChannels [4];
	ArrayOfChannels[0] = mReadChannel;
	ArrayOfChannels[1] = mCSChannel;
	ArrayOfChannels[2] = mDataChannel;
	ArrayOfChannels[3] = mWriteChannel;

	bool IsInvalidConfig = AnalyzerHelpers::DoChannelsOverlap(ArrayOfChannels,4);

	if(IsInvalidConfig)
	{
		SetErrorText( "Each channel must be unique!" );
		return false;
	}
	else
	{
		AddChannel( mReadChannel, "HT1621 LCD - RD", true );
		AddChannel( mCSChannel, "HT1621 LCD - CS", true );
		AddChannel( mDataChannel, "HT1621 LCD - Data", true );
		AddChannel( mWriteChannel, "HT1621 LCD - WR (TODO)", true );
		return true;
	}

	return true;
}

void HT1621LCDControllerAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mReadChannelInterface->SetChannel( mReadChannel );
	mCSChannelInterface->SetChannel( mCSChannel );
	mDataChannelInterface->SetChannel( mDataChannel );
	mWriteChannelInterface->SetChannel( mWriteChannel );
}

void HT1621LCDControllerAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mReadChannel;
	text_archive >> mCSChannel;
	text_archive >> mDataChannel;
	text_archive >> mWriteChannel;

	ClearChannels();
	AddChannel( mReadChannel, "HT1621 LCD - RD", true );
	AddChannel( mCSChannel, "HT1621 LCD - CS", true );
	AddChannel( mDataChannel, "HT1621 LCD - Data", true );
	AddChannel( mWriteChannel, "HT1621 LCD - WR (TODO)", true );

	UpdateInterfacesFromSettings();
}

const char* HT1621LCDControllerAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mReadChannel;
	text_archive << mCSChannel;
	text_archive << mDataChannel;
	text_archive << mWriteChannel;

	return SetReturnString( text_archive.GetString() );
}

