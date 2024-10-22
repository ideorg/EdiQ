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

  m_mruPaths.clear();
  for (const QJsonValue& value : pathsArray) {
    if (value.isString()) {
      m_mruPaths.append(value.toString());
    }
  }
  return true;
}

[[nodiscard]] QStringList Config::mruPaths() const {
  return m_mruPaths;
}