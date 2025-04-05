
#pragma once
#include <QString>
#include <albert/indexitem.h>
#include <vector>

class Collection {
public:
  Collection(QString name, QString id);

  void createIndexItems(std::vector<albert::IndexItem> &results) const;

  bool isChecked() const;

  const QString name;
  const QString id;
};
