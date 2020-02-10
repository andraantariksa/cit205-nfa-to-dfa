#include <cstdio>
#include <string>
#include <unordered_map>

#include <CSVparser.cpp>

std::vector<std::string> parseSet(std::string &set)
{

}

struct Row
{
	std::string set;
	std::vector<std::string> to;
};

int main(int argc, char **argv)
{
	csv::Parser file = csv::Parser("test.csv");

	return 0;
}
