#include "CollectionItem.h"
#include "plugin.h"
#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/Item>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <KContacts/Addressee>
#include <QString>
#include <albert/albert.h>
#include <albert/logging.h>
#include <albert/standarditem.h>
#include <qeventloop.h>

using namespace std;

CollectionItem::CollectionItem(QString n, QString ur, QString id)
    : name(n), url(ur), id(id) {}

bool CollectionItem::isChecked() const { return true; }

void CollectionItem::createIndexItems(
    vector<albert::IndexItem> &results) const {

  INFO << "Indexing items for collection: " << this->name;
  auto col_id = this->id.toInt();

  Akonadi::CollectionFetchJob *collectionJob = new Akonadi::CollectionFetchJob(
      Akonadi::Collection(col_id), Akonadi::CollectionFetchJob::Base, nullptr);

  QEventLoop loop;
  QObject::connect(
      collectionJob, &Akonadi::CollectionFetchJob::result,
      [=, &loop, &results](KJob *job) {
        auto fetchJob = static_cast<Akonadi::CollectionFetchJob *>(job);
        if (fetchJob->collections().isEmpty()) {
          WARN << "Unable to find collection: " << col_id;
          return;
        }
        Akonadi::ItemFetchJob *itemFetchJob =
            new Akonadi::ItemFetchJob(fetchJob->collections().first());
        itemFetchJob->fetchScope().fetchFullPayload();

        QObject::connect(
            itemFetchJob, &Akonadi::ItemFetchJob::result, Plugin::instance(),
            [=, &loop, &results](KJob *job) {
              if (job->error()) {
                WARN << "Error fetching items:" << job->errorString();
                return;
              }
              Akonadi::ItemFetchJob *fetchJob =
                  qobject_cast<Akonadi::ItemFetchJob *>(job);
              const Akonadi::Item::List items = fetchJob->items();
              for (const Akonadi::Item &item : items) {
                if (item.hasPayload<KContacts::Addressee>()) {
                  KContacts::Addressee contact =
                      item.payload<KContacts::Addressee>();

                  auto contact_name = contact.formattedName();

                  for (auto phoneNumber : contact.phoneNumbers()) {
                    auto number = phoneNumber.number();

                    auto phone_item = albert::StandardItem::make(
                        "phone" + number, contact_name, number, {"xdg:phone"},
                        {{"copy", Plugin::tr("Copy"),
                          [number]() { albert::setClipboardText(number); }},
                         {"call", Plugin::tr("Call"),
                          [number]() { albert::openUrl("tel:" + number); }},
                         {"call", "iMessage",
                          [number]() { albert::openUrl("sms:" + number); }}});

                    results.emplace_back(phone_item, phone_item->text());
                  }

                  for (auto email : contact.emails()) {

                    auto email_item = albert::StandardItem::make(
                        "email" + email, contact_name, email,
                        {"xdg:mail-client"},
                        {{"copy", Plugin::tr("Copy"),
                          [email]() { albert::setClipboardText(email); }},
                         {"call", Plugin::tr("Call"),
                          [email]() { albert::openUrl("tel:" + email); }},
                         {"call", "iMessage",
                          [email]() { albert::openUrl("sms:" + email); }}});

                    results.emplace_back(email_item, email_item->text());
                  }
                }
              }
              loop.quit();
            });
      });
  collectionJob->start();
  loop.exec();
}
