#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <set>
#include <gvc.h>

#define DEBUG 0

const char* DEAD_STATE = "-";

std::set<std::string> split_string_to_set(const std::string& in)
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

std::string set_to_string(const std::set<std::string>& set)
{
	std::string out;

	if (set.empty())
	{
		return out;
	}

	// e.g
	// a,b,c
	//
	// Add the first (0-th) element
	// a
	out += *set.begin();
	// Add the 1-st to n-th element
	// ,b
	// ,c
	for (auto it = ++set.begin(); it != set.end(); ++it)
	{
		out += "," + *it;
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
// If we aggregate a and b 0's character, it will resulting in a,b,c
std::string aggregate(std::set<std::string> splitted, size_t lang_nth, std::vector<std::string>* state, std::vector<std::vector<std::string>>* route)
{
	std::set<std::string> set_accumulator;
	// Iterate on the splitted set
	for (auto it = splitted.begin(); it != splitted.end(); ++it)
	{
		// Find each set on the state
		int found_on_ith;
		for (found_on_ith = 0; found_on_ith < state->size(); ++found_on_ith)
		{
			// If the set is found on the state
			if (*it == (*state)[found_on_ith])
			{
				// Get out from the iteration
				break;
			}
		}

		// It's exists              It's not a dead state
		if (it != splitted.end() && (*route)[found_on_ith][lang_nth] != DEAD_STATE)
		{
			// Get all of the set items to set_accumulator variable
			std::set<std::string> splitted2 = split_string_to_set((*route)[found_on_ith][lang_nth]);
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

	return set_to_string(set_accumulator);
}

// Main algorithm of the NFA to DFA
void solve(size_t state_nth, size_t lang_nth, std::vector<std::string>* state, std::vector<std::vector<std::string>>* route, std::vector<std::string> *language, size_t* size_state)
{
	// Current route target
	std::string current_set = (*route)[state_nth][lang_nth];
	// If it is found, it != state->end()
	auto it = std::find(state->begin(), state->end(), current_set);
	if (current_set.empty())
	{
		(*route)[state_nth][lang_nth] = aggregate(split_string_to_set((*state)[state_nth]), lang_nth, state, route);
		// Recurse on the result, because the result may produce non existing state
		solve(state_nth, lang_nth, state, route, language, size_state);
	}
	//       If it's not exists on state
	else if (it == state->end())//          && current_set != DEAD_STATE)
	{
		// Add the state from the current set
		state->push_back(current_set);
		// Add the route with empty string with the length equal with the language length
		std::vector<std::string> empty_routes(language->size());
		route->push_back(empty_routes);
		// Because the state is increased by one, we add the size state. So, the iteration on the main
		// function will increased
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
	// Starting state can only be one
	std::string starting_state = "0";
	// There may be multiple final states
	std::vector<std::string> final_states = { "1" };

	// Language specification
	std::vector<std::string> language;
	language.push_back("a");
	language.push_back("b");

	// State specification
	std::vector<std::string> state;
	state.push_back("0");
	state.push_back("1");
	state.push_back("2");
	state.push_back("3");
	state.push_back("4");

	// Route has equal length with state, and the inside has equal length with language
	std::vector<std::vector<std::string>> route;
	// - = dead state
	std::vector<std::string> temp;
	// In state 0
	temp.push_back("1,2,3");             // a
	temp.push_back("2,3");        // b
	route.push_back(temp);
	temp.clear();
	// In state 1
	temp.push_back("1,2");             // a
	temp.push_back("2,3");        // b
	route.push_back(temp);
	temp.clear();
	// In state 2
	temp.push_back(DEAD_STATE);        // a
	temp.push_back("2,3,4");             // b
	route.push_back(temp);
	temp.clear();
	// In state 3
	temp.push_back("4");             // a
	temp.push_back("2,3,4");        // b
	route.push_back(temp);
	temp.clear();
	// In state 4
	temp.push_back(DEAD_STATE);             // a
	temp.push_back(DEAD_STATE);             // b
	route.push_back(temp);
	temp.clear();

	// Iterate the state
	for (auto &it_state : state)
	{
		it_state = set_to_string(split_string_to_set(it_state));
	}

	// Iterate the state
	for (auto &it_state : route)
	{
		// Iterate the language letter
		for (auto &it_language: it_state)
		{
			it_language = set_to_string(split_string_to_set(it_language));
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

	// State size are saved because it may be modified when iterating on below
	size_t size_state = state.size();
	size_t size_lang = language.size();
	// Iterating the state
	for (int state_nth = 0; state_nth < size_state; ++state_nth)
	{
		// Iterating the language
		for (int lang_nth = 0; lang_nth < size_lang; ++lang_nth)
		{
#ifdef DEBUG
			std::cout << "a" << state_nth << " and " << lang_nth << "\n";
#endif
			solve(state_nth, lang_nth, &state, &route, &language, &size_state);
#ifdef DEBUG
			std::cout << "end a\n";
#endif
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

	// Initialize Graphviz
	GVC_t *gvc = gvContext();

	// Create the graph instance           Use directed graph
	Agraph_t *graph = agopen("NFA to DFA", Agdirected, nullptr);
	Agnode_t *from, *to, *start;
	Agedge_t *edge;
	
	// Iterate on state
	for (int i = 0; i < state.size(); ++i)
	{
		// Iterate on language
		for (int j = 0; j < route[i].size(); ++j)
		{
			from = agnode(graph, (char*)state[i].c_str(), true);
			agsafeset(from, "shape", "circle", "");

			to = agnode(graph, (char*)route[i][j].c_str(), true);
			agsafeset(to, "shape", "circle", "");

			edge = agedge(graph, from, to, "", true);
			agsafeset(edge, "label", (char*)language[j].c_str(), "");

			// Set starting node if it's a starting state
			if (state[i] == starting_state)
			{
				start = agnode(graph, "", true);
				agsafeset(start, "shape", "point", "");
				agedge(graph, start, from, "", true);
			}

			// Iterate the final state
			for (auto final_state: final_states)
			{
				// If the string contains ${final_state}, then the from node to double circle
				if (state[i].find(final_state) != std::string::npos)
				{
					agsafeset(from, "shape", "doublecircle", "");
					break;
				}
			}
		}
	}

	// Use Dot engine to render the graph
	// DOT is a graph description language. DOT graphs are typically files with the
	// filename extension gv or dot. The extension gv is preferred, to avoid confusion
	// with the extension dot used by versions of Microsoft Word before 2007.
	// https://en.wikipedia.org/wiki/DOT_(graph_description_language)
	gvLayout(gvc, graph, "dot");
	// Save as PNG
	gvRender(gvc, graph, "png", fopen("result.png", "w"));
	// Free the memory
	gvFreeLayout(gvc, graph);
	agclose(graph);

	return 0;
}