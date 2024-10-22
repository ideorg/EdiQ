#ifndef MRU_H
#define MRU_H

#include <QObject>
#include <QString>
#include <QList>

class MRU : public QObject {
  Q_OBJECT
  int m_maxSize;
  QStringList m_items;
public:
  explicit MRU(int maxSize, QObject *parent = nullptr)
      : QObject(parent), m_maxSize(maxSize) {}

  void setList(const QStringList &source);
  void add(const QString &item);
  QString takeItem(const QString &item);
  [[nodiscard]] QList<QString> items() const;

signals:
  void setItems(const QStringList &items);
  void itemAdded(const QString &item);
  void itemRemoved(const QString &item);
};

#endif//MRU_H
