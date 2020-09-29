#pragma once
#ifndef COMMAND_H
#define COMMAND_H

#include "xCommand.h"

#define MakeSub(Command_, Type_) SUB_FUNC_TYPE + std::string(Command_) + "T" + std::string(Type_) 

namespace
{
	enum VectorType
	{
		Type = 0
	};

	// Master set for all commands
	std::unordered_map<std::string, Command> Master;
	// Timer varible
	std::chrono::steady_clock::time_point* CTimerStart_ = nullptr;

	// Prints out a vector
	CommandDecl(CPrint)
	{
		for (const std::string& Str_ : Vector)
			Print(Str_, '\n');
	}

	void PreExit()
	{
		if(CTimerStart_)
			delete CTimerStart_;
		HLnet::NetworkCleanup();
	}
	// ^^^ related vvv
	CommandDecl(CExit)
	{
		// Exit
		atexit(PreExit);
		exit(0);
	}

	// Prints a shape
	CommandDecl(CPrintShape)
	{
		int Width = std::stoi(Vector[0]);
		int Height = std::stoi(Vector[1]);
		char Char = Vector[2][0];

		std::string Buffer = "";
		for (int Wi = 0; Wi < Width; Wi++)
		{
			for (int Hi = 0; Hi < Height; Hi++)
			{
				Buffer += Char;
			}

			Buffer += '\n';
		}

		Print(Buffer);
	}
	// Moves screen down
	CommandDecl(CClearScreen)
	{
		Print(CLEAR_SCREEN_STR);
		Print(PMUT_, '\n');
	}
	// Prints PMUT_
	CommandDecl(CGetVersion)
	{
		Print(PMUT_, '\n');
	}
	// Starts timer
	CommandDecl(CTimerStart)
	{
		CTimerStart_ = new std::chrono::steady_clock::time_point;
		*CTimerStart_ = std::chrono::high_resolution_clock::now(); // Start timer
		Print("Timer started/restarted...\n");
	}
	// Ends timer
	CommandDecl(CTimerEnd)
	{
		if (CTimerStart_)
		{
			std::chrono::steady_clock::time_point CTimerEnd_ = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> Duration = CTimerEnd_ - *CTimerStart_;
			Print("Time passed < " + std::to_string(Duration.count()) + "s >\n");
			delete CTimerStart_;
			CTimerStart_ = nullptr;
		}
		else
			throw TimerNotStarted("CTimerStart_ was nullptr");
			
	}
	// Print meth
	CommandDecl(CPrintYoutube)
	{
		Print("www.youtube.com\n");
	}
	// For info 
	CommandDecl(CInfoCommand)
	{
		Print("To find a list of commands and what they do;\n",
		      "refer to Help.txt provided with this copy of PMUT\n");
		Print("\nIf any bug is found please report to");
	}

	// Adds a command to Master command list
	void AddCommand(
		const CmdType& Func_, 
		const int& AmountOfArgs, 
		const std::string& String, 
		const bool& NoEditToInput = false)
	{
		Master[String] = Command(Func_, AmountOfArgs, NoEditToInput);
	}

	// Adds all commands to the unordered_map
	void CommandStart()
	{
		// Basic commands
		AddCommand(CExit, NO_ARGS, "exit");
		AddCommand(CClearScreen, NO_ARGS, "clear");
		AddCommand(CPrint, NO_END_ARGS, "print", true);
		AddCommand(CGetVersion, NO_ARGS, "get_version");
		AddCommand(CPrintShape, 3, "print_shape");
		AddCommand(CTimerStart, NO_ARGS, "start_timer");
		AddCommand(CTimerEnd, NO_ARGS, "end_timer");
		AddCommand(CPrintYoutube, NO_ARGS, "my_meth");
	}

	// Reinitialize
	CommandDecl(CInitCmd)
	{
		Print("Restarting master list...\n");
		Master.clear();
		AddCommand(CInitCmd, NO_ARGS, MakeSub("restart", "commands")); // Only exception when it comes to add commands 
		CommandStart();
		Print("Successfully restarted master list...\n"); 
	}
}

// PMUT Caller, responsible for calling and passing arguments to functions based on input from the terminal
namespace Caller
{
	static void _NoSafetyCallCommand(Input& Input) // PMUT Caller
	{
		const int NeededAmountOfArgs = Master[Input.Command].GetNeededArgs();
		switch (NeededAmountOfArgs)
		{
		case NO_END_ARGS:
			Input.PrepareFuncArgs(!Master[Input.Command].GetNoEditToArgs()); // Preparing function arguments
			Master[Input.Command](Input.FuncArgs);
			return;

		case NO_ARGS:
			Input.PrepareFuncArgs(false ,false); // Preparing function arguments
			Master[Input.Command](Input.FuncArgs);  
			return;

		default:
			break;
		}
		// prepare function args
		Input.PrepareFuncArgs(!Master[Input.Command].GetNoEditToArgs());

		// Check if the user put in the reqiured amount of arguments
		size_t UserAmountOfArgs = Input.Args.size();
		if (NeededAmountOfArgs != UserAmountOfArgs)
			throw AmountOfArgumentsNotEqual[UserAmountOfArgs];

		Master[Input.Command](Input.FuncArgs);
	}

	static void CallCommand(Input& Input)
	{
		Input.SetCommand();
		if (Master.find(Input.Command) != Master.end())
		{
			// Safety added becuase of check ^^^
			Caller::_NoSafetyCallCommand(Input);
		}
		else
			throw InvalidCmd("-c : " + Input.CommandCopy + " -t : " + Input.Type);
	}
}

#endif // COMMAND_H