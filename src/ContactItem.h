#pragma once
#include <QString>
#include <albert/albert.h>
#include <albert/item.h>
#include <vector>

class ContactItem : public albert::Item {
  // Item interface
public:
  ContactItem(const QString id, const QString name, const QString collection);

  QString id() const override;
  QString text() const override;
  QString subtext() const override;
  QStringList iconUrls() const override;
  QString inputActionText() const override;
  std::vector<albert::Action> actions() const override;

  QString id_;
  QString name_;
  QString collection_;
  // CNContactType type_;
};
