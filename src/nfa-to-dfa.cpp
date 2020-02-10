// TODO
// Harus disort
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>

// size_t = unsigned int
size_t STATE_NUM = 2;
size_t LANG_CHAR = 2;

std::vector<std::vector<std::string>> route(STATE_NUM, std::vector<std::string>(LANG_CHAR));
std::vector<std::string> state(STATE_NUM);

// string "a,b" -> vector ["a", "b"]
std::vector<std::string> split(std::string in)
{
    std::vector<std::string> out;
    std::string temp;
    for (auto i = in.begin(); i != in.end(); ++i)
    {
        if (*i == ',')
        {
            out.push_back(temp);
            temp.clear();
        }
        else
        {
            temp.push_back(*i);
        }
    }
    return out;
}

std::string aggregate(std::vector<std::string> splitted, size_t lang_nth, std::vector<std::string>* state, std::vector<std::vector<std::string>>* route)
{
    std::string acc;
    for (auto it = splitted.begin(); it != splitted.end(); ++it)
    {
        // find function
        int found_on_ith;
        for (found_on_ith = 0; found_on_ith < state->size(); ++found_on_ith)
        {
            if (*it == (*state)[found_on_ith])
            {
                break;
            }
        }

        if (it != splitted.end())
        {
            acc += (*route)[found_on_ith][lang_nth];
        }
    }

    return acc;
}

void f(size_t state_nth, size_t lang_nth, std::vector<std::string>* state, std::vector<std::vector<std::string>>* route, size_t* size_state)
{
    std::string current_set = (*route)[state_nth][lang_nth];
    auto it = std::find(state->begin(), state->end(), current_set);
    if (current_set.empty())
    {
        (*route)[state_nth][lang_nth] = aggregate(split((*state)[state_nth]), lang_nth, state, route);
        f(state_nth, lang_nth, state, route, size_state);
    }
    else if (it == state->end() && current_set != "-")
    {
        state->push_back(current_set);
        std::vector<std::string> empty_routes(LANG_CHAR);
        route->push_back(empty_routes);
        *size_state += 1;
    } 

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
}

int main()
{
    state[0] = "a";
    state[1] = "b";

    // - = dead state
    route[0][0] = "a,b";
    route[0][1] = "b";
    route[1][0] = "-";
    route[1][1] = "-";
    // route[2][0] & [2][1]

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
}