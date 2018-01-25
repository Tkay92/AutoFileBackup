#pragma once 

#include "stdafx.h"
#include "FileCopy.h"

using namespace System::IO;


Boolean FileCopy::GetDestinationPath(String^ DestinationPath)
{
	if (Directory::Exists(DestinationPath))
	{
		CopyDir = DestinationPath;
		return true;
	}
	return false;
}

FileCopy::FileCopy()
{
}

array<IO::FileSystemInfo^>^ FileCopy::GetDirFileList(String ^ InputDir)
{
	DirectoryInfo^ copyFileInfo = gcnew DirectoryInfo(InputDir);
	return copyFileInfo->GetFileSystemInfos("*");
}

Collections::Generic::List<String^>^ FileCopy::copyDirectory(String^ outputDirectory, array <IO::FileSystemInfo^>^ DirItemList)
{
	if (!Directory::Exists(outputDirectory))
	{
		Directory::CreateDirectory(outputDirectory);
	}
	Collections::Generic::List<String^>^ failedFileBuf = gcnew Collections::Generic::List<String^>;
	for each (FileSystemInfo^ fileObject in DirItemList)
	{
		
		String^ fullOutputPath = outputDirectory + "\\" + fileObject->Name;
		
		if ((fileObject->Attributes & FileAttributes::Directory) == FileAttributes::Directory)
		{
			if(!Directory::Exists(fullOutputPath))
			{
				Directory::CreateDirectory(fullOutputPath);
			}
			array <IO::FileSystemInfo^>^ TempItemList = FileCopy::GetDirFileList(fileObject->FullName);
			failedFileBuf->AddRange(FileCopy::copyDirectory(fullOutputPath, TempItemList));
		}
		else
		{
			
			if (File::Exists(fullOutputPath) && fileObject->LastWriteTime != File::GetLastWriteTime(fullOutputPath))//update any edited files
			{
				
				try {
					File::SetAttributes(fullOutputPath, FileAttributes::Normal);	//File::delete can't delete read only files, so set file attributes to normal before deleteing
					File::Delete(fullOutputPath);	// must delete before replacing file
					File::Copy(fileObject->FullName, fullOutputPath);
				}
				catch (UnauthorizedAccessException^)
				{
					failedFileBuf->Add(fileObject->Name);
				}
				catch (Security::SecurityException^)
				{
					failedFileBuf->Add(fileObject->Name);
				}
			}
			else if(!File::Exists(fullOutputPath))
			{
				try {
					File::Copy(fileObject->FullName, fullOutputPath);
				}
				catch (UnauthorizedAccessException^)
				{
					failedFileBuf->Add(fileObject->Name);
				}
				catch (Security::SecurityException^)
				{
					failedFileBuf->Add(fileObject->Name);
				}
			}

		}
	}
	return failedFileBuf;
}

Boolean FileCopy::replaceCheck(String^ CopyTarget, array <IO::FileSystemInfo^>^ DirItemList)
{

	DateTime LastNewFileEdit;
	DateTime LastCopiedFileEdit;
	DirectoryInfo^ existingDirCopyInfo = gcnew DirectoryInfo(CopyTarget);
	array<FileSystemInfo^>^ existingFileCopyInfo = existingDirCopyInfo->GetFileSystemInfos("*");
	for each (FileSystemInfo^ fileInfo in DirItemList)
	{
		if (fileInfo->LastWriteTime > LastNewFileEdit)
		{
			LastNewFileEdit = fileInfo->LastWriteTime;
		}
	}
	for each (FileSystemInfo^ fileInfo in existingFileCopyInfo)
	{
		if (fileInfo->LastWriteTime > LastCopiedFileEdit)
		{
			LastCopiedFileEdit = fileInfo->LastWriteTime;
		}
	}
	if (LastCopiedFileEdit <= LastNewFileEdit)
	{
		return true;
	}
	return false;
}