#include "CollectionItem.h"
#include <QString>

using namespace std;
using namespace albert;

CollectionItem::CollectionItem(QString n, QString id) : name(n), id(id) {}

bool CollectionItem::isChecked() const { return true; }

// void createIndexItems(vector<IndexItem> &results) {}
