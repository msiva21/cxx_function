#include "cxx_function.hpp"
#include <type_traits>
#include <cassert>

using namespace cxx_function;

struct f {
    int operator () ( int ) & { return 0; }
};

struct d : f {
    operator int & () const { static int q; return q; }
};

#if __cpp_noexcept_function_type
struct ne {
    int operator () ( int ) noexcept { return 0; }
};
#endif

static_assert ( std::is_default_constructible< function< int( int ) > >::value, "" );
static_assert ( std::is_default_constructible< unique_function< int( int ) > >::value, "" );

static_assert ( std::is_convertible< f, function< int( int ) > >::value, "" );
static_assert ( std::is_convertible< f, function< int( std::integral_constant< long, 100 > ) > >::value, "" );
static_assert ( ! std::is_convertible< f, function< int( int ) && > >::value, "" );
static_assert ( ! std::is_convertible< f, function< int( int ) &, int( int ) && > >::value, "" );

#if __cpp_noexcept_function_type
static_assert ( ! std::is_default_constructible< function< int( int ) noexcept > >::value, "" );
static_assert ( ! std::is_default_constructible< unique_function< int( int ) noexcept > >::value, "" );

static_assert ( ! std::is_convertible< f, function< int( int ) noexcept > >::value, "" );
static_assert ( std::is_convertible< ne, function< int( int ) noexcept > >::value, "" );
#endif

static_assert ( std::is_convertible< function< d() >, function< f() > >::value, "" );
static_assert ( std::is_convertible< function< d &() >, function< f const &() > >::value, "" );
static_assert ( ! std::is_convertible< function< d() >, function< f const &() > >::value, "" );
static_assert ( std::is_convertible< function< d() >, function< int const &() > >::value, "" );
static_assert ( std::is_convertible< function< d &() >, function< int const &() > >::value, "" );

static_assert ( std::is_convertible< function< int() >, function< long() > >::value, "" );
static_assert ( std::is_convertible< function< int &() >, function< int const &() > >::value, "" );
static_assert ( ! std::is_convertible< function< int &() >, function< long const &() > >::value, "" );
static_assert ( std::is_convertible< function< int * const &() >, function< void *() > >::value, "" );
static_assert ( ! std::is_convertible< function< int * const &() >, function< void * const &() > >::value, "" );

static_assert ( std::is_nothrow_constructible< function< int( int ) >, function< int( int ) > >::value, "" );
static_assert ( std::is_nothrow_constructible< function< int( int ) >, function< int( int ) > && >::value, "" );
static_assert ( std::is_nothrow_constructible< function< int( int ) >, function< int( int ) & > >::value, "" );
static_assert ( std::is_nothrow_constructible< function< int( int ) >, function< int( int ) const & > >::value, "" );

static_assert ( ! std::is_constructible< function< int( int ) >, function< int( int ) >, std::allocator<void> >::value, "" );

static_assert ( std::is_nothrow_constructible< function< int( int ) >, std::allocator_arg_t, std::allocator<void>,
    in_place_t< std::nullptr_t >, std::nullptr_t >::value, "" );
static_assert ( ! std::is_nothrow_constructible< function< int( int ) >, std::allocator_arg_t, std::allocator<void>,
    in_place_t< function< int( int ) > >, f >::value, "" );

typedef unique_function< int( int ) & > uft;
static_assert ( std::is_assignable< uft, uft >::value, "" );
static_assert ( std::is_assignable< uft, uft && >::value, "" );
//static_assert ( ! std::is_assignable< uft, uft & >::value, "" ); -- Removed assignability SFINAE.
static_assert ( ! std::is_assignable< uft, uft const & >::value, "" );
//static_assert ( ! std::is_assignable< uft, uft const && >::value, "" );

typedef function< int( int ) & > ft;
static_assert ( std::is_assignable< ft, ft >::value, "" );
static_assert ( std::is_assignable< ft, ft && >::value, "" );
static_assert ( std::is_assignable< ft, ft & >::value, "" );
static_assert ( std::is_assignable< ft, ft const & >::value, "" );
static_assert ( std::is_assignable< ft, ft const && >::value, "" );

static_assert ( ! std::is_constructible< function< int( int ) & >, std::allocator_arg_t, std::allocator< uft >, uft >::value, "" );
static_assert ( ! std::is_constructible< function< int( int ) & >, std::allocator_arg_t, std::allocator< uft >, uft & >::value, "" );

static_assert ( ! std::is_constructible< function< int( int ) && >, f >::value, "" );

uft a, q( std::allocator_arg, std::allocator< uft >{}, std::move( a ) );

int main () {}
