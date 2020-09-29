#pragma once
#ifndef PCONSTANTS_H
#define PCONSTANTS_H

/* Used in the Command(the command class, location is xCommand.h) 
   to let PMUT caller know that it can use an endless amount of Arguments &
   if no arguments are passed it will function just fine*/
#define NO_END_ARGS -2 

/* Used in the Command(the command class, location is xCommand.h) 
   to let PMUT caller know that it uses no Arguments & 
   if arguments are passed nothing will happen to those arguments*/
#define NO_ARGS 0

// Used in console to begin an argument statment
#define	ARG_STATMENT   "-a["

// Used in console to end a argument statement
#define ARG_END			']'

// Used in console to seperate arguments
#define ARG_SEPERATOR   ',' 

// Type qualifer
#define TYPE_QUALIFIER "-t" 

// ASCII Escape code for "clearing" screen
#define CLEAR_SCREEN_STR "\033[2J\033[1;1H"

// Used to make sure user can't type out a Command Type in a non-standard way
#define SUB_FUNC_TYPE '\006'

#include <unordered_map>
#include <chrono>

#endif