#include "Config.h"

[[nodiscard]] QString Config::getConfigFilePath() const {
  QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
  return configDir + "/" + m_appName + "/" + m_appName.toLower() + "_config.json";
}

bool Config::loadMRUPaths() {
  QFile configFile(m_configFilePath);
  if (!configFile.open(QIODevice::ReadOnly)) {
    qWarning() << "Could not open config file for reading:" << m_configFilePath;
    return false;
  }

  QByteArray jsonData = configFile.readAll();
  QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

  if (!jsonDoc.isObject()) {
    qWarning() << "Invalid JSON format in config file:" << m_configFilePath;
    return false;
  }

  QJsonObject jsonObj = jsonDoc.object();
  QJsonObject mruObj = jsonObj["mru"].toObject();
  QJsonArray pathsArray = mruObj["paths"].toArray();

  QStringList m_mruPaths;
  for (const QJsonValue& value : pathsArray) {
    if (value.isString()) {
      m_mruPaths.append(value.toString());
    }
  }
  mru->setList(m_mruPaths);
  return true;
}

bool Config::saveMRUPaths() {
  QFile configFile(m_configFilePath);
  if (!configFile.open(QIODevice::WriteOnly)) {
    qWarning() << "Could not open config file for writing:" << m_configFilePath;
    return false;
  }

  QJsonObject jsonObj;
  QJsonObject mruObj;
  QJsonArray pathsArray;
  auto m_mruPaths = mru->items();
  for (const QString& path : m_mruPaths) {
    pathsArray.append(path);
  }

  mruObj["paths"] = pathsArray;
  jsonObj["mru"] = mruObj;

  QJsonDocument jsonDoc(jsonObj);
  configFile.write(jsonDoc.toJson());
  configFile.close();

  return true;
}
