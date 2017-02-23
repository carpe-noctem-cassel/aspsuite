/*
 * ConceptNetEdge.cpp
 *
 *  Created on: Feb 23, 2017
 *      Author: stefan
 */

#include "containers/ConceptNetEdge.h"

#include <sstream>
#include <string>

namespace cng
{

	ConceptNetEdge::ConceptNetEdge(std::string id, std::string language, std::string firstConcept, std::string secondConcept, std::string relation, double weight)
	{
		this->id = id;
		this->language = language;
		this->firstConcept = firstConcept;
		this->secondConcept = secondConcept;
		this->weight = weight;
		this->relation = relation;
	}

	ConceptNetEdge::~ConceptNetEdge()
	{
	}

	std::string ConceptNetEdge::toString()
	{
		std::stringstream ss;
		ss << "Edge: " << firstConcept << " " << relation << " " << secondConcept << " " << weight << std::endl;
		return ss.str();
	}

} /* namespace cng */
