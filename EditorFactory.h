/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-17
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#ifndef EDIQ_EDITORFACTORY_H
#define EDIQ_EDITORFACTORY_H

#include "Config.h"
#include "IEditorFactory.h"
#include "UntitledCounter.h"
#include <repository.h>
#include "mrutabwidget.h"


class EditorFactory : public QObject, public IEditorFactory {
  MruTabWidget *tabWidget;
  UntitledCounter untitledCounter;
  KSyntaxHighlighting::Repository *repository = nullptr;
  QString themeName = "";
  Config *config;
public:
  explicit EditorFactory(MruTabWidget *tabWidget);
  ~EditorFactory() {
    delete config;
    delete repository;
  }
  IEditor *createEditorTab(const QString &path) override;
  int getEditorCount() override;
  IEditor *getEditor(int index) override;
  IEditor *getCurrentEditor() override;
  IEditor *getEditorByPath(const QString &path) override;
  bool tryCloseEditor(int index, IEditor::CloseEnum &canClose) override;
  bool tryCloseCurrentEditor() override;
  bool tryCloseAll() override;
  void onTextChanged() override;
  void onRefreshRepository() override;
  void onChangeThemeName(QString newName);
  MRU* getMRU();
private:
  void closeEditor(int index);
};

#endif //EDIQ_EDITORFACTORY_H
