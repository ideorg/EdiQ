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
  [[nodiscard]] QStringList items() const;

signals:
  void setItems(const QStringList &items);
};

#endif//MRU_H
