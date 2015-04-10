#ifndef CMDLINE_ARGS_H
#define CMDLINE_ARGS_H

/* Class that encapsulates a command line argument vector.
 *
 * This class allows bash-style command line argument parsing
 * and helps the creation of sub-command lines.
 *
 * Strong exception safety guarantee: if any member throws an exception,
 * the object is guaranteed to be left untouched.
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
        /* Constructs the argument vector from the given argc and argv.
         * Note we do not change argv,
         * nor assume argv[argc] is actually the null pointer.
         *
         * Observe that the avaliabre arguments will start from argv[1];
         * argv[0] is stored as the program_name.
         *
         * By default, the logging device is std::cerr.
         */
        args( int argc, char const * const * argv );

        /* Constructs an empty argument vector.
         * You need to use the args::push_back method to populate this object,
         * and args::program_name to configure its name.
         */
        args();


        /* Returns the number of remaining strings in the argument vector.
         */
        std::size_t size() const;

        /* Take a look into the next argument,
         * without changing the argument vector state.
         *
         * If there is no strings left, throws std::out_of_range.
         */
        std::string peek() const;

        /* Shifts the argument vector by one position.
         *
         * If there is no strings left, throws std::out_of_range.
         */
        void shift();

        /* Obtains the next string and shifts the argument vector
         * by one position.
         *
         * If there is no strings left, throws std::out_of_range.
         */
        std::string next();

        /* Appends the given string to the argument vector.
         */
        void push_back( std::string );

        /* Constructs a "subargument" vector from the current position.
         * The following 'size' arguments will be used as argument vectors.
         *
         * program_name will be empty.
         *
         * Advances the argument vector by 'size' positions.
         *
         * If there isn't enough strings left, throws std::out_of_range.
         */
        args subarg( std::size_t size );

        /* Same as subarg, but args::peek() will be used as program_name
         * for the returned argument vector.
         *
         * This advances the argument vector by 'size + 1' positions.
         *
         * If there isn't enough strings left, throws std::out_of_range.
         */
        args subcmd( std::size_t size );

        /* Sets/retrieves the log stream.
         * This stream should be used to indicate command line argument errors;
         * for instance, operator>> writes to this log
         * if it could not correctly parse some argument.
         */
        void log( std::ostream & );
        std::ostream & log();

        /* Sets/retrieves the program name. */
        void program_name( const std::string & );
        const std::string & program_name() const;
    };

} // namespace cmdline

#include "cmdline/parser.hpp"

#endif // CMDLINE_ARGV_H
