#pragma once
#ifndef PMUT_ERROR_H
#define PMUT_ERROR_H

#include "base.h"

namespace pmut
{
	namespace errors
	{
		struct error
		{
			std::string error_;
			std::string how = "";

			error(std::string error_)
				: error_(error_) {}

			template<typename T>
			error& operator()(T how)
			{
				set_how(how);
				return *this;
			}

		private:
			template<typename T>
			void set_how(T how)
			{
				this->how = std::to_string(how);
			}

			void set_how(std::string how)
			{
				this->how = how;
			}

			void set_how(const char* how)
			{
				this->how = std::string(how);
			}
		};

		ONE_DEF error uknown_option("uknown option");
		ONE_DEF error uknown_command("uknown command");
		ONE_DEF error no_seperator_found("no seperator found in option");
		ONE_DEF error invalid_amount_of_options("invalid amount of options");
		ONE_DEF error invalid_character_for_option("invalid character for option");
		ONE_DEF error pmut_ct_error("a error has occurred in pmut-ct(PMUT's converion tree)");
		ONE_DEF error no_terminating_end_character_found("no terminating option end character found");
		ONE_DEF error invalid_postion_of_command_extractor_operators("invalid postion of command extractor operators");
		ONE_DEF error cannot_call_a_command_already_active("cannot call another command becuase one is already active");
	}
}

#endif //!PMUT_ERROR_H