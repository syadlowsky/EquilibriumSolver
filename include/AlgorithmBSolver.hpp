/*
    Copyright 2008, 2009 Matthew Steel.

    This file is part of EF.

    EF is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    EF is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with EF.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef ALGORITHM_B_SOLVER_HPP
#define ALGORITHM_B_SOLVER_HPP

#include <list>
#include <limits>
#include <ostream>

#include "GraphEdge.hpp"
#include "Origin.hpp"
#include "Bush.hpp"
#include "ABGraph.hpp"
#include "InputGraph.hpp"

/**
 * Solver for the Traffic Assignment Problem using an algorithm like (but not
 * the same as) Robert Dial's Algorithm B. Probably needs renaming? Either
 * way, it's really swell.
 */
class AlgorithmBSolver
{
	
	public:
		/**
		 * TODO
		 */
		AlgorithmBSolver(const InputGraph& g);
		
		/**
		 * TODO
		 */
		void solve(unsigned iterationLimit = std::numeric_limits<unsigned>::max());

		bool fixBushSets(std::list<Bush*>& fix, std::list<Bush*>& output, double average, bool whetherMove);
		
//		/**
//		 * TODO
//		 */
//		void outputAnswer(boost::shared_ptr<InputGraph>) const;

		void printBushes() {
			for(std::list<Bush*>::const_iterator i = bushes.begin(); i != bushes.end(); ++i) {
				std::cout << "-  - - - -- -  - - - - - - - - - -- " << std::endl;
				(*i)->printCrap();
			}
			for(std::list<Bush*>::const_iterator i = lazyBushes.begin(); i != lazyBushes.end(); ++i) {
				std::cout << "-  - - - -- -  - - - - - - - - - -- " << std::endl;
				(*i)->printCrap();
			}
		}
		//Ugh, remnants of old debugging?
		//Doesn't print anything good most of the time now, considering node/edge data is all shared now across bushes.
		
		friend std::ostream& operator<<(std::ostream& o, AlgorithmBSolver& e)
		{
			std::vector<GraphEdge>::iterator begin=e.graph.begin(), end=e.graph.end(); //Different class name?
			for(std::vector<GraphEdge>::iterator i = begin; i != end; ++i)
				o << "Edge (" << i->fromNode()->getId() << ',' << i->toNode()->getId() << "), " << *i << '\n';
			return o;
		}//FIXME: Make this do something more useful? Format it nicely?

		/**
		 * Gets us a count of the total number of bush-specific edges
		 * with positive flows. For curiosity. (It's usually about a
		 * fifth of all bush-specific edges. Could get big space
		 * savings at the expense of some time if we'd like.)
		 */
		int getCount() const;
		double relativeGap();
		void wasteTime() const;
		~AlgorithmBSolver();
		//NOTE: Have we remembered to clean up nonexistant edges?
	private:
		//Edge data:
		ABGraph graph;
		
		//Solver origin-specific data:
		std::list<Bush*> bushes;
		std::list<Bush*> lazyBushes;
		std::list<Origin> ODData;
		
		//So we don't have to allocate in topological sorts? Really?
		//A premature optimisation, but probably an optimisation.
		//So long as we clean it up, I guess...
		std::vector<std::pair<double, unsigned> > tempStore;
};
#endif
