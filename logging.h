//TODO: introduce normal logging

#define ASSERT( predicate, errorMsg )       \
    {                                       \
        if ( !predicate ) {                 \
			printf( errorMsg );				\
			printf( "\n" );					\
        }                                   \
    }		

#define VERIFY( predicate, errorMsg, operation )       \
    {                                       \
        if ( !predicate ) {                 \
			printf( errorMsg );				\
			printf( "\n" );					\
			operation;						\
        }                                   \
    };

#define LOG( msg ) {						\
	printf ( msg );							\
	printf ( "\n" );						\
}
