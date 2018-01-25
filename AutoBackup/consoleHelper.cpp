#include "stdafx.h"
#include "consoleHelper.h"


consoleHelper::consoleHelper()
{
}

void consoleHelper::FileCopyProgressTracker(Int32 NoOfFilesToCopy)
{
	throw gcnew System::NotImplementedException();
}



Boolean consoleHelper::YesNoQuestion(String^ QuestionText)
{
	Console::Write(QuestionText);
	Console::WriteLine("	[Y/N]");
	try { YNAnswerKey = Char::ToLower(Convert::ToChar(Console::ReadLine())); }
	catch (FormatException^)
	{
		YNAnswerKey = Convert::ToChar("z");
	}
	while (!(YNAnswerKey->Equals('y')|| YNAnswerKey->Equals('n')))
	{
		Console::Write(L"Error invalid input, ");
		Console::Write(QuestionText);
		Console::WriteLine("	[Y/N]");
		try { YNAnswerKey = Char::ToLower(Convert::ToChar(Console::ReadLine())); }
		catch (FormatException^)
		{
			YNAnswerKey = Convert::ToChar("z");
		}
	}
	if (YNAnswerKey->Equals('y'))
	{
		return true;
	}
	return false;
}