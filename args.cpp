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

    _log = &std::clog;
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

/* Sets/retrieves the log stream. */
void args::log( std::ostream & os ) {
    _log = &os;
}

std::ostream & args::log() {
    return *_log;
}

} // namespace cmdline