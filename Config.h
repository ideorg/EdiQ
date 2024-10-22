#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDebug>
#include <utility>

class Config {
  QString m_appName;
  QString m_configFilePath;
  QStringList m_mruPaths;
  [[nodiscard]] QString getConfigFilePath() const;
public:
  Config(QString  appName)
      : m_appName(std::move(appName)), m_configFilePath(getConfigFilePath()) {}
  bool loadMRUPaths();
  [[nodiscard]] QStringList mruPaths() const;
};

#endif//CONFIG_H
