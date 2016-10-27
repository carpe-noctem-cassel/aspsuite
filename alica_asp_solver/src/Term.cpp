/*
 * Term.cpp
 *
 *  Created on: Jul 13, 2016
 *      Author: Stefan Jakob
 */

#include <alica_asp_solver/Term.h>
#include <SystemConfig.h>

namespace alica
{
	namespace reasoner
	{

		Term::Term(int lifeTime)
		{
			this->backgroundFile = "";
			this->rule = "";
			this->head = "";
			this->body = "";
			this->lifeTime = lifeTime;
			this->externals = nullptr;
			this->numberOfModels = "";
		}

		Term::~Term()
		{
		}

		bool Term::setRule(string rule)
		{
			if (this->rule.compare("") != 0)
			{
				return false;
			}
			this->rule = rule;
			if (rule.find(":-") != string::npos)
			{
				size_t endOfHead = rule.find(":-");
				size_t startOfBody = endOfHead + 2;
				this->head = rule.substr(0, endOfHead);
				this->body = rule.substr(startOfBody, rule.size() - startOfBody - 1);
				this->head = supplementary::Configuration::trim(this->head);
				this->body = supplementary::Configuration::trim(this->body);
			}
			return true;
		}

		void Term::addFact(string fact)
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

		string Term::getRuleHead()
		{
			return this->head;
		}

		string Term::getRuleBody()
		{
			return this->body;
		}

		// TODO umbenennen in program section?
		string Term::getBackGroundFileName()
		{
			return this->backgroundFile;
		}

		void Term::setBackgroundFileName(string backgroundFile)
		{
			this->backgroundFile = backgroundFile;
		}

		int Term::getLifeTime()
		{
			return this->lifeTime;
		}

		string Term::getRule()
		{
			return this->rule;
		}

		vector<string> Term::getFacts()
		{
			return this->facts;
		}

		void Term::setExternals(shared_ptr<map<string, bool>> externals)
		{
			this->externals = externals;
		}

		shared_ptr<map<string, bool> > Term::getExternals()
		{
			return this->externals;
		}

		string Term::getNumberOfModels()
		{
			return this->numberOfModels;
		}

		void Term::setNumberOfModels(string numberOfModels)
		{
			this->numberOfModels = numberOfModels;
		}

	} /* namespace reasoner */
} /* namespace alica */

