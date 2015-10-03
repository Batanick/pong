#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

//TODO: introduce normal logging
#define ASSERT(predicate, errorMsg)       \
    {                                       \
    if ( !predicate ) {                 \
    printf( errorMsg );                \
    printf( "\n" );                    \
    }                                   \
    }

#define VERIFY(predicate, errorMsg, operation)       \
    {                                       \
    if ( !predicate ) {                 \
    printf( "%s\n", errorMsg );                \
    operation;                        \
    }                                   \
    };

#define LOG(msg, ...) {                        \
  printf ( msg, ##__VA_ARGS__ );                            \
  printf ( "\n" );                        \
}

#endif