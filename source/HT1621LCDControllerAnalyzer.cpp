#include "HT1621LCDControllerAnalyzer.h"
#include "HT1621LCDControllerAnalyzerSettings.h"
#include <AnalyzerChannelData.h>
#include <AnalyzerHelpers.h>

HT1621LCDControllerAnalyzer::HT1621LCDControllerAnalyzer()
:	Analyzer2(),  
	mSettings( new HT1621LCDControllerAnalyzerSettings() ),
    mRD( NULL ),
    mCS( NULL ),
    mData( NULL ),
    mWR( NULL ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

HT1621LCDControllerAnalyzer::~HT1621LCDControllerAnalyzer()
{
	KillThread();
}

void HT1621LCDControllerAnalyzer::SetupResults()
{
	mResults.reset( new HT1621LCDControllerAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	if( mSettings->mReadChannel != UNDEFINED_CHANNEL )
		mResults->AddChannelBubblesWillAppearOn( mSettings->mReadChannel );
	if( mSettings->mWriteChannel != UNDEFINED_CHANNEL )
		mResults->AddChannelBubblesWillAppearOn( mSettings->mWriteChannel );
}

void HT1621LCDControllerAnalyzer::WorkerThread()
{
	Setup();
//	mSampleRateHz = GetSampleRate();

	if( mCS->GetBitState() == BIT_HIGH )
		mCS->AdvanceToNextEdge();
	mCurrentSample = mCS->GetSampleNumber();

	for( ; ; )
	{
		GetOp();
		switch( mCurrentOp ) {
		case OP_COMMAND:
			GetCommand();
			break;
		case OP_READ:
		case OP_WRITE:
		default:
			// TODO
			break;
		}
		mCS->AdvanceToNextEdge();
		mCS->AdvanceToNextEdge();
		mCurrentSample = mCS->GetSampleNumber();
		CheckIfThreadShouldExit();
	}
}

void HT1621LCDControllerAnalyzer::Setup()
{
	if( mSettings->mReadChannel != UNDEFINED_CHANNEL )
		mRD = GetAnalyzerChannelData( mSettings->mReadChannel );
	else
		mRD = NULL;

	mCS = GetAnalyzerChannelData( mSettings->mCSChannel );
	mData = GetAnalyzerChannelData( mSettings->mDataChannel );

	if( mSettings->mWriteChannel != UNDEFINED_CHANNEL )
		mWR = GetAnalyzerChannelData( mSettings->mWriteChannel );
	else
		mWR = NULL;
}

bool HT1621LCDControllerAnalyzer::WouldAdvancingTheClockToggleEnable(AnalyzerChannelData* clock)
{
    return mCS->WouldAdvancingToAbsPositionCauseTransition( clock->GetSampleOfNextEdge() );
}

void HT1621LCDControllerAnalyzer::GetOp()
{
    U64 op;
    GetWord(mWR, 3, op);
	if( op == OP_READ )
        mCurrentOp = OP_READ;
    else if( op == OP_WRITE )
        mCurrentOp = OP_WRITE;
    else if( op == OP_COMMAND )
        mCurrentOp = OP_COMMAND;
    else
        mCurrentOp = OP_UNKNOWN;
}

void HT1621LCDControllerAnalyzer::GetCommand()
{
    U64 cmd;
    GetWord(mWR, 9, cmd);
}

void HT1621LCDControllerAnalyzer::GetWord(AnalyzerChannelData* clock, U32 bits, U64& result)
{
	DataBuilder word;
	word.Reset( &result, AnalyzerEnums::MsbFirst, bits);
	
	U64 first_sample;
	
	mArrowLocations.clear();
	ReportProgress( mCS->GetSampleNumber() );
	
	clock->AdvanceToAbsPosition( mCurrentSample );
	for( U32 i=0; i<bits; ++i )
	{
        if( WouldAdvancingTheClockToggleEnable( clock ) ) {
            mCurrentOp = OP_UNKNOWN;
            return;
        }
		clock->AdvanceToNextEdge();

		if( i == 0 )
			first_sample = clock->GetSampleNumber();

        if( WouldAdvancingTheClockToggleEnable( clock ) ) {
            mCurrentOp = OP_UNKNOWN;
            return;
        }
		clock->AdvanceToNextEdge();

		mCurrentSample = clock->GetSampleNumber();
		mData->AdvanceToAbsPosition( mCurrentSample );
		word.AddBit( mData->GetBitState() );
		mArrowLocations.push_back( mCurrentSample );
    }

	U32 count = mArrowLocations.size();
	for( U32 i=0; i<count; ++i )
		mResults->AddMarker( mArrowLocations[i], AnalyzerResults::UpArrow, mSettings->mWriteChannel );

	Frame result_frame;
	result_frame.mStartingSampleInclusive = first_sample;
	result_frame.mEndingSampleInclusive = clock->GetSampleNumber();
	result_frame.mData1 = result;
	result_frame.mData2 = 0;
	result_frame.mFlags = 0;
	mResults->AddFrame( result_frame );

	mResults->CommitResults();
}

bool HT1621LCDControllerAnalyzer::NeedsRerun()
{
	return false;
}

U32 HT1621LCDControllerAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 HT1621LCDControllerAnalyzer::GetMinimumSampleRateHz()
{
    // we have no idea, depends on RD/WR clock
	return 10000;
}

const char* HT1621LCDControllerAnalyzer::GetAnalyzerName() const
{
	return "HT1621 LCD Controller";
}

const char* GetAnalyzerName()
{
	return "HT1621 LCD Controller";
}

Analyzer* CreateAnalyzer()
{
	return new HT1621LCDControllerAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
