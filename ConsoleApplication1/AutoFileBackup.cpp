//AutoFileBackup
//A program for copying listed directories to an alternative location 
#include "stdafx.h"
#include "settingsConfig.h"
#include "FileCopy.h"
#include "FileListParse.h"
#include "consoleHelper.h"

using namespace System;



int main(array<System::String ^> ^args)
{

	FileCopy^ fileCopier0 = gcnew FileCopy;	//new instance of file copier
	FileListParse^ fileListParser = gcnew FileListParse;

	Console::WriteLine(L"Auto File Backup V1");
	Console::WriteLine(L"Tom Kay 2017");
	Console::WriteLine(L"Loading ConfigFile...");
	settingsConfig^ config0 = gcnew settingsConfig; //newinstance of configutration file reader, config file read on declaration
	config0->readSettingsFile();
	String^ FileListLocation;
	if (config0->getSettingFromFile("FileList", &FileListLocation))
	{
		Console::Write(L"got file list location: ");
		Console::WriteLine(FileListLocation);
	}
	else
	{
		while (1)
		{
			Console::WriteLine("Could not find FileList Location in configfile,");
			Console::WriteLine("please enter FileList.txt location:");
			FileListLocation = Console::ReadLine() + L"\\FileList.txt";
			if (File::Exists(FileListLocation))
			{
				Console::WriteLine("got file list location:");
				Console::WriteLine(FileListLocation);
				break;
			}
		}
	}

	while (!fileListParser->GetFileListPath(FileListLocation))
	{
		Console::WriteLine(L"Could not find FileList.txt, please enter file directory: ");
		FileListLocation = Console::ReadLine()+L"\\FileList.txt";
		Console::WriteLine(FileListLocation);
	}
	config0->writeSettingToFileBuf("FileList", FileListLocation);


	//load or set copy location

	String^ CopyLocation;
	if (config0->getSettingFromFile("CopyDirectory", &CopyLocation))
	{
		Console::Write(L"loaded copy directory: ");
		Console::WriteLine(CopyLocation);
	}
	else
	{
		Console::WriteLine("Could not find copy directory in configfile,");
		Console::WriteLine("please enter the destination folder of copied files:");
		CopyLocation = Console::ReadLine();
	}
	while (!fileCopier0->GetDestinationPath(CopyLocation))
	{
		
		if (consoleHelper::YesNoQuestion(L"could not find output directory, would you like to create it?"))
		{
			try { Directory::CreateDirectory(CopyLocation); }
			catch (DirectoryNotFoundException^)
			{
				Console::WriteLine("could not make new directory at specified path, please enter different directory: ");
				CopyLocation = Console::ReadLine();
			}
		}
		else
		{
			Console::WriteLine("please enter new directory: ");
			CopyLocation = Console::ReadLine();
		}
	}
	config0->writeSettingToFileBuf("CopyDirectory", CopyLocation);

	while (consoleHelper::YesNoQuestion("would you like to add more file locations to backup?"))
	{
		Console::WriteLine(L"Please add new file directory: ");
		String^ DirToAdd = Console::ReadLine();
		if (Directory::Exists(DirToAdd))
		{
			fileListParser->PathsToBackup->Add(DirToAdd);
			Console::WriteLine(L"Added Directory to backup list");
		}
		else
		{
			Console::WriteLine(L"Error: Directory not Found");
		}
	}
	Console::WriteLine(L"Configuration complete, scanning files for changes and most up to date versions");

	for each (String^ FilePath in fileListParser->PathsToBackup)
	{
		DirectoryInfo^ FilePathInfo = gcnew DirectoryInfo(FilePath);
		Collections::Generic::List<String^>^ failedFileCopyBuf = gcnew Collections::Generic::List<String^>;
		array <IO::FileSystemInfo^>^ filesToCopy = fileCopier0->GetDirFileList(FilePath);
		if (!fileCopier0->replaceCheck(FilePath, filesToCopy))
		{
			
			if(consoleHelper::YesNoQuestion("Directory to backup contains files older than in the backed up directory, would you like to proceed?"))
			{
				failedFileCopyBuf->AddRange(fileCopier0->copyDirectory(CopyLocation +"\\"+ FilePathInfo->Name, filesToCopy));
			}
		}
		else
		{
			failedFileCopyBuf->AddRange(fileCopier0->copyDirectory(CopyLocation +"\\" + FilePathInfo->Name, filesToCopy));
		}
		if (failedFileCopyBuf->Count != 0)
		{
			Console::WriteLine("could not copy all files in directory: ");
			Console::WriteLine(FilePath);
			Console::WriteLine("the following files were not copied:");
			for each (String^ line in failedFileCopyBuf)
			{
				Console::WriteLine(line);
			}
		}
		else
		{
			Console::WriteLine("All files in directory: ");
			Console::WriteLine(FilePath);
			Console::WriteLine("were copied successfully");
		}
	}

	config0->saveFile();
	fileListParser->SaveFileListPath(FileListLocation);
	
	Console::WriteLine("Backup complete, press any key to exit...");
	Console::Read();

    return 0;
}



