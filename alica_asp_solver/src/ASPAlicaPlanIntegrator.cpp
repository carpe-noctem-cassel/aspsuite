/*
 * ASPAlicaPlanIntegrator.cpp
 *
 *  Created on: Sep 9, 2015
 *      Author: Stephan Opfer
 */

#include "alica_asp_solver/ASPAlicaPlanIntegrator.h"
#include "alica_asp_solver/ASPGenerator.h"

#include <engine/model/Plan.h>
#include <engine/model/PlanType.h>
#include <engine/model/BehaviourConfiguration.h>
#include <engine/model/State.h>
#include <engine/model/FailureState.h>
#include <engine/model/SuccessState.h>
#include <engine/model/Task.h>
#include <engine/model/EntryPoint.h>
#include <engine/model/SyncTransition.h>
#include <engine/model/RuntimeCondition.h>
#include <engine/model/PreCondition.h>

#include <regex>

using namespace std;

namespace alica
{
	namespace reasoner
	{

		ASPAlicaPlanIntegrator::ASPAlicaPlanIntegrator(shared_ptr<ClingoLib> clingo, ASPGenerator* gen)
		{
			this->clingo = clingo;
			this->gen = gen;
		}

		ASPAlicaPlanIntegrator::~ASPAlicaPlanIntegrator()
		{
		}

		void ASPAlicaPlanIntegrator::loadPlanTree(Plan* p)
		{
			this->processedPlanIds.clear();

			// Start recursive integration of plan tree
			this->processPlan(p);

			// Ground the created plan base
			this->clingo->ground( { {"planBase", {}}}, nullptr);
			this->clingo->ground( { {"alicaBackground", {}}}, nullptr);
		}

		/**
		 * Recursively integrates the given plan into the data-structures of the asp solver.
		 * @param Plan p.
		 * @return Returns true, if the processed plan is a tree.
		 */
		void ASPAlicaPlanIntegrator::processPlan(Plan* p)
		{
			long currentPlanId = p->getId();
			if (find(processedPlanIds.begin(), processedPlanIds.end(), currentPlanId) != processedPlanIds.end())
			{ // already processed
				return;
			}
			else
			{
				processedPlanIds.push_back(currentPlanId);
			}

			cout << "ASPAlicaPlanIntegrator: processing plan " << p->getName() << " (ID: " << p->getId() << ")" << endl;

			// add the plan
			this->clingo->add("planBase", {}, gen->plan(p));

			// TODO: add pre- and run-time condition of plan
			this->processPreCondition(p->getPreCondition());

			// add entry points and their tasks
			for (auto& idEntryPointPair : p->getEntryPoints())
			{
				EntryPoint* entryPoint = idEntryPointPair.second;
				Task* task = entryPoint->getTask();

				// add task
				// TODO: what is, if we add the task two or more times to planBase?
				// TODO: should I also integrate the IDLE Task ?
				// - for runtime evaluation this should be part of the ALICA background knowledge
				this->clingo->add("planBase", {}, gen->task(task));
				this->clingo->add("planBase", {}, gen->hasTask(p, task));

				// add entry point
				this->clingo->add("planBase", {}, gen->entryPoint(entryPoint));
				if (entryPoint->isSuccessRequired())
				{
					this->clingo->add("planBase", {}, gen->successRequired(entryPoint));
				}
				this->clingo->add("planBase", {}, gen->hasInitialState(entryPoint, entryPoint->getState()));
				this->clingo->add("planBase", {}, gen->hasMinCardinality(entryPoint, entryPoint->getMinCardinality()));
				this->clingo->add("planBase", {}, gen->hasMaxCardinality(entryPoint, entryPoint->getMaxCardinality()));
				this->clingo->add("planBase", {}, gen->hasEntryPoint(p, task, entryPoint));
				this->clingo->add("planBase", {}, gen->hasEntryPoint(p, task, entryPoint));
			}

			// add state
			for (auto& state : p->getStates())
			{

				this->clingo->add("planBase", {}, gen->hasState(p, state));

				if (state->isFailureState())
				{
					// add failure state
					this->clingo->add("planBase", {}, gen->failureState(state));

					// TODO: handle post-condition of failure state
					// ((FailureState) state).getPostCondition();
				}
				else if (state->isSuccessState())
				{
					// add success state
					this->clingo->add("planBase", {}, gen->successState(state));

					// TODO: handle post-condition of success state
					// ((SuccessState) state).getPostCondition();
				}
				else // normal state
				{
					// add state
					this->clingo->add("planBase", {}, gen->state(state));

					for (auto& abstractChildPlan : state->getPlans())
					{
						if (alica::Plan* childPlan = dynamic_cast<alica::Plan*>(abstractChildPlan))
						{
							this->clingo->add("planBase", {}, gen->hasPlan(state, childPlan));

							this->processPlan(childPlan);
						}
						else if (alica::PlanType* childPlanType = dynamic_cast<alica::PlanType*>(abstractChildPlan))
						{
							this->clingo->add("planBase", {}, gen->planType(childPlanType));
							this->clingo->add("planBase", {}, gen->hasPlanType(state, childPlanType));

							for (auto& childPlan : childPlanType->getPlans())
							{
								this->clingo->add("planBase", {}, gen->hasRealisation(childPlanType, childPlan));
								this->processPlan(childPlan);
							}
						}
						else if (alica::BehaviourConfiguration* childBehaviourConf =
								dynamic_cast<alica::BehaviourConfiguration*>(abstractChildPlan))
						{
							// TODO: Handle BehaviourConfigurations
						}
					}
				}

				// add state transition relations
				for (auto& inTransition : state->getInTransitions())
				{
					this->clingo->add("planBase", {}, gen->hasInTransition(state, inTransition));
				}
				for (auto& outTransition : state->getOutTransitions())
				{
					this->clingo->add("planBase", {}, gen->hasOutTransition(state, outTransition));
				}
			}

			// add transitions
			for (auto& transition : p->getTransitions())
			{
				this->clingo->add("planBase", {}, gen->transition(transition));

				//TODO: handle pre-conditions of transitions
			}

			// add synchronisations
			for (auto& syncTransition : p->getSyncTransitions())
			{
				this->clingo->add("planBase", {}, gen->synchronisation(syncTransition));
				for (auto& transition : syncTransition->getInSync())
				{
					this->clingo->add("planBase", {}, gen->hasSynchedTransition(syncTransition, transition));
				}
				// TODO: maybe it is nice to have the timeouts of a sync transition
			}
		}

		void ASPAlicaPlanIntegrator::processPreCondition(PreCondition* cond)
		{
			if (!cond || !cond->isEnabled())
			{
				return;
			}

			// alica program facts
			this->clingo->add("planBase", {}, gen->preCondition(cond));
			if (Plan* plan = dynamic_cast<Plan*>(cond->getAbstractPlan()))
			{
				this->clingo->add("planBase", {}, gen->hasPreCondition(plan, cond));
			}

			// asp encoded precondition
			if (cond->getConditionString() != "")
			{
				const string& condString = cond->getConditionString();

				cout << "ASP-Integrator: " << gen->preConditionHolds(cond) << endl;
				this->clingo->add("planBase", {}, gen->preConditionHolds(cond));

				// analysis of asp encoded precondition, because of non-local in relations
				std::regex words_regex("((\\s|,){1}|^)in\\((\\s*)([A-Z]+(\\w*))(\\s*),(\\s*)([a-z]+(\\w*))(\\s*)(,(\\s*)([a-zA-Z]+(\\w*))(\\s*)){2}\\)");
				auto words_begin = std::sregex_iterator(condString.begin(), condString.end(), words_regex);
				auto words_end = std::sregex_iterator();

				for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
					std::smatch match = *i;
					std::string match_str = match.str();
					std::cout <<"ASPAlicaPlanInegrator: MATCH>>>>>>"<< match_str << "<<<<<<" << std::endl;;
				}
			}
		}

	} /* namespace reasoner */
} /* namespace alica */
