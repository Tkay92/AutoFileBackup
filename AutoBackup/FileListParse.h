#pragma once

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;

ref class FileListParse
{
private:
	
public:
	List<String^>(^PathsToBackup);
	FileListParse();
	Boolean GetFileListPath(String^ FileListPath);
	String^ GetTopFile();
	void addStringToBuffer();
	Boolean SaveFileListPath(String ^ FileListPath);
};


