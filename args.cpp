/* Implementation of args.h
 */
#include <iostream>
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

std::string args::peek() const {
    return _args[_index];
}

void args::shift() {
    _index++;
}

std::string args::next() {
    std::string ret = peek();
    shift();
    return ret;
}

args args::subarg( std::size_t size ) {
    args ret;
    ret._args = std::vector<std::string>(
        _args.begin() + _index,
        _args.begin() + _index + size
    );
    _index += size;
    return ret;
}

args args::subcmd( std::size_t size ) {
    args ret;
    ret.program_name( next() );
    ret._args = std::vector<std::string>(
        _args.begin() + _index,
        _args.begin() + _index + size
    );
    _index += size;
    return ret;
}

void args::log( std::ostream & os ) {
    _log = &os;
}

void args::push_back( std::string str ) {
    _args.push_back( str );
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
