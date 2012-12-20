/*
 * ClusterContracter.cpp
 *
 *  Created on: 30.10.2012
 *      Author: cls
 */

#include "ClusterContracter.h"

namespace EnsembleClustering {

ClusterContracter::ClusterContracter() {
	// TODO Auto-generated constructor stub

}

ClusterContracter::~ClusterContracter() {
	// TODO Auto-generated destructor stub
}

Graph& ClusterContracter::run(Graph& G, Clustering& zeta) {

	Graph* Gcon = new Graph();

	IndexMap<cluster, node> clusterToSuperNode(zeta.upperBound()); // there is one supernode for each cluster

	// populate map cluster -> supernode
	G.forallNodes([&](node v){
		cluster c = zeta.clusterOf(v);
		if (! clusterToSuperNode.contains(c)) {
			node sv = G.addNode(); // TOOD: probably does not scale well, think about allocating ranges of nodes
			clusterToSuperNode[c] = sv;
		}
	});

	// find supernode for node with this function
	auto getSuperNode = [&](node v) {
		cluster cv = zeta[v];
		assert (cv <= zeta.upperBound());
		node sv = clusterToSuperNode[cv];
		return sv;
	};

	// iterate over edges of G and create edges in Gcon or update edge and node weights in Gcon
	G.forallEdges([&](node u, node v) {
		node su = getSuperNode(u);
		node sv = getSuperNode(v);
		if (zeta.clusterOf(u) == zeta.clusterOf(v)) {
			// add edge weight to supernode (self-loop) weight
			Gcon->setWeight(su, Gcon->weight(su) + G.weight(u, v));
		} else {
			// add edge weight to weight between two supernodes
			if (Gcon->hasEdge(u, v)) {
				Gcon->setWeight(su, sv, Gcon->weight(su, sv) + G.weight(u, v));
			} else {
				// create new edge
				Gcon->insertEdge(su, sv, G.weight(u, v));
			}

		}
	});

	return (*Gcon);
}

}
