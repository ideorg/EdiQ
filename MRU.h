#ifndef MRU_H
#define MRU_H

#include <QObject>
#include <QString>
#include <QList>

class MRU : public QObject {
  Q_OBJECT
  int m_maxSize;
  QList<QString> m_items;
public:
  explicit MRU(int maxSize, QObject *parent = nullptr)
      : QObject(parent), m_maxSize(maxSize) {}

  void add(const QString &item) {
    m_items.removeAll(item);
    m_items.prepend(item);

    emit itemAdded(item);
    if (m_items.size() > m_maxSize) {
      QString removedItem = m_items.takeLast();
      emit itemRemoved(removedItem);
    }
  }

  QString takeItem(const QString &item) {
    int index = m_items.indexOf(item);
    if (index != -1) {
      QString removedItem = m_items.takeAt(index);
      emit itemRemoved(removedItem);  // Emituj sygnał usunięcia
      return removedItem;
    }
    return "";
  }

  [[nodiscard]] QList<QString> items() const {
    return m_items;
  }

signals:
  void itemAdded(const QString &item);
  void itemRemoved(const QString &item);
};

#endif//MRU_H
