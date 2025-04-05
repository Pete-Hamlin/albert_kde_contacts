#include "Collection.h"
#include <QString>

using namespace std;
using namespace albert;

Collection::Collection(QString n, QString id) : name(n), id(id) {}

bool Collection::isChecked() const { return true; }

// void createIndexItems(vector<IndexItem> &results) {}
