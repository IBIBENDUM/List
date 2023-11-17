#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "console_args.h"

bool handle_cmd_args(const int argc, char* const* argv, const char* format, Args_values* values)
{
    assert(argv);
    assert(argc);
    assert(values);

    if (!format)
    {
        LOG_ERROR("Empty format!\n");
        return false;
    }

    int arg = 0;
    while ((arg = getopt(argc, argv, format)) != -1)
    {
        switch (arg)
        {
            case 'm':
            {
                bool is_level_found = false;
                for (size_t i = 0; i < LOG_AMOUNT_OF_LVLS; i++)
                {
                    if (strcmp(optarg, log_levels_strings[i]) == 0)
                    {
                        values->log_level = (log_level) i;
                        set_log_level(values->log_level);
                        is_level_found = true;
                        break;
                    }
                }
                if (is_level_found)
                    break;

                LOG_ERROR("There is no %s log level", optarg);
                return false;
            }

            case 'h':
            {
                print_help(format);
                return false;
            }

            default:
            {
                LOG_ERROR("Unknown option found\n");
                print_help(format);

                return false;
            }
        }
    }
    return true;
}

void print_help(const char* format)
{
    printf("OPTIONS:\n");
    for (int i = 0; i < strlen(format); i++)
    {
        if (format[i] != ':')
        {
            const char arg = format[i];
            switch (arg)
            {
                case 'h':   printf("-h             Display help message\n"); break;
                case 'm':   printf("-m             Set log level (TRACE, DEBUG, INFO, WARN, ERROR, DISABLE)\n"); break;
                default:    LOG_ERROR("No help information"); break; // ???
            }
        }
    }
}

