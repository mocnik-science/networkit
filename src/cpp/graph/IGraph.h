/*
 * IGraph.h
 *
 *  Created on: 10.05.2014
 *      Author: Klara Reichard (klara.reichard@gmail.com), Marvin Ritter (marvin.ritter@gmail.com)
 */

#ifndef IGRAPH_H
#define IGRAPH_H

#include <limits>
#include <string>
#include <algorithm>
#include <functional>
#include <stdexcept>

#include "../Globals.h"
#include "../viz/Point.h"

namespace NetworKit {

/** Typedefs **/

typedef std::function<void(node)> FNode;
typedef std::function<void(node, node)> FNodePair;
typedef std::function<void(node, edgeweight)> FNodeWeighted;

typedef std::function<void(node, node)> FEdge;
typedef std::function<void(node, node, double)> FEdgeWeighted;

typedef std::function<double(node)> FNodeSum;
typedef std::function<double(node, node, double)> FEdgeWeightedSum;

typedef std::function<bool()> FCondition;
typedef std::function<bool(node)> FNodeCondition;

/**
 * Interface for all graph classes. Every graph class has to implement all interface methods.
 */
class IGraph {

protected:
	count graphId;

public:

	IGraph() {
		static count nextGraphId = 1;
		graphId = nextGraphId++;
	}

	IGraph(const IGraph& other) = default;

	IGraph(IGraph&& other) = default;

	~IGraph() = default;

	IGraph& operator=(IGraph&& other) = default;

	IGraph& operator=(const IGraph& other) = default;

	/**
	 * Get the ID of this graph. The ID is a unique unsigned integer given to
	 * every graph on construction.
	 */
	count getId() const { return graphId; }

	/**
	 * Calculate an approximation of the memory used by this graph. Only memory increasing with the
	 * number of edges or nodes of this graph is taken into account. 
	 */
	virtual count getMemoryUsage() const = 0;

	/**
	 * Try to save some memory by shrinking internal data structures of the graph. Only run this
	 * once you finished editing the graph. Otherwise it will cause unnecessary reallocation of
	 * memory. 
	 */
	virtual void shrinkToFit() = 0;

	/**
	 * Set name of graph.
	 */
	virtual void setName(std::string name) = 0;

	/*
	 * @return name of graph
	 */
	virtual std::string getName() const = 0;

	/**
	 * Get string representation
	 */
	virtual std::string toString() const = 0;


	/** NODE MODIFIERS **/

	/**
	 * Add a new node to the graph and return it.
	 */
	virtual node addNode() = 0;

	/**
	 * Add a new node to the graph with coordinates @a x and @y and return it.
	 */
	virtual node addNode(float x, float y) = 0;

	/**
	 * Remove an isolated node v from the graph.
	 *
	 * Although it would be convenient to remove all incident edges at the same time,
	 * this causes complications for dynamic applications. Therefore, removeNode is an
	 * atomic event. All incident edges need to be removed first and an exception is thrown
	 * otherwise.
	 */
	virtual void removeNode(node v) = 0;

	/**
	 * Check if node v exists in the graph.
	 */
	virtual bool hasNode(node v) const = 0;


	/** NODE PROPERTIES **/

	/**
	 * Return the number of neighbors for node v.
	 */
	virtual count degree(node v) const = 0;

	/**
	 * @return true if the node is isolated (= degree is 0)
	 */
	virtual bool isIsolated(node v) const = 0;

	/**
	 * @return Weighted degree of @a v. For directed graphs this is the sum of weights off all outgoing edges fo @a v.
	 */
	virtual edgeweight weightedDegree(node v) const = 0;

	/**
	 * @return random node of the graph
	 */
	virtual node randomNode() const = 0;


	/** EDGE MODIFIERS **/

	/**
	 * Insert an directed edge between from @a u to @a v.
	 */
	virtual void addEdge(node u, node v, edgeweight weight) = 0;

	/**
	 * Remove directed edge between from @a u to @a v.
	 */
	virtual void removeEdge(node u, node v) = 0;

	/**
	 * Check if directed edge {u,v} exists.
	 *
	 */
	virtual bool hasEdge(node u, node v) const = 0;

	/**
	 * Merges edge {u,v} to become a supernode. Edges to u and v are
	 * rewired, multiple edges merged and their weights added.
	 * The vertex weights of @a u and @a v are added.
	 * A self-loop is only created if @a discardSelfLoop is set to false.
	 *
	 * @return New node that has been created if u != v. Otherwise none.
	 */
	// node mergeEdge(node u, node v, bool discardSelfLoop = true);


	/** GLOBAL PROPERTIES **/

	/**
	 * Return true if this graph supports edge weights other than 1.0
	 */
	virtual bool isWeighted() const = 0;

	/** 
	 * Return true if this graph supports directed edges.
	 */
	virtual bool isDirected() const = 0;

	/**
	 * Return true if graph contains no nodes.
	 */
	virtual bool isEmpty() const = 0;

	/**
	 * Return the number of nodes in the graph.
	 *
	 */
	virtual count numberOfNodes() const = 0;

	/**
	 * Return the number of edges in the graph.
	 *
	 *	 */
	virtual count numberOfEdges() const = 0;

	/**
	 * @return the number of loops {v, v} in the graph.
	 *
	 * This involves calculation, so store result if needed multiple times.
	 */
	virtual count numberOfSelfLoops() const = 0;

 	/**
	 * Get an upper bound for the node ids in the graph.
	 */
	virtual index upperNodeIdBound() const = 0;

	/** DYNAMICS **/

	/**
	 * Trigger a time step - increments counter.
	 */
	virtual void timeStep() = 0;

	/**
	 * Get time step counter.
	 */
	virtual count time() = 0;


	/** COORDINATES **/

	virtual void setCoordinate(node v, Point<float> value) = 0;

	virtual Point<float>& getCoordinate(node v) = 0;

	virtual float minCoordinate(count dim) = 0;

	virtual float maxCoordinate(count dim) = 0;

	virtual void initCoordinates() = 0;


	/** EDGE ATTRIBUTES **/

	/**
	 * Return edge weight.
	 *
	 * Return 0 if edge does not exist.
	 */
	virtual edgeweight weight(node u, node v) const = 0;

	/**
	 * Set the weight of an edge. If the edge does not exist,
	 * it will be inserted.
	 *
	 * @param[in]	u	endpoint of edge
	 * @param[in]	v	endpoint of edge
	 * @param[in]	weight	edge weight
	 */
	virtual void setWeight(node u, node v, edgeweight w) = 0;

	/**
	 * Increase the weight of an edge. If the edge does not exist,
	 * it will be inserted.
	 *
	 * @param[in]	u	endpoint of edge
	 * @param[in]	v	endpoint of edge
	 * @param[in]	weight	edge weight
	 */
	virtual void increaseWeight(node u, node v, edgeweight w) = 0;

	/**
	 * Add new edge map for an attribute of type double.
	 */
	virtual int addEdgeAttribute_double(double defaultValue) = 0;

	/**
	 * @return attribute of type double for an edge.
	 *
	 * @param[in]	u	node
	 * @param[in]	v	node
	 * @param[in]	attrId	attribute id
	 */
	virtual double attribute_double(node u, node v, int attrId) const = 0;

	/**
	 * Set edge attribute of type double If the edge does not exist,
	 * it will be inserted.
	 *
	 * @param[in]	u	endpoint of edge
	 * @param[in]	v	endpoint of edge
	 * @param[in]	attr	double edge attribute
	 */
	virtual void setAttribute_double(node u, node v, int attrId, double attr) = 0;


	/** SUMS **/

	/**
	 * @return sum of all edge weights
	 */
	virtual edgeweight totalEdgeWeight() const = 0;


	/** Collections **/

	/**
	 * Return list of nodes
	 */
	virtual std::vector<node> nodes() const = 0;

	/**
	 * Return list of edges as node pairs.
	 */
	virtual std::vector<std::pair<node, node> > edges() const = 0;


	/**
	 * Return list of neighbors for given node.
	 */
	virtual std::vector<node> neighbors(node u) const = 0;


	/** NODE ITERATORS **/

	/**
	 * Iterate over all nodes of the graph and call f (lambda closure).
	 */
	virtual void forNodes(FNode f) const = 0;

	/**
	 * Iterate in parallel over all nodes of the graph and call f (lambda closure).
	 */
	virtual void parallelForNodes(FNode f) const = 0;

	/**
	 * Iterate over all nodes of the graph and call f (lambda closure) as long as the condition remains true.
	 * This allows for breaking from a node loop.
	 */
	virtual void forNodesWhile(FCondition condition, FNode f) const = 0;

	/**
	 * Iterate over all nodes of the graph and call f (lambda closure) as long as the condition remains true.
	 * This allows for breaking from a node loop.
	 */
	virtual void forNodes(FNodeCondition condition, FNode f) const = 0;

	/**
	 * Iterate randomly over all nodes of the graph and call f (lambda closure).
	 */
	virtual void forNodesInRandomOrder(FNode f) const = 0;

	/**
	 * Iterate in parallel over all nodes of the graph and call f (lambda closure).
	 * Using schedule(guided) to remedy load-imbalances due to e.g. unequal degree distribution.
	 */
	virtual void balancedParallelForNodes(FNode f) const = 0;

	/**
	 * Iterate over all undirected pairs of nodesand call f (lambda closure).
	 */
	virtual void forNodePairs(FNodePair f) const = 0;

	/**
	 * Iterate over all undirected pairs of nodes in parallel and call f (lambda closure).
	 */
	virtual void parallelForNodePairs(FNodePair f) const = 0;


 	/** REDUCTION ITERATORS **/

	/**
	 * Iterate in parallel over all nodes and sum (reduce +) the values returned by the f
	 */
	virtual double parallelSumForNodes(FNodeSum f) const { return 0.0; };


	/** EDGE ITERATORS **/

	/**
	 * Iterate over all edges of the graph and call f (lambda closure).
	 */
	// virtual void forEdges(L f) const = 0;
	virtual void forEdges(FEdge f) const = 0;

	/**
	 * Iterate in parallel over all edges of the graph and call f (lambda closure).
	 */
	virtual void parallelForEdges(FEdge f) const = 0;

	/**
	 * Iterate over all edges of the graph and call f (lambda closure).
	 *
	 * f takes arguments (u, v, w) where u and v are the nodes of the edge and w is its weight.
	 */
	virtual void forWeightedEdges(FEdgeWeighted f) const = 0;

	/**
	 * Iterate over all edges of the graph and call f (lambda closure).
	 *
	 * f takes arguments (u, v, w) where u and v are the nodes of the edge and w is its weight.
	 */
	virtual void parallelForWeightedEdges(FEdgeWeighted f) const = 0;

	/**
	 * Iterate over all edges of the const graph and call f (lambda closure).
	 *
	 *	@param[in]	attrId		attribute id
	 *	@param[in]	f 		takes arguments (u, v, a) where a is an edge attribute of edge {u, v}
	 *
	 */
	virtual void forEdgesWithAttribute_double(int attrId, FEdgeWeighted f) const = 0;


	/** NEIGHBORHOOD ITERATORS **/

	/**
	 * Iterate over all neighbors of a node and call f (lamdba closure).
	 */
	virtual void forNeighborsOf(node u, FNode f) const = 0;

	/**
	 * Iterate over all edge weights of a node and call f (lamdba closure).
	 */
	virtual void forWeightedNeighborsOf(node u, FNodeWeighted f) const = 0;

	/**
	 * Iterate over all incident edges of a node and call f (lamdba closure).
	 */
	virtual void forEdgesOf(node u, FEdge f) const = 0;

	/**
	 * Iterate over all incident edges of a node and call f (lamdba closure).
	 *
	 * f takes parameters (u, v, w) where w is the edge weight.
	 *
	 */
	virtual void forWeightedEdgesOf(node u, FEdgeWeighted f) const = 0;


	/** REDUCTION ITERATORS **/

	/**
	 * Iterate in parallel over all edges and sum (reduce +) the values returned by the f
	 */
	virtual double parallelSumForWeightedEdges(FEdgeWeightedSum f) const = 0;


	/** GRAPH SEARCHES **/

	virtual void BFSfrom(node r, FNode f) const = 0;

	virtual void BFSEdgesfrom(node r, FEdge f) const = 0;

	virtual void DFSfrom(node r, FNode f) const = 0;
	
	virtual void DFSEdgesfrom(node r, FEdge f) const = 0;

};

} /* namespace NetworKit */

#endif /* IGRAPH_H */