/*
 * Term.cpp
 *
 *  Created on: Jul 13, 2016
 *      Author: Stefan Jakob
 */

#include <ASPCommonsTerm.h>
#include <SystemConfig.h>

namespace reasoner
{

	ASPCommonsTerm::ASPCommonsTerm(int lifeTime)
	{
		this->programSection = "";
		this->id = -1;
		this->lifeTime = lifeTime;
		this->externals = nullptr;
		this->numberOfModels = "";
		this->type = ASPQueryType::Undefined;
		this->queryId = -1;
	}

	ASPCommonsTerm::~ASPCommonsTerm()
	{
	}

	void ASPCommonsTerm::addRule(string rule)
	{
		this->rules.push_back(rule);
	}

	void ASPCommonsTerm::addFact(string fact)
	{
		if (fact.find(".") == string::npos)
		{
			this->facts.push_back(fact + ".");
		}
		else
		{
			this->facts.push_back(fact);
		}
	}

	vector<string> ASPCommonsTerm::getRuleHeads()
	{
		return this->heads;
	}

	vector<string> ASPCommonsTerm::getRuleBodies()
	{
		return this->bodies;
	}

	string ASPCommonsTerm::getProgramSection()
	{
		return this->programSection;
	}

	void ASPCommonsTerm::setProgramSection(string programSection)
	{
		this->programSection = programSection;
	}

	int ASPCommonsTerm::getLifeTime()
	{
		return this->lifeTime;
	}

	void ASPCommonsTerm::setLifeTime(int lifeTime)
	{
		this->lifeTime = lifeTime;
	}


	vector<string> ASPCommonsTerm::getRules()
	{
		return this->rules;
	}

	vector<string> ASPCommonsTerm::getFacts()
	{
		return this->facts;
	}

	void ASPCommonsTerm::setExternals(shared_ptr<map<string, bool>> externals)
	{
		this->externals = externals;
	}

	shared_ptr<map<string, bool> > ASPCommonsTerm::getExternals()
	{
		return this->externals;
	}

	string ASPCommonsTerm::getNumberOfModels()
	{
		return this->numberOfModels;
	}

	void ASPCommonsTerm::setNumberOfModels(string numberOfModels)
	{
		this->numberOfModels = numberOfModels;
	}

	ASPQueryType ASPCommonsTerm::getType()
	{
		return this->type;
	}

	void ASPCommonsTerm::setType(ASPQueryType type)
	{
		this->type = type;
	}

	long ASPCommonsTerm::getId()
	{
		return id;
	}

	void ASPCommonsTerm::setId(long id)
	{
		this->id = id;
	}
	/**
	 * The query id has to be added to any predicate which is added to the program, naming rule
	 * heads and facts!
	 * An unique id is given by the ASPSolver!
	 */
	int ASPCommonsTerm::getQueryId()
	{
		return queryId;
	}

	/**
	 * The query id has to be added to any predicate which is added to the program, naming rule
	 * heads and facts!
	 * An unique id is given by the ASPSolver!
	 */
	void ASPCommonsTerm::setQueryId(int queryId)
	{
		this->queryId = queryId;
	}

	string ASPCommonsTerm::getQueryRule()
	{
		return queryRule;
	}

	void ASPCommonsTerm::setQueryRule(string queryRule)
	{
		if (queryRule.empty())
		{
			return;
		}

		queryRule = supplementary::Configuration::trim(queryRule);
		size_t endOfHead = queryRule.find(":-");
		if (endOfHead != string::npos)
		{
			// for rules (including variables)
			size_t startOfBody = endOfHead + 2;
			this->heads.push_back(supplementary::Configuration::trim(queryRule.substr(0, endOfHead)));
			this->bodies.push_back(
					supplementary::Configuration::trim(queryRule.substr(startOfBody, queryRule.size() - startOfBody - 1)));
		}
		else
		{
			// for ground literals
			this->heads.push_back(queryRule);
		}
		this->queryRule = queryRule;
	}

} /* namespace reasoner */

