#include "stdafx.h"
#include "FileListParse.h"


FileListParse::FileListParse()
{

}

Boolean FileListParse::GetFileListPath(String^ FileListPath)
{
	try {

		PathsToBackup = gcnew List<String^>(safe_cast<IEnumerable<String^>^>(File::ReadLines(FileListPath)));
	}
	catch (DirectoryNotFoundException^)
	{
		return false;
	}

	Int32 countLimit = PathsToBackup->Count;
	for (Int32 i = 0; i< countLimit; i++)
	{
		String^ FilePath = PathsToBackup[i];
		if (FilePath->Length <= 3)
		{
			PathsToBackup->RemoveAt(i);
			i--;
			countLimit--;
		}
	}
	return true;
}

String ^ FileListParse::GetTopFile()
{
	throw gcnew System::NotImplementedException();
	// TODO: insert return statement here
}

void FileListParse::addStringToBuffer()
{
	throw gcnew System::NotImplementedException();
}

Boolean FileListParse::SaveFileListPath(String^ FileListPath)
{
	File::WriteAllLines(FileListPath, PathsToBackup);
	return true;
}

