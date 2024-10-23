#include "MRU.h"

void MRU::setList(const QStringList &source) {
  m_items = source;
}

void MRU::add(const QString &item) {
  while (m_items.removeOne(item))
    emit itemRemoved(item);
  m_items.prepend(item);
  emit itemAdded(item);
  if (m_items.size() > m_maxSize) {
    QString removedItem = m_items.takeLast();
    emit itemRemoved(removedItem);
  }
}

QString MRU::takeItem(const QString &item) {
  auto index = m_items.indexOf(item);
  if (index != -1) {
    QString removedItem = m_items.takeAt(index);
    emit itemRemoved(removedItem);
    return removedItem;
  }
  return "";
}

[[nodiscard]] QStringList MRU::items() const {
  return m_items;
}
