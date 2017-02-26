/*
 * ChangeSolverSettingsCommand.h
 *
 *  Created on: Jan 19, 2017
 *      Author: stefan
 */

#ifndef INCLUDE_COMMANDS_CHANGESOLVERSETTINGSCOMMAND_H_
#define INCLUDE_COMMANDS_CHANGESOLVERSETTINGSCOMMAND_H_

#include <commands/Command.h>
#include <vector>
#include <memory>
#include <QJsonObject>

namespace cng
{

	class SolverSettings;
	class ConceptNetGui;
	class SettingsDialog;
	/**
	 * Class inheriting from Command interface used to Change Solver Settings
	 */
	class ChangeSolverSettingsCommand : public Command, public std::enable_shared_from_this<ChangeSolverSettingsCommand>
	{
	Q_OBJECT
	public:
		ChangeSolverSettingsCommand(ConceptNetGui* gui, SettingsDialog* dialog, std::shared_ptr<SolverSettings> settings);
		virtual ~ChangeSolverSettingsCommand();

		void execute();
		void undo();

		QJsonObject toJSON();

		std::shared_ptr<SolverSettings> previousSettings;
		std::shared_ptr<SolverSettings> currentSettings;
		ConceptNetGui* gui;
		SettingsDialog* dialog;
	};

} /* namespace cng */

#endif /* INCLUDE_COMMANDS_CHANGESOLVERSETTINGSCOMMAND_H_ */
