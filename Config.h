#ifndef CONFIG_H
#define CONFIG_H

#include "MRU.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <utility>

class Config {
  QString m_appName;
  QString m_configFilePath;
  [[nodiscard]] QString getConfigFilePath() const;
  bool loadMRUPaths();
  bool saveMRUPaths();
public:
  Config(QString  appName)
      : m_appName(std::move(appName)), m_configFilePath(getConfigFilePath()) {
    mru = new MRU(10);
    loadMRUPaths();
  }
  ~Config() {
    saveMRUPaths();
    delete mru;
  }
  MRU *mru;
};

#endif//CONFIG_H
