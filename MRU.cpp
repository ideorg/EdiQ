#include "MRU.h"

void MRU::setList(const QStringList &source) {
  m_items = source;
  emit setItems(m_items);
}

void MRU::add(const QString &item) {
  m_items.removeAll(item);
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
    emit itemRemoved(removedItem);  // Emituj sygnał usunięcia
    return removedItem;
  }
  return "";
}

[[nodiscard]] QList<QString> MRU::items() const {
  return m_items;
}
