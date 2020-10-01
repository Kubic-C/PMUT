#pragma once
#ifndef COMMAND_H
#define COMMAND_H

#include "xCommand.h"

#define MakeSub(Command_, Type_) SUB_FUNC_TYPE + std::string(Command_) + "-t=" + std::string(Type_) 

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
	// Vector for TCP Server
	std::map<std::string, HLnet::TCPServer>  TCPServerMaster;

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
	// Make a tcp server, Name, Ip, Port, 
	CommandDecl(CMakeTCPServer)
	{
		switch (Vector.size())
		{
		case 3:
			TCPServerMaster[Vector[2]] = (HLnet::TCPServer(Vector[0], std::stoi(Vector[1])));
			break;

		case 4:
			TCPServerMaster[Vector[2]] = (HLnet::TCPServer(Vector[0], std::stoi(Vector[1]), Vector[3]));
			break;

		default:
			throw IllegalAmountOfArgs[Vector.size()];
		}
		Print("The server < " + Vector[2] + " > has successfully been created\n");
	};

	CommandDecl(CRunTCPServer)
	{
		if (Vector[0] == "single")
		{
			if (TCPServerMaster.find(Vector[1]) != TCPServerMaster.end())
				TCPServerMaster[Vector[1]].STRun();
			else
				goto Error;

			return;
		}
		else
			throw UnkownRunningType(Vector[0]);

    Error:
		throw UknownServer(Vector[1]);
	}

	CommandDecl(CPrintAllTCPServers)
	{
		for (auto const& Pair : TCPServerMaster)
		{
			Print(Pair.first, " ");
			Pair.second.GetTCPSocket().PrintInfo();
		}
	}

	CommandDecl(CPrintTCPServer)
	{
		if (TCPServerMaster.find(Vector[0]) != TCPServerMaster.end())
			TCPServerMaster[Vector[0]].GetTCPSocket().PrintInfo();
		else
			throw UknownServer(Vector[0]);
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

	// Adds a command to Master command list
	void AddCommand(
		const CmdType& Func_,
		const int& LowArg_,
		const int& HighArg_,
		const std::string& String,
		const bool& NoEditToInput = false)
	{
		Master[String] = Command(Func_, LowArg_, HighArg_, NoEditToInput);
	}

	// Adds all commands to the unordered_map
	void CommandStart()
	{
		// Basic commands
		AddCommand(CExit, NO_ARGS, "exit");
		AddCommand(CExit, NO_ARGS, "q");
		AddCommand(CClearScreen, NO_ARGS, "clear");
		AddCommand(CPrint, NO_END_ARGS, "print", true);
		AddCommand(CGetVersion, NO_ARGS, "get_version");
		AddCommand(CPrintShape, 3, "print_shape");
		AddCommand(CTimerStart, NO_ARGS, "start_timer");
		AddCommand(CTimerEnd, NO_ARGS, "end_timer");
		AddCommand(CPrintYoutube, NO_ARGS, "my_meth");
		AddCommand(CMakeTCPServer, 3, 4, MakeSub("make_server", "tcp"));
		AddCommand(CRunTCPServer, 2, MakeSub("run_server", "tcp"));
		AddCommand(CPrintAllTCPServers, NO_ARGS, MakeSub("print", "tcp_server"));
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
	static std::vector<std::string>& RemoveSpaces(Input& Input_, bool RemoveSpaces)
	{
		if (RemoveSpaces)
			ConsoleParser::GetRidOfSpaces(Input_);

		return Input_.Arguments;
	}

	static void _NoSafetyCallCommand(Input& Input) // PMUT Caller
	{
		const int NeededAmountOfArgs = Master[Input.Command].GetLowArgValue();
		switch (NeededAmountOfArgs)
		{
		case NO_END_ARGS:
			Master[Input.Command](RemoveSpaces(Input, !Master[Input.Command].GetNoEditToArgs()));
			return;

		case NO_ARGS:
			Master[Input.Command]();
			return;

		default:
			break;
		}

		// Check if the user put in the reqiured amount of arguments
		size_t UserAmountOfArgs = Input.Arguments.size();
		if (Master[Input.Command].GetLowArgValue() > UserAmountOfArgs || Master[Input.Command].GetHighArgValue() < UserAmountOfArgs)
			throw AmountOfArgumentsNotEqual[UserAmountOfArgs];

		Master[Input.Command](RemoveSpaces(Input, !Master[Input.Command].GetNoEditToArgs()));
	}

	static void CallCommand(Input& Input)
	{
		if (Master.find(Input.Command) != Master.end())
		{
			// Safety added becuase of check ^^^
			Caller::_NoSafetyCallCommand(Input);
		}
		else
			throw InvalidCmd(Input.Command);
	}
}

#endif // COMMAND_H