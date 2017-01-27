/*
 * AspQuery.h
 *
 *  Created on: Jun 8, 2016
 *      Author: Stefan Jakob
 */

#ifndef SYMROCK_ALICA_ASP_SOLVER_INCLUDE_ALICA_ASP_SOLVER_ASPQUERY_H_
#define SYMROCK_ALICA_ASP_SOLVER_INCLUDE_ALICA_ASP_SOLVER_ASPQUERY_H_

#include <string>
#include <clingo/clingocontrol.hh>

#include "../include/asp_commons/ASPQueryType.h"

using namespace std;

//#define ASPQUERY_DEBUG

namespace reasoner
{

	class ASPCommonsTerm;
	class AnnotatedValVec;
	class ASPSolver;
	class ASPQuery
	{
	public:
		ASPQuery(ASPSolver* solver, shared_ptr<reasoner::ASPCommonsTerm> term);
		virtual ~ASPQuery();

		shared_ptr<vector<Gringo::ValVec>> getCurrentModels();

		int getLifeTime();
		void setLifeTime(int lifeTime);
		void reduceLifeTime();

		void onModel(ClingoModel& clingoModel);

		map<Gringo::Value, vector<Gringo::Value>> getHeadValues();

		ASPSolver* getSolver();
		vector<string> getRules();
		shared_ptr<reasoner::ASPCommonsTerm> getTerm();

		string getProgrammSection();
		void setProgrammSection(string programmSection);
		virtual void removeExternal() = 0;

		string toString();
		ASPQueryType getType();

	protected:
		/**
		 * queryString is used to ask the solver if specific predicates are true.
		 * predicates are separated by "," meaning all of them will be in the same rule and ";"
		 * meaning that there is a rule for every predicate
		 */
		ASPSolver* solver;
		shared_ptr<vector<Gringo::ValVec>> currentModels;
		vector<string> rules;
		// key := headValue , value := values which satisfies it
		map<Gringo::Value, vector<Gringo::Value>> headValues;
		// lifeTime == 1 => query is used once
		// lifeTime == x => query is used x times
		// LifeTime == -1 => query is used until unregistered
		int lifeTime;
		string programSection;
		shared_ptr<reasoner::ASPCommonsTerm> term;
		ASPQueryType type;

	private:
		bool checkMatchValues(const Gringo::Value* value1, const Gringo::Value* value2);
		void saveHeadValuePair(Gringo::Value key, Gringo::Value value);

	};

} /* namespace reasoner */

#endif /* SYMROCK_ALICA_ASP_SOLVER_INCLUDE_ALICA_ASP_SOLVER_ASPQUERY_H_ */
