/* Implementation of args.h
 */
#include <iostream>
#include <stdexcept>
#include "args.h"

namespace cmdline {

args::args( int argc, char const * const * argv ) {
    _program_name = argv[0];
    _args.reserve( argc - 1 );
    for( int i = 1; i < argc; i++ )
        _args.push_back( argv[i] );
    _index = 0;

    _log = &std::cerr;
}

args::args() {
    _index = 0;
    _log = &std::cerr;
}

std::size_t args::size() const {
    return _args.size() - _index;
}

std::size_t args::total_size() const {
    return _args.size();
}

std::string args::peek() const {
    if( _index >= _args.size() )
        throw std::out_of_range( "No argument left to peek." );

    return _args[_index];
}

std::string args::peek( int index ) const {
    if( _index + index >= _args.size() )
        throw std::out_of_range( "Argument vector too short." );
    if( _index + index < 0 )
        throw std::out_of_range( "The index must not become negative." );

    return _args[_index + index];
}

void args::shift() {
    if( _index >= _args.size() )
        throw std::out_of_range( "No arguments left to shift." );

    _index++;
}

std::string args::next() {
    /* Peek takes care of throwing for us. */
    std::string ret = peek();
    shift();
    return ret;
}

void args::push_back( std::string str ) {
    _args.push_back( str );
}

range_parser args::range( double min ) {
    return range_parser( *this, min );
}

range_parser args::range( double min, double max ) {
    return range_parser( *this, min, max );
}

args args::subarg( std::size_t size ) {
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

args args::subarg_until( bool (* predicate )(const std::string&) ) {
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

args args::subcmd( std::size_t size ) {
    std::string name = next();
    args ret = subarg( size );
    ret.program_name( name );
    return ret;
}

args args::subcmd_until( bool (* predicate )(const std::string&) ) {
    std::string name = next();
    args ret = subarg_until( predicate );
    ret.program_name( name );
    return ret;
}

void args::log( std::ostream & os ) {
    _log = &os;
}

std::ostream & args::log() {
    return *_log;
}

void args::program_name( const std::string & name ) {
    _program_name = name;
}

const std::string & args::program_name() const {
    return _program_name;
}

} // namespace cmdline
