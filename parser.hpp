#ifndef CMDLINE_PARSER_HPP
#define CMDLINE_PARSER_HPP

/* Utility operator that aids command line parsing.
 */

#ifndef CMDLINE_ARGS_H
#error "Do not use this header directly; include cmdline/args.h instead."
#endif

#include <sstream>

namespace cmdline {

    template <typename T>
    args & operator>>( args & a, T & t ) {
        std::sstream stream( a.next() );
        stream >> t;
        if( !stream ) {
            a.log() << "Error: could not parse " << stream.str() << ".\n";
            return args;
        }
        if( !stream.eof() ) {
            a.log() << "Warning: partially parsed string\n"
                << "Unparsed bit: '" << stream.str.substr(stream.tellg())
                << "'\n";
            return args;
        }
        return args;
    }

} // namespace cmdline

#endif // CMDLINE_PARSER_HPP
