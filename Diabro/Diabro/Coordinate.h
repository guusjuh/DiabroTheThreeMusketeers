#ifndef COORDINATE_H_
#define COORDINATE_H_

struct RealCoordinate;

struct Coordinate {
	int x;
	int z;
	Coordinate(int x, int z) : x(x), z(z) {	}
	Coordinate() : x(0), z(0) {	}
	~Coordinate() { }
	Coordinate operator+(const Coordinate &other)
	{
		Coordinate n;
		n.x = x + other.x;
		n.z = z + other.z;
		return n;
	}
	void operator += (Coordinate &rhs);
	void operator -= (Coordinate &rhs);
	void operator = (RealCoordinate &rhs);
};
struct RealCoordinate {
	float rx;
	float rz;
	RealCoordinate(float x, float z) : rx(x), rz(z) {	}
	RealCoordinate() : rx(0), rz(0) {	}
	~RealCoordinate() { }
	void operator=(const Coordinate &value);
	void operator+= (Coordinate &rhs);
	void operator+= (RealCoordinate &rhs);
	void operator-= (RealCoordinate &rhs);
	void operator-= (Coordinate &rhs);
};
#endif