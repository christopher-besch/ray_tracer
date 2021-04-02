#include "console.h"

#include <cstring>
#include <algorithm>

#include "utils.h"

////////////////////
// VectorArgument //
////////////////////
VectorArgument::VectorArgument(std::vector<const char *> init_arguments, int min_arguments, int max_arguments, bool optional)
    : m_init_arguments(init_arguments), m_min_arguments(min_arguments), m_max_arguments(max_arguments), m_optional(optional), m_found(false)
{
#ifdef DEBUG
    // checks
    if (m_init_arguments.empty())
        raise_error("Trying to create VectorArgument with no init arguments!");
    for (const char *init_argument : init_arguments)
        if (init_argument[0] != '-')
            raise_error("Init arguments have to start with '-'! '" << init_argument << "' is invalid!")
#endif
}

void VectorArgument::add_argument(const char *argument)
{
#ifdef DEBUG
    if (m_arguments.size() == m_max_arguments)
        raise_error("Trying to add more arguments than allowed!");
#endif
    m_arguments.push_back(argument);
}

bool VectorArgument::contains_init_argument(const char *init_argument) const
{
    // look mum I managed to cram three good lines of code into one bad one
    return std::find_if(m_init_arguments.begin(), m_init_arguments.end(), [&](const char *this_init_argument) { return std::strcmp(this_init_argument, init_argument) == 0; }) != m_init_arguments.end();
}

void VectorArgument::test_requirements() const
{
    if (!m_optional && !m_found)
        raise_error("Console Parameter " << m_init_arguments[0] << " is missing!");
    if (m_found && m_arguments.size() < m_min_arguments)
        raise_error("Console Parameter " << m_init_arguments[0] << " needs more arguments!");
}

//////////////////////
// ConsoleArguments //
//////////////////////
// test if all VectorArguments fullfill their requirements
void ConsoleArguments::test_requirements() const
{
    for (const VectorArgument &vector_argument : m_vector_arguments)
        vector_argument.test_requirements();
}

// returns vector argument with supplied init_argument
const VectorArgument &ConsoleArguments::operator[](const char *init_argument) const
{
    for (const VectorArgument &vector_argument : m_vector_arguments)
        if (vector_argument.contains_init_argument(init_argument))
            return vector_argument;
    raise_error("Can't find init argument " << init_argument << "!");
}

// return other argument at that position
const char *ConsoleArguments::operator[](int idx) const
{
#ifdef DEBUG
    if (idx < 0 || idx >= m_other_arguments.size())
        raise_error("Trying to access other arguments with invalid index!");
#endif
    return m_other_arguments[idx];
}

void ConsoleArguments::load_arguments(int argc, char *argv[])
{
    // will change once an init argument is found
    VectorArgument *current_vector_argument = nullptr;
    for (int idx = 0; idx < argc; idx++)
    {
        // is init argument
        if (argv[idx][0] == '-')
        {
            bool found = false;
            // find used vector argument
            for (VectorArgument &vector_argument : m_vector_arguments)
            {
                // update pointer if found
                if (vector_argument.contains_init_argument(argv[idx]))
                {
                    current_vector_argument = &vector_argument;
                    vector_argument.got_found();
                    found = true;
                    break;
                }
            }
            if (!found)
                raise_error("Unknown Parameter: " << argv[idx]);
        }
        // is other argument
        else
        {
            // when no init argument found before this
            if (!current_vector_argument)
                m_other_arguments.push_back(argv[idx]);
            else
            {
                current_vector_argument->add_argument(argv[idx]);
                // when enough found
                if (current_vector_argument->is_maxed())
                    current_vector_argument = nullptr;
            }
        }
    }
    test_requirements();
}
