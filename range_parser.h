#ifndef CMDLINE_RANGE_PARSER_H
#define CMDLINE_RANGE_PARSER_H

namespace cmdline {

    class args;

    /* Class that encapsulate the range information
     * that will be used to parse a number from a cmdline::args,
     * using the operator>> below.
     *
     * The number should be contained in the range [min, max].
     * If min == max, then the range is [min, \infty).
     * If the number falls outside the range,
     * an adequate error message is written to the cmdline::args.
     *
     * Note that the number is still parsed and assigned to the
     * operator's right-hand side.
     */
    class range_parser {
        double min;
        double max;
        cmdline::args & _args;
    public:
        range_parser( cmdline::args & _args, long long min, long long max ) :
            min( min ),
            max( max ),
            _args( _args )
        {}
        range_parser( cmdline::args & _args, long long min ) :
            min( min ),
            max( min ),
            _args( _args )
        {}
        template< typename Number >
        friend void operator>>( range_parser &&, Number & );
    };

    // Implementation of operator>> is in parser.hpp.
} // namespace cmdline

#endif // CMDLINE_RANGE_PARSER_H
