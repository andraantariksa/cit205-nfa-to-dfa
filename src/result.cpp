#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <set>
#include <gvc.h>

// size_t = unsigned int
size_t STATE_NUM = 3;
size_t LANG_CHAR = 2;

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
		if (it != splitted.end() && (*route)[found_on_ith][lang_nth] != "-")
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
		return "-";
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

void f(size_t state_nth, size_t lang_nth, std::vector<std::string>* state, std::vector<std::vector<std::string>>* route, size_t* size_state)
{
	std::string current_set = (*route)[state_nth][lang_nth];
	auto it = std::find(state->begin(), state->end(), current_set);
	if (current_set.empty())
	{
		(*route)[state_nth][lang_nth] = aggregate(split_to_set((*state)[state_nth]), lang_nth, state, route);
		f(state_nth, lang_nth, state, route, size_state);
	}
	else if (it == state->end() && current_set != "-")
	{
		state->push_back(current_set);
		std::vector<std::string> empty_routes(LANG_CHAR);
		route->push_back(empty_routes);
		*size_state += 1;
	}

//	printf("===================================================\n");
//	for (int i = 0; i < state->size(); ++i)
//	{
//		printf("%8s | ", (*state)[i].c_str());
//		for (int j = 0; j < (*route)[i].size(); ++j)
//		{
//			printf("%10s | ", (*route)[i][j].c_str());
//		}
//		printf("\n");
//	}
}

int main()
{
	// graphviz

	Agraph_t *graph;
	Agnode_t *from, *to;
	Agedge_t *edge;

	static GVC_t *gvc;

	if (!gvc)
		gvc = gvContext();

	graph = agopen("g", Agdirected, nullptr);

	// end graphviz

	std::vector<std::vector<std::string>> route(STATE_NUM, std::vector<std::string>(LANG_CHAR));
	std::vector<std::string> state(STATE_NUM);

	state[0] = "a";
	state[1] = "b";
	state[2] = "c";

	// - = dead state
	route[0][0] = "a,b";
	route[0][1] = "a,c";
	route[1][0] = "b";
	route[1][1] = "c";
	route[2][0] = "c";
	route[2][1] = "c,a";

	for (auto &it : state)
	{
		it = set_to_string(split_to_set(it));
	}

	for (auto &it_ : route)
	{
		for (auto &it: it_)
		{
			it = set_to_string(split_to_set(it));
		}
	}

	for (int i = 0; i < state.size(); ++i)
	{
		printf("%8s | ", state[i].c_str());
		for (int j = 0; j < route[i].size(); ++j)
		{
			printf("%10s | ", route[i][j].c_str());
		}
		printf("\n");
	}

	size_t size_state = state.size();
	size_t size_lang = LANG_CHAR;
	for (int state_nth = 0; state_nth < size_state; ++state_nth)
	{
		for (int lang_nth = 0; lang_nth < size_lang; ++lang_nth)
		{
			// std::cout << "a" << state_nth << " and " << lang_nth << "\n";
			f(state_nth, lang_nth, &state, &route, &size_state);
			// std::cout << "end a\n";
		}
	}

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

//  graphviz

	for (int i = 0; i < state.size(); ++i)
	{
		for (int j = 0; j < route[i].size(); ++j)
		{
			from = agnode(graph, (char*)std::string(state[i]).c_str(), true);
			agsafeset(from, "shape", "circle", "");
			to = agnode(graph, (char*)std::string(route[i][j]).c_str(), true);
			agsafeset(to, "shape", "circle", "");
			edge = agedge(graph, from, to, "", true);
			agsafeset(edge, "label", (char*)std::string(1, '0' + j).c_str(), "");
		}
	}

	gvLayout(gvc, graph, "dot");

	gvRender(gvc, graph, "png", fopen("res.png", "w"));

	gvFreeLayout(gvc, graph);

	agclose(graph);
// end graphviz
	return 0;
}