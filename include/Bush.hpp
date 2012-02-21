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


#ifndef BUSH_HPP
#define BUSH_HPP

#include "GraphEdge.hpp"
#include "Origin.hpp"
#include "BushNode.hpp"
#include "ABGraph.hpp"
#include "EdgeVector.hpp"

#include <vector>
#include <utility>
#include <iostream>

class Bush
{
	public:
		Bush(const Origin&, ABGraph&, std::vector<unsigned>&, std::vector<unsigned>&);//Inits bush, sends initial flows
		bool fix(double);
		void printCrap();
		int getOrigin() { return origin.getOrigin(); }
		long giveCount()/* const*/;
		double allOrNothingCost();
		double maxDifference();
		~Bush();
		void clearChanges() {
			additions.clear();
			deletions.clear();
			tempStore.clear();
		}
		bool anyChanges() {
			return (additions.size()+deletions.size())>0;
		}
	private:
		bool updateEdges();
		bool equilibriateFlows(double);//Equilibriates, tells graph what's going on
		void updateEdges(std::vector<BushEdge>::iterator&, std::vector<BushEdge>::iterator, double, unsigned);
		void buildTrees();
		void sendInitialFlows();
		//Makes sure all our edges are pointing in the right direction, and we're sorted well.
		void setUpGraph();
		void topologicalSort();
		void applyBushEdgeChanges();
		void partialTS(unsigned, unsigned);
		
		const Origin& origin;
		std::vector<unsigned> edges;//Stores offsets into edge storage in TO.
		std::vector<BushEdge> edgeStorage;//Stores BushEdges in contiguous memory (in TO)
		
		std::vector<unsigned> topologicalOrdering;
		
		//Shared with other bushes so we don't deallocate/reallocate data uselessly between bush iterations
		std::vector<BushNode>& sharedNodes;
		std::vector<unsigned>& tempStore;//Used in topo sort, don't want to waste the alloc/dealloc time.
		std::vector<unsigned> &reverseTS;
		
		ABGraph& graph;
		
		std::vector<std::pair<unsigned, BackwardGraphEdge*> > additions;//Used in updates. [to, edge]
			//could sort on to-node?
		std::vector<std::pair<unsigned, BushEdge*> > deletions;//[to-node, edge]
};

class NodeIndexComparator
{
public:
	NodeIndexComparator(std::vector<BushNode> &sharedNodes) : sharedNodes(sharedNodes) {}
	bool operator()(unsigned first, unsigned second) {
		return sharedNodes[first].maxDist() < sharedNodes[second].maxDist();
	}
private:
	std::vector<BushNode> &sharedNodes;
};

//Inlined because we call this once per node per iteration, and spend 35% of our time in here. FIXME
inline void Bush::updateEdges(std::vector<BushEdge>::iterator &from, std::vector<BushEdge>::iterator end, double maxDist, unsigned id)
{
	for(; from < end; ++from) {
		if(from->fromNode()->maxDist() > maxDist) {
			deletions.push_back(std::make_pair(
				id,
				&*from
			));
			additions.push_back(std::make_pair(
				from->fromNode()-&sharedNodes[0],
				graph.forward(from->underlyingEdge())->getInverse()
			));
		}
	}
}

#endif
