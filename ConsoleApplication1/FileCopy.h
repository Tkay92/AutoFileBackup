#pragma once

using namespace System;
ref class FileCopy
{
private:
	;
public:
	FileCopy();
	Boolean GetDestinationPath(String^ DestinationPath);
	array<IO::FileSystemInfo^>^GetDirFileList(String ^ InputDir);
	Collections::Generic::List<String^>^ copyDirectory(String ^outputFile, array <IO::FileSystemInfo^>^ DirItemList);
	Boolean replaceCheck(String^ CopyTarget, array <IO::FileSystemInfo^>^ DirItemList);
	String^ CopyDir;	//general copy location
	String^ CopyLocation;	//specific folder location to copy current files to
};

