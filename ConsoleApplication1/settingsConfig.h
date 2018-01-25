#pragma once 

//controls funtions getting console input and reading / writing to config file
#ifndef _SETTINGSCONFIG_H_
#define _SETTINGSCONFIG_H_

#include "windows.h"


//#include <msclr\marshal_cppstd.h> //for marshal_as function to convert from System::String to std::string

using namespace System;
using namespace System::IO;
//using namespace msclr::interop;
using namespace System::Collections::Generic;

ref class settingsConfig //ref makes class managed, managed members only
{
public:
	settingsConfig();
	~settingsConfig();
	void readSettingsFile();
	bool getSettingFromFile(String^ identifyingString, Int32 *settingOut);
	bool getSettingFromFile(String^ identifyingString, String^*settingOut);
	void readSettingsFromFile(int *Serialport, int *noOfLeds, int *brightness);
	bool writeSettingToFileBuf(String^ identifyingString, int settingIn);
	bool writeSettingToFileBuf(String^ identifyingString, String^settingIn);
	bool saveFile();
	void writeSerialPortToFile(int * Serialport);
	void writeLedNoToFile(int * noOfLeds);
	void writeBrightnessToFile(int * brightness);


	value struct settingsFlag
	{
		Boolean settingsLoaded;
		Boolean foundPort;
		Boolean foundLeds;
		Boolean foundBrtnss;
	};
	settingsFlag flags;


private:
	int parseValue(String^ s);
	String^ managedFilePathString;
	//System::Collections::Generic::List<String^>^ configLines = gcnew ;
	List<String^>(^ configLines);
};
#endif
