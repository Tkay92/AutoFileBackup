#pragma once 
#include "stdafx.h"
#include "settingsConfig.h"

//#include "msclr\marshal_cppstd.h"

settingsConfig::settingsConfig()
{
	WCHAR filePathBuffer[MAX_PATH];
	HMODULE hmodule = GetModuleHandle(NULL);
	int programPath = GetModuleFileName(hmodule, filePathBuffer, MAX_PATH);
	managedFilePathString = gcnew String(filePathBuffer);
	managedFilePathString = Path::GetDirectoryName(managedFilePathString) + "\\config.txt";
	if (programPath)
	{
		Console::Write("got filepath = ");
		Console::WriteLine(managedFilePathString);
	}
	else
	{
		Console::WriteLine("didn't get filepath");
	}
}


settingsConfig::~settingsConfig()
{

}

void settingsConfig::readSettingsFile() //copies settings to buffer array from file
{
	configLines = gcnew List<String^>(safe_cast<IEnumerable<String^>^>(File::ReadLines(managedFilePathString)));
}

bool settingsConfig::getSettingFromFile(String^ identifyingString, Int32 *settingOut) //extracts file
{
	
	for each (String^ s in configLines)
	{
		if ((s->ToLower())->Contains(identifyingString->ToLower()))
		{
			
			try
			{
				String^valueString = (s->Substring(s->IndexOf('=') + 1))->Trim();
				*settingOut = Convert::ToInt32(valueString);
				return true;
			}
			catch (System::FormatException^)
			{
				return false;
			}
		}
	}
	return false;
}



bool settingsConfig::getSettingFromFile(String^ identifyingString, String^ *settingOut)
{
	for each (String^ s in configLines)
	{
		if ((s->ToLower())->Contains(identifyingString->ToLower()))
		{
			*settingOut = (s->Substring(s->IndexOf('=') + 1))->Trim();
			return true;
		}
	}
	return false;
}


void settingsConfig::readSettingsFromFile(int *Serialport, int *noOfLeds, int *brightness) //read/parse values from settings file
{
	System::Collections::Generic::IEnumerable<String^>(^fileLines) = File::ReadLines(managedFilePathString);

	int counter = 0;
	for each (String^ s in fileLines)
	{
		int value = 0;
		try
		{
			String^valueString = s->Substring(s->IndexOf('=') + 1);
			valueString = valueString->Trim();
			try
			{
				value = Convert::ToInt32(valueString);
			}
			catch (FormatException^ e) // catch non data lines in file
			{
				value = 0;
			}
		}
		catch (ArgumentOutOfRangeException^ e)
		{
			value = 0;
		}
		if (s->Contains("SerialPort"))
		{
			*Serialport = value;
			flags.foundPort = true;
		}
		else if (s->Contains("NoOfLEDS"))
		{
			*noOfLeds = value;
			flags.foundLeds = true;
		}
		else if (s->Contains("Brightness"))
		{
			*brightness = value;
			flags.foundBrtnss = true;
		}
		counter++;
	}
	flags.settingsLoaded = flags.foundBrtnss && flags.foundLeds && flags.foundPort;

}

bool settingsConfig::writeSettingToFileBuf(String ^ identifyingString, int settingIn)
{
	bool foundFlag = FALSE;
	String^ newSetting = identifyingString + " = " + Convert::ToString(settingIn);
	int stringCounter = 0;
	for each (String^ s in configLines)
	{
		if (s->Contains(identifyingString))
		{
			s = newSetting;
			configLines->RemoveAt(stringCounter);
			configLines->Insert(stringCounter, s);
			return true;
		}
		stringCounter++;
	}
	configLines->Add(newSetting);
	return false;
}

bool settingsConfig::writeSettingToFileBuf(String ^ identifyingString, String ^ settingIn)
{
	bool foundFlag = FALSE;
	String^ newSetting = identifyingString + " = " + settingIn;
	int stringCounter = 0;
	for each (String^ s in configLines)
	{
		if (s->Contains(identifyingString))
		{
			s = newSetting;
			configLines->RemoveAt(stringCounter);
			configLines->Insert(stringCounter, s);
			return true;
		}
		stringCounter++;
	}
	configLines->Add(newSetting);
	return false;
}

bool settingsConfig::saveFile()
{
		File::WriteAllLines(managedFilePathString, configLines);
		return true;
}

void settingsConfig::writeSerialPortToFile(int *Serialport)
{

	
	System::Collections::Generic::IEnumerable<String^>(^fileLines) = File::ReadLines(managedFilePathString);
	bool foundFlag = false;
	int counter = 0;
	for each (String^ s in fileLines)
	{
		if (s->Contains("SerialPort"))
		{
			foundFlag = true;
			s = (s->Substring(0,s->IndexOf('=') + 1))+" "+ (Convert::ToString(*Serialport));
		}
		
	}
	File::GetAccessControl(managedFilePathString);
	if (foundFlag)
	{
		File::WriteAllLines(managedFilePathString, fileLines);
	}
	else
	{
		File::AppendAllText(managedFilePathString, (String::Concat((String::Concat("SerialPort = " , Convert::ToString(*Serialport))), "\r\n")));
	}
}

void settingsConfig::writeLedNoToFile(int *noOfLeds)
{
	System::Collections::Generic::IEnumerable<String^>(^fileLines) = File::ReadLines(managedFilePathString);
	bool foundFlag = false;
	int counter = 0;
	for each (String^ s in fileLines)
	{
		if (s->Contains("NoOfLEDS"))
		{
			foundFlag = true;
			s = String::Concat(s->Substring(0, s->IndexOf('=') + 1) , String::Concat( " " + (Convert::ToString(*noOfLeds))));
		}

	}
	File::GetAccessControl(managedFilePathString);
	if (foundFlag)
	{
		File::WriteAllLines(managedFilePathString, fileLines);
	}
	else
	{
		File::AppendAllText(managedFilePathString, (String::Concat((String::Concat("NoOfLEDS = ", Convert::ToString(*noOfLeds))), "\r\n")));
	}
	
}

void settingsConfig::writeBrightnessToFile(int *brightness)
{
	System::Collections::Generic::IEnumerable<String^>(^fileLines) = File::ReadLines(managedFilePathString);
	bool foundFlag = false;
	int counter = 0;
	for each (String^ s in fileLines)
	{
		if (s->Contains("Brightness"))
		{
			foundFlag = true;
			s = String::Concat(s->Substring(0, s->IndexOf('=') + 1), String::Concat(" " + (Convert::ToString(*brightness))));
		}

	}
	File::GetAccessControl(managedFilePathString);
	if (foundFlag)
	{
		File::WriteAllLines(managedFilePathString, fileLines);
	}
	else
	{
		File::AppendAllText(managedFilePathString, (String::Concat((String::Concat("Brightness = " , Convert::ToString(*brightness))), "\r\n")));
	}
	
}


int settingsConfig::parseValue(String ^ s)
{
	return 0;
}
