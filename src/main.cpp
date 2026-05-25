#include <Battle/Battle.hpp>

#include <IO/System/EventLog.hpp>

#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
	using namespace sw;

	if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

	EventLog eventLog;
	battle::Battle battle(eventLog);

	battle.loadScenario(file);
	battle.run();

	return 0;
}
