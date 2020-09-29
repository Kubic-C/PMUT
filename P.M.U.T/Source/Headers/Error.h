#pragma once
#ifndef ERROR_H
#define ERROR_H

#include "xError.h"

namespace
{
	Error InvalidCmd        ("The command requested is not valid or cannot be ran", "");
	Error InvalidArgs       ("The arguments passed where invalid", "");
	Error CouldNotFindStr   ("Could not find", "");
	Error EndingBeforeStart ("The ending statment was before the starting stament", "");
	Error IllegalInput      ("Illegal input", "");
	Error AmountOfArgumentsNotEqual  ("Too many, or not enough arguments where given", "");
	Error GenericError               ("Generic Error", "");
	Error IncorrectArgumentStatment("Incorrect argument statement", ""); 
	Error TimerNotStarted          ("Timer has not been started", "");
}

#endif // ERROR_H