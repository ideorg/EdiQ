#ifndef MRU_H
#define MRU_H

#include <QString>
#include <QList>

class MRU {
  int m_maxSize;
  QList<QString> m_items;
public:
  explicit MRU(int maxSize): m_maxSize(maxSize) {}

  void add(const QString &item) {
    m_items.removeAll(item);
    m_items.prepend(item);

    if (m_items.size() > m_maxSize) {
      m_items.removeLast();
    }
  }

  QString takeItem(const QString &item) {
    int index = m_items.indexOf(item);
    if (index != -1) {
      return m_items.takeAt(index);
    }
    return "";
  }

  QList<QString> items() const {
    return m_items;
  }
};

#endif//MRU_H
