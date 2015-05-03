#ifndef CMDLINE_ARGS_H
#define CMDLINE_ARGS_H

/* Class that encapsulates a command line argument vector.
 *
 * This class allows bash-style command line argument parsing
 * and helps the creation of sub-command lines.
 *
 * Strong exception safety guarantee: if any member throws an exception,
 * the object is guaranteed to be left untouched.
 *
 * This header also contains one helper class, range_parser.
 */

#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace cmdline {

    class args;
    class range_parser;

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

        /* Returns the total number of strings stored in the argument vector.
         */
        std::size_t total_size() const;

        /* Take a look into the next argument,
         * without changing the argument vector state.
         *
         * If there is no strings left, throws std::out_of_range.
         */
        std::string peek() const;

        /* Take a look into the index-th next argument.
         * If index == 0, this function has the same effect as peek().
         *
         * Note that index may be negative;
         * the method will retrieve previous arguments.
         *
         * Throws std::out_of_range if there is not enough arguments.
         */
        std::string peek( int index ) const;

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

        /* Retrurns a range parser to parse the next command line option.
         *
         * Use like this:
         *  cmdline::args args( argc, argv );
         *  int i;
         *  if( args.next() == "--val" )
         *      args.range( 2, 14 ) >> i;
         *
         * If i is not between 2 and 14, an error will be written to log().
         * The other overload merely specifies the minimum value.
         *
         * Observation: the error message is based on the previous argument.
         * If you are scanning several different arguments,
         * use operator>> directly and supply your own error messages.
         */
        cmdline::range_parser range( double min );
        cmdline::range_parser range( double min, double max );

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

        /* Constructs a "subargument" vector beginning from the current position
         * and ending when the boolean predicate is true.
         *
         * If the end of the argument vector is reached
         * before the predicate being true,
         * returns all the remaining arguments.
         *
         * program_name will be empty.
         *
         * Note that this will advance the argument vector.
         */
        args subarg_until( bool (* predicate )(const std::string&) );

        /* Same as subarg, but args::peek() will be used as program_name
         * for the returned argument vector.
         * args::peek() itself will not appear in the returned vector.
         *
         * This advances the argument vector by 'size + 1' positions.
         * If there isn't enough strings left, throws std::out_of_range.
         */
        args subcmd( std::size_t size );

        /* Same as subarg_until, but uses args::peek() as program_name.
         * args::peek() itself will not be tested against the predicate,
         * nor appear as an argument in the returned argument vector.
         */
        args subcmd_until( bool (* predicate )(const std::string&) );

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

    /* Uses the next() value of 'a' to write to 't'.
     * Any error that occours are written to a.log().
     *
     * This function is capable of parsing any typename T
     * for which operator>>( std::ostream&, T& ) is defined.
     */
    template <typename T>
    args & operator>>( args & a, T & t );

// Class implementation

inline args::args( int argc, char const * const * argv ) {
    _program_name = argv[0];
    _args.reserve( argc - 1 );
    for( int i = 1; i < argc; i++ )
        _args.push_back( argv[i] );
    _index = 0;

    _log = &std::cerr;
}

inline args::args() {
    _index = 0;
    _log = &std::cerr;
}

inline std::size_t args::size() const {
    return _args.size() - _index;
}

inline std::size_t args::total_size() const {
    return _args.size();
}

inline std::string args::peek() const {
    if( _index >= _args.size() )
        throw std::out_of_range( "No argument left to peek." );

    return _args[_index];
}

inline std::string args::peek( int index ) const {
    if( _index + index >= _args.size() )
        throw std::out_of_range( "Argument vector too short." );
    if( _index + index < 0 )
        throw std::out_of_range( "The index must not become negative." );

    return _args[_index + index];
}

inline void args::shift() {
    if( _index >= _args.size() )
        throw std::out_of_range( "No arguments left to shift." );

    _index++;
}

inline std::string args::next() {
    /* Peek takes care of throwing for us. */
    std::string ret = peek();
    shift();
    return ret;
}

inline void args::push_back( std::string str ) {
    _args.push_back( str );
}

inline range_parser args::range( double min ) {
    return range_parser( *this, min );
}

inline range_parser args::range( double min, double max ) {
    return range_parser( *this, min, max );
}

inline args args::subarg( std::size_t size ) {
    if( _index + size >= _args.size() + 1 )
        throw std::out_of_range( "Not enough arguments to form subarg." );

    args ret;
    ret._args = std::vector<std::string>(
        _args.begin() + _index,
        _args.begin() + _index + size
    );
    _index += size;
    return ret;
}

inline args args::subarg_until( bool (* predicate )(const std::string&) ) {
    args ret;
    auto it = _args.begin() + _index;
    unsigned size = 0;
    for( ; it < _args.end(); ++it, ++size )
        if( predicate( *it ) )
            break;

    ret._args = std::vector<std::string>( _args.begin() + _index, it );
    _index += size;
    return ret;
}

inline args args::subcmd( std::size_t size ) {
    std::string name = next();
    args ret = subarg( size );
    ret.program_name( name );
    return ret;
}

inline args args::subcmd_until( bool (* predicate )(const std::string&) ) {
    std::string name = next();
    args ret = subarg_until( predicate );
    ret.program_name( name );
    return ret;
}

inline void args::log( std::ostream & os ) {
    _log = &os;
}

inline std::ostream & args::log() {
    return *_log;
}

inline void args::program_name( const std::string & name ) {
    _program_name = name;
}

inline const std::string & args::program_name() const {
    return _program_name;
}

// Operators implementation

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

/* We must declare this operator as taking a rvalue reference
 * instead of a normal reference
 * because args::range return a range_parser by value,
 * thus making it a rvalue, not a lvalue. */
template< typename Number >
void operator>>( range_parser && range, Number & n ) {
    std::string error;
    if( range._args.size() < range._args.total_size() )
        error = "Error: argument to " + range._args.peek(-1);
    else
        error = "Error: number";

    range._args >> n;
    if( n < range.min ) {
        range._args.log() << error << " must be greater than "
                << (Number) range.min << ".\n";
    }
    if( range.min < range.max && range.max < n ) {
        range._args.log() << error << " must be smaller than "
                << (Number) range.max << ".\n";
    }
}

} // namespace cmdline

#endif // CMDLINE_ARGV_H
