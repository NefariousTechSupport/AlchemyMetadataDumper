//=============================================================================
// Includes
//-----------------------------------------------------------------------------
#include <cstdio>
#include <string>





//=============================================================================
// Command declarations
//-----------------------------------------------------------------------------
int ComputeBases(const char* inDol, const char* outMakefile);
int Inject(const char* gameDol, const char* injectDol, const char* outDol);




//=============================================================================
// PrintHelp : Prints the help section
//-----------------------------------------------------------------------------
void PrintHelp()
{
	printf(
		"dolinject - Static code injection command line tool for DOL binaries\n"
		"\n"
		"dolinject <subcommand> <flags>\n"
		"\n"
		"Subcommands:\n"
		"\t- compute-bases <input dol> <output makefile>\n"
		"\t  Computes the desired .text, .data, and .bss sections to compile your\n"
		"\t  injected code with from the provided and outputs a makefile with the settings\n"
		"\t  to be used in compiling the code.\n"
		"\n"
		"\t- inject <game dol> <injection dol> <output dol>\n"
		"\t  Does the actual code injection. It takes in two input .dols and an output .dol\n"
	);
}





//=============================================================================
// main
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	int result = 1;

	if (argc < 2)
	{
		PrintHelp();
	}

	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];

		if (arg == "--help" || arg == "help")
		{
			PrintHelp();
			break;
		}
		else if (arg == "compute-bases")
		{
			if (i + 2 >= argc)
			{
				PrintHelp();
				break;
			}

			result = ComputeBases(argv[i + 1], argv[i + 2]);
			break;
		}
		else if (arg == "inject")
		{
			if (i + 3 >= argc)
			{
				PrintHelp();
				break;
			}

			result = Inject(argv[i + 1], argv[i + 2], argv[i + 3]);
			break;
		}
	}

	return result;
}