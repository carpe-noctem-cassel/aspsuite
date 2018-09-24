/*
 * IASPSolver.h
 *
 *  Created on: Jan 26, 2017
 *      Author: stefan
 */

#ifndef INCLUDE_ASP_COMMONS_IASPSOLVER_H_
#define INCLUDE_ASP_COMMONS_IASPSOLVER_H_

#include <string>
#include <vector>
//#include <clingo/clingocontrol.hh>
#include <clingo.hh>
#include <memory>

using namespace std;

namespace reasoner
{
	class ASPCommonsTerm;
	class ASPCommonsVariable;
	class ASPQuery;
	class IASPSolver : public enable_shared_from_this<IASPSolver>
	{
	public:
		static const void* const WILDCARD_POINTER;
		static const string WILDCARD_STRING;

		IASPSolver();
		virtual ~IASPSolver();

		virtual bool existsSolution(vector<shared_ptr<ASPCommonsVariable>>& vars, vector<shared_ptr<ASPCommonsTerm>>& calls) = 0;
		virtual bool getSolution(vector<shared_ptr<ASPCommonsVariable>>& vars, vector<shared_ptr<ASPCommonsTerm>>& calls,
							vector<void*>& results) = 0;
		virtual shared_ptr<ASPCommonsVariable> createVariable(long id) = 0;

		virtual void disableWarnings(bool noWarns) = 0;
		virtual bool loadFileFromConfig(string configKey) = 0;
		virtual void loadFile(string filename) = 0;

		//virtual void ground(Clingo::GroundVec const &vec, Clingo::Context *context) = 0;
		virtual void ground(Clingo::PartSpan parts, Clingo::GroundCallback = nullptr) = 0; 
		virtual void assignExternal(Clingo::Symbol ext, Clingo::TruthValue truthValue) = 0;
		virtual void releaseExternal(Clingo::Symbol ext) = 0;
		virtual bool solve() = 0;
		virtual void add(char const *name, Clingo::StringSpan const &params, char const *par) = 0;
		virtual Clingo::Symbol parseValue(std::string const &str) = 0;
		virtual int getQueryCounter() = 0;

		virtual void removeDeadQueries() = 0;
		virtual bool registerQuery(shared_ptr<ASPQuery> query) = 0;
		virtual bool unregisterQuery(shared_ptr<ASPQuery> query) = 0;
		virtual void printStats() = 0;
		virtual vector<shared_ptr<ASPQuery>> getRegisteredQueries() = 0;

	};

} /* namespace reasoner */

#endif /* INCLUDE_ASP_COMMONS_IASPSOLVER_H_ */