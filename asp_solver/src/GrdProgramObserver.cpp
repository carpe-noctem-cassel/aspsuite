#include "asp_solver/GrdProgramObserver.h"

namespace reasoner {

	GrdProgramObserver::GrdProgramObserver() {}

	GrdProgramObserver::~GrdProgramObserver() {}

	const std::string GrdProgramObserver::getGroundProgram() const {
		return this->groundAtoms;
	}

	void GrdProgramObserver::output_atom(Clingo::Symbol symbol, Clingo::atom_t atom) {
		this->groundAtoms.append(symbol.to_string()).append(" ");
	}

	void GrdProgramObserver::external(Clingo::atom_t atom, Clingo::ExternalType type) {
		std::cout << "External: " << atom << " " << type << std::endl;
	}

	void GrdProgramObserver::clear() {
		this->groundAtoms.clear();
	}

}
/*
a(X), c(X) :- b(X).
b(1).
b(2).
b(3).
 */