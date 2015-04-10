#ifndef CMDLINE_PARSER_HPP
#define CMDLINE_PARSER_HPP

/* Utility operator that aids command line parsing.
 */

#ifndef CMDLINE_ARGS_H
#error "Do not use this header directly; include cmdline/args.h instead."
#endif

#include <sstream>

namespace cmdline {

    /* Uses the next() value of 'a' to write to 't'.
     * Any error that occours are written to a.log().
     *
     * This function is capable of parsing any typename T
     * for which operator>>( std::ostream&, T& ) is defined.
     */
    template <typename T>
    args & operator>>( args & a, T & t );


// Implementation

    template <typename T>
    args & operator>>( args & a, T & t ) {
        std::stringstream stream( a.next() );
        stream >> t;
        if( !stream ) {
            a.log() << "Error: could not parse " << stream.str() << ".\n";
            return a;
        }
        if( !stream.eof() ) {
            a.log() << "Warning: partially parsed string\n"
                << "Unparsed bit: '" << stream.str().substr(stream.tellg())
                << "'\n";
            return a;
        }
        return a;
    }

} // namespace cmdline

#endif // CMDLINE_PARSER_HPP
