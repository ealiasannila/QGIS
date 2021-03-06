/*
 * Funnel.cpp
 *
 *  Created on: Aug 16, 2016
 *      Author: elias
 */

#include "Funnel.h"
#include <sstream>
#include <iostream>

Funnel::Funnel(const Coords* l, const Coords* a, const Coords* r) {
	lc.push_back(a);
	lc.push_back(l);
	rc.push_back(a);
	rc.push_back(r);
}
Funnel::Funnel(std::deque<const Coords*> lc, std::deque<const Coords*> rc) {
	this->lc = lc;
	this->rc = rc;
}

//TODO varaudu jos deque on tyhjä tai jos törmää apexiin?
std::pair<const Coords*, const Coords*> Funnel::getBase() {
	return std::pair<const Coords*,const Coords* >(lc.back(), rc.back());
}
std::deque<const Coords*> Funnel::getLC() {
	return lc;
}
std::deque<const Coords*> Funnel::getRC() {
	return rc;
}

std::string Funnel::toString() {
	std::stringstream sstm;
	sstm << "\nLC: ";
	for (const Coords* c : lc) {
		sstm << '\n' << c->toString();
	}
	sstm << "\nRC: ";
	for (const Coords* c : rc) {
		sstm << '\n' << c->toString();
	}

	return sstm.str();

}

/*
 * o is inside first segments of funnel, in which case funnel is split
 * so that left and right chains form two new funnels with the other chain
 * formed by apex and c.
 */
Funnel Funnel::split(const Coords* o) {
	std::deque<const Coords*> newlc = lc;
	std::deque<const Coords*> newrc;

	lc.clear();
	lc.push_back(rc.front());
	lc.push_back(o);

	newrc.push_back(newlc.front());
	newrc.push_back(o);
	return Funnel(newlc, newrc);
}
/*
 * o is not inside the sector formed by fisrt segments of chains, but it is inside the funnel. In this case the funnel is shrunk
 * so that o forms the endpoint of the chain being shrunk.
 */
void Funnel::shrink(const Coords* o, std::deque<const Coords*>* chain, unsigned lastRemaining) {
	while (chain->size() - 1 > lastRemaining) {
		chain->pop_back();
	}
	chain->push_back(o);
}
/*
 * locates the last endpoint of an edge in chain, from which's startpoint o can't be seen. (returns index of endpoint of that segment)
 */
int Funnel::findInChain(const Coords* o, std::deque<const Coords*> chain, int side) {
	for (unsigned i = 1; i < chain.size(); i++) {
		if (o->isRight(chain.at(i - 1), chain.at(i)) == side) {
			return i - 1;
		}
	}
	return -1;
}

/*tests if o is inside first sector:
 * 0 -> in
 * -1 -> left
 * 1 -> right
 */
int Funnel::inFirstSector(const Coords* o) {
	int lo = o->isRight(lc.at(0), lc.at(1));
	int ro = o->isRight(rc.at(0), rc.at(1));

	if (lo != 1) {
		return -1;
	}
	if (ro != -1) {
		return 1;
	}
	return 0;
}

/*
 * Returns location of opposite node from the base of the funnel:
 * Does one of the following depending on the location of opposing vertex o:
 *  1. splits funnel
 *  2. shrinks either chain
 *  3. expands either chain
 */
void Funnel::reactToOpposite(const Coords* o, std::deque<Funnel>* funnelQueue, nSet* neighbours, int polygon) {
	switch (this->inFirstSector(o)) {
	int lastRemaining;
	case 0:
		funnelQueue->push_back(this->split(o));
		neighbours->insert(std::pair<const Coords*, int>(o, polygon));

		//TODO Add the split funnels to handling queue - Ehkä pakita siihen että palauttaa numerokoodin ja kutsuja tekee sen perusteella juttuja
		// toinen vaihtis olis että palauttaa aina listan funneleita, eli joko vain itsensä, tai sitten 2 uutta. Tai pointteri quehen...
		break;
	case -1:
		lastRemaining = this->findInChain(o, lc, 1);
		if (lastRemaining == -1) {
			lc.push_back(o);
		} else {
			this->shrink(o, &lc, lastRemaining);
		}
		break;
	case 1:
		lastRemaining = this->findInChain(o, rc, -1);
		if (lastRemaining == -1) {
			rc.push_back(o);
		} else {
			this->shrink(o, &rc, lastRemaining);
		}
		break;
	}
}
Funnel::~Funnel() {
// TODO Auto-generated destructor stub
}

