#include "CollectionItem.h"
#include "ContactItem.h"
#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/Item>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <KContacts/Addressee>
#include <QString>
#include <albert/logging.h>

using namespace std;
using namespace albert;

CollectionItem::CollectionItem(QString n, QString ur, QString id)
    : name(n), url(ur), id(id) {}

bool CollectionItem::isChecked() const { return true; }

void CollectionItem::createIndexItems(vector<IndexItem> &results) const {

  DEBG << "Indexing items for collection: " << this->name;
  auto col_id = this->id.toInt();

  Akonadi::CollectionFetchJob *collectionJob = new Akonadi::CollectionFetchJob(
      Akonadi::Collection(col_id), Akonadi::CollectionFetchJob::Base, nullptr);

  QObject::connect(
      collectionJob, &Akonadi::CollectionFetchJob::result, [=](KJob *job) {
        auto fetchJob = static_cast<Akonadi::CollectionFetchJob *>(job);
        if (fetchJob->collections().isEmpty()) {
          WARN << "Unable to find collection: " << col_id;
          return;
        }
        Akonadi::ItemFetchJob *itemFetchJob =
            new Akonadi::ItemFetchJob(fetchJob->collections().first());
        itemFetchJob->fetchScope().fetchFullPayload();
        QObject::connect(
            itemFetchJob, &Akonadi::ItemFetchJob::result, [](KJob *job) {
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
                  // Add contact item logic here
                  qDebug() << "Name:" << contact.formattedName();
                  qDebug() << "Phone:"
                           << contact.phoneNumbers().value(0).number();
                  qDebug() << "Email:" << contact.emails().value(0);
                }
              }
            });
      });
}
