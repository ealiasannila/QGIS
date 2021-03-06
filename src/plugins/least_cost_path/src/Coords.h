/*
 * const Coords.h
 *
 *  Created on: Aug 16, 2016
 *      Author: elias
 */

#ifndef  Coords_H_
#define  Coords_H_
#include <string>
#include <set>
#include <map>
#include <cmath>
#include <limits>
#include "defs.h"
class Coords {
private:

	//change map to two sorted vectors (ints and coords) and binary search. (few insertions many lookups)
	mutable double toStart;
	mutable const Coords* predecessor;
	mutable allNContainer leftNeighbours;
	mutable allNContainer rightNeighbours;

	double x;
	double y;

public:
	Coords();
	Coords(double x, double y);
	Coords(double x, double y, int polygon);
	double getX() const {
		return x;
	}
	double getToStart() const {
		return toStart;
	}
	const Coords* getPred() const {
		return predecessor;
	}
	void setToStart(double cost) const;
	void setPred(const Coords* pred) const;
	double getY() const {
		return y;
	}
	int isRight(const Coords* c1, const Coords* c2) const;
	nContainer getRightNeighbours(int polygon) const;
	nContainer getLeftNeighbours(int polygon) const;
	allNeighIter getAllLeftN() const;
	allNeighIter getAllRightN() const;
	void addToPolygon(int polygon) const;
	void addNeighbours(const Coords* l, const Coords* r, int polygon) const;
	std::string toString() const;
	virtual ~ Coords();

	bool operator==(const Coords& c) const;
	bool operator!=(const Coords& c) const;
};

#endif /* const Coords_H_ */
