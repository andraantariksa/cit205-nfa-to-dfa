#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <set>
#include <gvc.h>

// Constant
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
}

int main()
{
	unsigned int total_state;
	std::cout << "Total state: ";
	std::cin >> total_state;

	// State specification
	std::vector<std::string> state;
	
	for (int i = 0; i < total_state; ++i)
	{
		std::string temp_state;
		std::cout << i + 1 << ". ";
		std::cin >> temp_state;
		state.push_back(temp_state);
	}

	// Starting state can only be one
	std::string starting_state;
	
	std::cout << "Starting state: ";
	std::cin >> starting_state;

	unsigned int total_final_states;
	std::cout << "Total final states: ";
	std::cin >> total_final_states;

	// There may be multiple final states
	std::vector<std::string> final_states;

	for (int i = 0; i < total_final_states; ++i)
	{
		std::string temp_final_state;
		std::cout << i + 1 << ". ";
		std::cin >> temp_final_state;
		final_states.push_back(temp_final_state);
	}

	unsigned int total_language_letter;
	std::cout << "Total language letter: ";
	std::cin >> total_language_letter;

	// Language specification
	std::vector<std::string> language;
	
	for (int i = 0; i < total_language_letter; ++i)
	{
		std::string temp_language_letter;
		std::cout << i + 1 << ". ";
		std::cin >> temp_language_letter;
		language.push_back(temp_language_letter);
	}

	std::cout << "Enter the route, separated with space on each language\n";

	// Route has equal length with state, and the inside has equal length with language
	std::vector<std::vector<std::string>> route;
	
	for (int i = 0; i < total_state; ++i)
	{
		std::vector<std::string> temp;
		std::cout << state[i] << ". ";

		for (int j = 0; j < total_language_letter; ++j)
		{
			std::string temp_route_set;
			std::cin >> temp_route_set;
			temp.push_back(temp_route_set);
		}
		route.push_back(temp);
	}
	

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

// print
	printf("NFA table\n");
	for (int i = 0; i < 12 + 13 * language.size(); ++i)
	{
		printf("=");
	}
	printf("\n");

	printf("%10s |", "State");
	for (std::string &language_letter: language)
	{
		printf("%11s |", language_letter.c_str());
	}
	printf("\n");

	for (int i = 0; i < 12 + 13 * language.size(); ++i)
	{
		printf("=");
	}
	printf("\n");
	for (int i = 0; i < state.size(); ++i)
	{
		printf("%10s | ", state[i].c_str());
		for (int j = 0; j < route[i].size(); ++j)
		{
			printf("%10s | ", route[i][j].c_str());
		}
		printf("\n");
	}
// end print table

	// State size are saved because it may be modified when iterating on below
	size_t size_state = state.size();
	size_t size_lang = language.size();
	// Iterating the state
	for (int state_nth = 0; state_nth < size_state; ++state_nth)
	{
		// Iterating the language
		for (int lang_nth = 0; lang_nth < size_lang; ++lang_nth)
		{
			solve(state_nth, lang_nth, &state, &route, &language, &size_state);
		}
	}

// print
	printf("DFA table\n");
	for (int i = 0; i < 12 + 13 * language.size(); ++i)
	{
		printf("=");
	}
	printf("\n");

	printf("%10s |", "State");
	for (std::string &language_letter: language)
	{
		printf("%11s |", language_letter.c_str());
	}
	printf("\n");

	for (int i = 0; i < 12 + 13 * language.size(); ++i)
	{
		printf("=");
	}
	printf("\n");
	for (int i = 0; i < state.size(); ++i)
	{
		printf("%10s | ", state[i].c_str());
		for (int j = 0; j < route[i].size(); ++j)
		{
			printf("%10s | ", route[i][j].c_str());
		}
		printf("\n");
	}
// end print table

	// Initialize Graphviz
	GVC_t *gvc = gvContext();

	// Create the graph instance           Use directed graph
	Agraph_t *graph = agopen("NFA to DFA", Agdirected, nullptr);
	// Horizontal graph
	agsafeset(graph, "rankdir", "LR", "");
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
			// Get the edge label
			char* label_c_string = agget(edge, "label");
			// If there's no label exists OR The label length is 0
			if (label_c_string == nullptr || strlen(label_c_string) == 0)
			{
				// Set the label as the language
				agsafeset(edge, "label", (char*)language[j].c_str(), "");
			}
			// If there's a label exist
			else
			{
				// Old label + current language
				std::string label_temp = label_c_string;
				label_temp += "," + language[j];
				agsafeset(edge, "label", (char*)label_temp.c_str(), "");
			}

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

			// Same with above
			for (auto final_state: final_states)
			{
				// If the string contains ${final_state}, then the to node to double circle
				if (route[i][j].find(final_state) != std::string::npos)
				{
					agsafeset(to, "shape", "doublecircle", "");
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