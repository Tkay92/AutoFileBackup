#pragma once
using namespace System;

ref class consoleHelper
{
public:
	static Char^ YNAnswerKey;
	consoleHelper();
	static void FileCopyProgressTracker(Int32 NoOfFilesToCopy);
	static Boolean YesNoQuestion(String^ QuestionText);

};

