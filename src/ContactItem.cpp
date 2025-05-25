#include "ContactItem.h"
#include "plugin.h"

using namespace std;
using namespace albert;

ContactItem::ContactItem(const QString id, const QString name,
                         const QString collection)
    : id_(id), name_(name), collection_(collection) {}

QString ContactItem::id() const { return id_; }

QString ContactItem::text() const { return name_; }

QString ContactItem::subtext() const { return collection_; }

QStringList ContactItem::iconUrls() const { return {QStringLiteral("")}; }

QString ContactItem::inputActionText() const { return name_; }

vector<Action> ContactItem::actions() const {
  vector<Action> actions;

  actions.emplace_back("cn-open", Plugin::tr("Open in contacts app"), [this] {
    // QUrl refuses to parse addressboolk urls. Use platform open.
    // https://bugreports.qt.io/browse/QTBUG-129496
    albert::runDetachedProcess({"xdg-open", "addressbook://" + id_});
  });

  // Phone numbers
  // for (CNLabeledValue<CNPhoneNumber *> *number in contact.phoneNumbers) {
  //   auto val = QString::fromNSString(number.value.stringValue);
  //   QString label;
  //   if (number.label == nil || number.label.length == 0)
  //     label = Plugin::tr("Phone");
  //   else if (auto *l = [CNLabeledValue localizedStringForLabel:number.label];
  //   l)
  //     label = QString::fromNSString(l);
  //   else
  //     label = QString::fromNSString(number.label);

  //   actions.emplace_back("cn-copy",
  //                        Plugin::tr("Copy phone number '%1'").arg(label),
  //                        [=] { setClipboardText(val); });
  //   actions.emplace_back("cn-call",
  //                        Plugin::tr("Call phone number '%1'").arg(label),
  //                        [=] { openUrl("tel:" + val); });
  //   actions.emplace_back("cn-iMess", Plugin::tr("iMessage to
  //   '%1'").arg(label),
  //                        [=] { openUrl("sms:" + val); });
  // }

  // // Email addresses
  // for (CNLabeledValue<NSString *> *mail in contact.emailAddresses) {
  //   auto val = QString::fromNSString(mail.value);
  //   QString label;
  //   if (mail.label == nil || mail.label.length == 0)
  //     label = Plugin::tr("Email");
  //   else if (auto *l = [CNLabeledValue localizedStringForLabel:mail.label];
  //   l)
  //     label = QString::fromNSString(l);
  //   else
  //     label = QString::fromNSString(mail.label);

  //   actions.emplace_back("ea-copy",
  //                        Plugin::tr("Copy email address '%1'").arg(label),
  //                        [=] { setClipboardText(val); });
  //   actions.emplace_back("ea-mail", Plugin::tr("Send mail to
  //   '%1'").arg(label),
  //                        [=] { openUrl("mailto:" + val); });
  // }

  return actions;
}
