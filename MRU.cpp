#include "MRU.h"

void MRU::setList(const QStringList &source) {
  m_items = source;
}

void MRU::add(const QString &item) {
  m_items.removeAll(item);
  m_items.prepend(item);
  if (m_items.size() > m_maxSize) {
    QString removedItem = m_items.takeLast();
  }
  emit setItems(m_items);
}

QString MRU::takeItem(const QString &item) {
  auto index = m_items.indexOf(item);
  if (index != -1) {
    QString removedItem = m_items.takeAt(index);
    return removedItem;
  }
  emit setItems(m_items);
  return "";
}

[[nodiscard]] QStringList MRU::items() const {
  return m_items;
}
