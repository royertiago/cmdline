#ifndef CMDLINE_ARGS_H
#define CMDLINE_ARGS_H

/* Class that encapsulates a command line argument vector.
 *
 * This class allows bash-style command line argument parsing
 * and helps the creation of sub-command lines.
 */

#include <ostream>
#include <string>
#include <vector>

namespace cmdline {

    class args {
        std::vector< std::string > _args;
        std::string _program_name;
        std::size_t _index;

        std::ostream * _log;

    public:

        args( int argc, char const * const * argv );

        /* Returns the number of strings
         * in the argument vector.
         */
        std::size_t size() const;

        /* Take a look into the next argument,
         * without changing the argument vector state.
         */
        std::string peek() const;

        /* Shifts the argument vector by one position.
         */
        void shift();

        /* Obtains the next string and shifts the argument vector
         * by one position.
         */
        std::string next();

        /* Sets/retrieves the log stream. */
        void log( std::ostream & );
        std::ostream & log();

    };

} // namespace cmdline

#include "cmdline/parser.hpp"

#endif // CMDLINE_ARGV_H
