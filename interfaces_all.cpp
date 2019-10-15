#include <stdio.h>
#include <stdlib.h>


#define N 1E8


#define TEST_FUNC(name) void name( void )
typedef void (* testFunc) ( void );


double _SUM;

// ------------ //
// No interface //
// ------------ //

namespace nointerface {

	struct Circle {
	    double radius;
	};


	inline
	double CircleArea( Circle c )
	{
	    return 3.14 * c.radius * c.radius;
	}

}


// test functions

TEST_FUNC(test_static_circle_stack)
{
	using namespace nointerface;

	Circle c;
    c.radius = 0.1;

    double sum = 0.0;

    for ( int i=0; i<N; ++i ) {
        sum += CircleArea( c );
        c.radius += 0.01;
    }

    _SUM = sum;
}


TEST_FUNC(test_static_circle_heap)
{
	using namespace nointerface;

	Circle *c = (Circle *) malloc( sizeof(*c) );
    c->radius = 0.1;

    double sum = 0.0;

    for ( int i=0; i<N; ++i ) {
        sum += CircleArea( *c );
        c->radius += 0.01;
    }

    _SUM = sum;
}



// // ---------------------- //
// // C++ standard interface //
// // ---------------------- //

namespace cppinterface {

	class Shape {

	public:

		virtual double area( void ) = 0;

	};


	class Circle : public Shape {

	public:

		Circle( double r ) : radius(r) {}


		virtual double area( void ) override
		{
			return 3.14 * radius * radius;
		}

		double radius;
	};


}


// test functions


TEST_FUNC(test_shape_cpp_stack)
{
	using namespace cppinterface;

	Circle c(0.1);

	double sum = 0.0;

	for ( int i=0; i<N; ++i ) {
		sum += c.area();
		c.radius += 0.01;
	}

	_SUM = sum;
}


TEST_FUNC(test_shape_cpp_heap)
{
	using namespace cppinterface;

	Circle *c = new Circle(0.1);

	double sum = 0.0;

	for ( int i=0; i<N; ++i ) {
		sum += c->area();
		c->radius += 0.01;
	}

	_SUM = sum;
}


// ----------------------------- //
// Manual V-Table Implementation //
// ----------------------------- //


namespace vtableinterface {

	// Base

	struct Shape;


	#define AREA_VFUNC(name) double name( void *obj )
	typedef double (* areaFunc) ( void *obj );


	struct Shape_vtbl {
	    areaFunc      areaF;
	};


	struct ShapeBase {
	    Shape_vtbl *vptr;
	};


	struct Shape {
	    ShapeBase  base;
	    void      *payload;
	};


	double ShapeArea( Shape s ) {
	    return s.base.vptr->areaF( s.payload );
	}


	// Circle

	struct Circle {
	    double radius;
	};


	AREA_VFUNC(circleArea)
	{
	    Circle *c = (Circle *) obj;
	    return 3.14 * c->radius * c->radius;
	}


	Shape_vtbl gCircle_vtbl = (Shape_vtbl) {
	    .areaF  = circleArea,
	};


	void CircleInit( Shape *s, Circle *c, double radius )
	{
	    c->radius    = radius;
	    s->base.vptr = &gCircle_vtbl;
	    s->payload   = (void *) c;
	}

}


// test functions

TEST_FUNC(test_shape_vtable_stack)
{
	using namespace vtableinterface;

    Shape circle;
    Circle c;
    CircleInit( &circle, &c, 0.1 );

    double sum = 0.0;

    for ( int i=0; i<N; ++i ) {
        sum += ShapeArea( circle );
        c.radius += 0.01;
    }

    _SUM = sum;
}


TEST_FUNC(test_shape_vtable_heap)
{
	using namespace vtableinterface;

    Shape *circle = (Shape *) malloc( sizeof(*circle) );
    Circle *c = (Circle *) malloc( sizeof(*c) );
    CircleInit( circle, c, 0.1 );

    double sum = 0.0;

    for ( int i=0; i<N; ++i ) {
        sum += ShapeArea( *circle );
        c->radius += 0.01;
    }

    _SUM = sum;
}


// ------------------------- //
// Interface without V-Table //
// ------------------------- //

namespace interface {

	#define AREA_VFUNC(name) double name( void *obj )
	typedef double (* areaFunc) ( void *obj );


	struct Shape {
	    areaFunc  area;
	    void      *payload;
	};


	double ShapeArea( Shape s ) {
	    return s.area( s.payload );
	}


	struct Circle {
	    double radius;
	};


	AREA_VFUNC(circleArea)
	{
	    Circle *c = (Circle *) obj;
	    return 3.14 * c->radius * c->radius;
	}


	void CircleInit( Shape *s, Circle *c, double radius )
	{
	    c->radius    = radius;
	    s->area      = circleArea;
	    s->payload   = (void *) c;
	}

}


// test functions

TEST_FUNC(test_interface_stack)
{
	using namespace interface;

	Shape circle;
    Circle c;
    CircleInit( &circle, &c, 0.1 );

    double sum = 0.0;

    for ( int i=0; i<N; ++i ) {
        sum += ShapeArea( circle );
        c.radius += 0.01;
    }

    _SUM = sum;
}


TEST_FUNC(test_interface_heap)
{
	using namespace interface;

	Shape *circle = (Shape *) malloc( sizeof(*circle) );
    Circle *c = (Circle *) malloc( sizeof(*c) );
    CircleInit( circle, c, 0.1 );

    double sum = 0.0;

    for ( int i=0; i<N; ++i ) {
        sum += ShapeArea( *circle );
        c->radius += 0.01;
    }

    _SUM = sum;
}


#include "utilities.h"



int main( int argn, const char *argv[] )
{
	{
		Test tests[] = {
			(Test){ test_static_circle_stack, "Static Circle on Stack" },
			(Test){ test_static_circle_heap, "Static Circle on Heap" },

			(Test){ test_shape_cpp_stack, "Cpp Interface on Stack" },
			(Test){ test_shape_cpp_heap, "Cpp Interface on Heap" },

			(Test){ test_shape_vtable_stack, "V-Table Interface on Stack" },
			(Test){ test_shape_vtable_heap, "V-Table Interface on Heap" },

			(Test){ test_interface_stack, "Interface on Stack" },
			(Test){ test_interface_heap, "Interface on Heap" },

		};

		RunTests( tests );
	}

	return 0;
}


