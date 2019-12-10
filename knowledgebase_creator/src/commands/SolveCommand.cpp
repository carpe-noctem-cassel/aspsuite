#include "commands/SolveCommand.h"

#include "gui/ModelSettingDialog.h"
#include "gui/KnowledgebaseCreator.h"

#include <ui_knowledgebasecreator.h>

#include "handler/CommandHistoryHandler.h"

#include <reasoner/asp/Solver.h>

namespace kbcr
{

SolveCommand::SolveCommand(KnowledgebaseCreator* gui)
{
    this->type = "Solve";
    this->gui = gui;
    this->satisfiable = false;
}

SolveCommand::~SolveCommand() {}

void SolveCommand::printModels()
{
    std::stringstream ss;
    for (int i = 0; i < this->currentModels.size(); i++) {
        ss << "Model number " << i + 1 << ":\n" << std::endl;
        for (auto atom : this->currentModels.at(i)) {
            ss << atom << " ";
        }
        ss << std::endl;
        this->gui->getUi()->sortedModelsLabel->setText(QString(ss.str().c_str()));
    }
    this->gui->getUi()->currentModelsLabel->setText(QString(ss.str().c_str()));
}

void SolveCommand::execute()
{
    if (this->gui->modelSettingsDialog->getNumberOfModels() != -1) {
        this->gui->getSolver()->setNumberOfModels(this->gui->modelSettingsDialog->getNumberOfModels());
    }
    this->satisfiable = this->gui->getSolver()->solve();
    this->currentModels = this->gui->getSolver()->getCurrentModels();
    this->gui->chHandler->addToCommandHistory(shared_from_this());
    printModels();
    printSortedModels();
}

void SolveCommand::undo()
{
    this->gui->chHandler->removeFromCommandHistory(shared_from_this());
}

QJsonObject SolveCommand::toJSON()
{
    QJsonObject ret;
    ret["type"] = "Solve";
    return ret;
}

std::vector<Clingo::SymbolVector> SolveCommand::getCurrentModels()
{
    return this->currentModels;
}

bool SolveCommand::isSatisfiable()
{
    return this->satisfiable;
}

void SolveCommand::printSortedModels()
{
    std::stringstream ss;
    std::vector<std::vector<std::string>> sorted = std::vector<std::vector<std::string>>(this->currentModels.size());
    for (int i = 0; i < this->currentModels.size(); i++) {
        for (auto atom : this->currentModels.at(i)) {
            ss << atom;
            sorted.at(i).push_back(ss.str());
            ss.str("");
        }
        std::sort(sorted.at(i).begin(), sorted.at(i).end());
    }
    ss.str("");
    for (int i = 0; i < sorted.size(); i++) {
        ss << "Model number " << i + 1 << ":\n" << std::endl;
        for (auto atom : sorted.at(i)) {
            ss << atom << " ";
        }
        ss << std::endl;
    }
    this->gui->getUi()->sortedModelsLabel->setText(QString(ss.str().c_str()));
}

} /* namespace kbcr */
