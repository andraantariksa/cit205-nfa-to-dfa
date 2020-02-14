#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <set>
#include <gvc.h>

#define DEBUG 0

const char* DEAD_STATE = "∅";

// Split a string to set
// "a,b" -> ["a", "b"]
std::set<std::string> split_to_set(const std::string& in)
{
	std::set<std::string> out;
	std::string temp;
	for (char i : in)
	{
		// If there's a comma, then the temp string is appended to
		// output vector and clear the temp string
		if (i == ',')
		{
			out.insert(temp);
			temp.clear();
		}
		else
		{
			// while it's not comma, add the character to temp string
			temp.push_back(i);
		}
	}

	// Append the last string to output vector
	if (!temp.empty())
	{
		out.insert(temp);
	}

	return out;
}

// Aggregate the set from previous defined state
//
// e.g
//
// --------+------------+------------+
//         |          0 |          1 |
// --------+------------+------------+
//       a |        a,b |          b |
//       b |          c |          - |
//
// If we aggregate a,b 0's character, it will resulting in a,b,c
std::string aggregate(std::set<std::string> splitted, size_t lang_nth, std::vector<std::string>* state, std::vector<std::vector<std::string>>* route)
{
	std::set<std::string> set_accumulator;
	for (auto it = splitted.begin(); it != splitted.end(); ++it)
	{
		// Find each set language character
		int found_on_ith;
		for (found_on_ith = 0; found_on_ith < state->size(); ++found_on_ith)
		{
			if (*it == (*state)[found_on_ith])
			{
				break;
			}
		}

		// It's exists              It's not a dead state
		if (it != splitted.end() && (*route)[found_on_ith][lang_nth] != DEAD_STATE)
		{
			std::set<std::string> splitted2 = split_to_set((*route)[found_on_ith][lang_nth]);
			for (const std::string& it2 : splitted2)
			{
				set_accumulator.insert(it2);
			}
		}
	}

	// If there's no result from aggregation, then it must be a dead state
	if (set_accumulator.empty())
	{
		return DEAD_STATE;
	}

	std::string resulting_accumulator;
	resulting_accumulator += *set_accumulator.begin();
	for (auto it = ++set_accumulator.begin(); it != set_accumulator.end(); ++it)
	{
		resulting_accumulator += ',' + *it;
	}

	return resulting_accumulator;
}

std::string set_to_string(const std::set<std::string>& set)
{
	std::string out;

	if (set.empty())
	{
		return out;
	}

	// Add the first (0-th) element
	// a
	out += *set.begin();
	// Add the 1-st to n-th element
	// Add the next element with first
	// ,b
	// ,c
	for (auto it = ++set.begin(); it != set.end(); ++it)
	{
		out += "," + *it;
	}

	return out;
}

void f(size_t state_nth, size_t lang_nth, std::vector<std::string>* state, std::vector<std::vector<std::string>>* route, std::vector<std::string> *language, size_t* size_state)
{
	std::string current_set = (*route)[state_nth][lang_nth];
	auto it = std::find(state->begin(), state->end(), current_set);
	if (current_set.empty())
	{
		(*route)[state_nth][lang_nth] = aggregate(split_to_set((*state)[state_nth]), lang_nth, state, route);
		f(state_nth, lang_nth, state, route, language, size_state);
	}
	else if (it == state->end() && current_set != DEAD_STATE)
	{
		state->push_back(current_set);
		std::vector<std::string> empty_routes(language->size());
		route->push_back(empty_routes);
		*size_state += 1;
	}

#ifdef DEBUG
#ifdef DEBUG_VERBOSE
	printf("===================================================\n");
	for (int i = 0; i < state->size(); ++i)
	{
		printf("%8s | ", (*state)[i].c_str());
		for (int j = 0; j < (*route)[i].size(); ++j)
		{
			printf("%10s | ", (*route)[i][j].c_str());
		}
		printf("\n");
	}
#endif
#endif
}

int main()
{
	std::string starting_state = "q0";
	std::vector<std::string> final_states = { "q2" };

	std::vector<std::string> language;
	language.push_back("0");
	language.push_back("1");

	std::vector<std::string> state;
	state.push_back("q0");
	state.push_back("q1");
	state.push_back("q2");

	std::vector<std::vector<std::string>> route;
	// - = dead state
	std::vector<std::string> temp;
	// In state q0
	temp.push_back("q0,q1");
	temp.push_back("q0");
	route.push_back(temp);
	temp.clear();
	// In state q1
	temp.push_back(DEAD_STATE);
	temp.push_back("q2");
	route.push_back(temp);
	temp.clear();
	// In state q2
	temp.push_back(DEAD_STATE);
	temp.push_back(DEAD_STATE);
	route.push_back(temp);
	temp.clear();

	// Sort every set in state
	for (auto &it_state : state)
	{
		it_state = set_to_string(split_to_set(it_state));
	}

	// Sort every route set
	for (auto &it_state : route)
	{
		for (auto &it_language: it_state)
		{
			it_language = set_to_string(split_to_set(it_language));
		}
	}

#ifdef DEBUG
	for (int i = 0; i < state.size(); ++i)
	{
		printf("%8s | ", state[i].c_str());
		for (int j = 0; j < route[i].size(); ++j)
		{
			printf("%10s | ", route[i][j].c_str());
		}
		printf("\n");
	}
#endif

	size_t size_state = state.size();
	size_t size_lang = language.size();
	for (int state_nth = 0; state_nth < size_state; ++state_nth)
	{
		for (int lang_nth = 0; lang_nth < size_lang; ++lang_nth)
		{
			// std::cout << "a" << state_nth << " and " << lang_nth << "\n";
			f(state_nth, lang_nth, &state, &route, &language, &size_state);
			// std::cout << "end a\n";
		}
	}

#ifdef DEBUG
	printf("===================================================\n");

	for (int i = 0; i < state.size(); ++i)
	{
		printf("%8s | ", state[i].c_str());
		for (int j = 0; j < route[i].size(); ++j)
		{
			printf("%10s | ", route[i][j].c_str());
		}
		printf("\n");
	}
#endif

	GVC_t *gvc = gvContext();

	Agraph_t *graph = agopen("NFA to DFA", Agdirected, nullptr);
	Agnode_t *from, *to, *start;
	Agedge_t *edge;

	for (int i = 0; i < state.size(); ++i)
	{
		for (int j = 0; j < route[i].size(); ++j)
		{
			from = agnode(graph, (char*)state[i].c_str(), true);
			agsafeset(from, "shape", "circle", "");
			to = agnode(graph, (char*)route[i][j].c_str(), true);
			agsafeset(to, "shape", "circle", "");
			edge = agedge(graph, from, to, "", true);
			agsafeset(edge, "label", (char*)language[j].c_str(), "");

			if (state[i] == starting_state)
			{
				start = agnode(graph, "", true);
				agsafeset(start, "shape", "point", "");
				agedge(graph, start, from, "", true);
			}

			for (auto final_state: final_states)
			{
				if (state[i].find(final_state) != std::string::npos)
				{
					agsafeset(from, "shape", "doublecircle", "");
					break;
				}
			}
		}
	}

	gvLayout(gvc, graph, "dot");
	gvRender(gvc, graph, "png", fopen("res.png", "w"));

	gvFreeLayout(gvc, graph);
	agclose(graph);

	return 0;
}