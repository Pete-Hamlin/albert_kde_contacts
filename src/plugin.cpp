#include "plugin.h"
#include "configwidget.h"
#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/Item>
#include <Akonadi/ItemFetchJob>
#include <Akonadi/ItemFetchScope>
#include <QMessageBox>
#include <QSettings>
#include <QString>
#include <albert/albert.h>
#include <albert/logging.h>
#include <albert/notification.h>
#include <albert/standarditem.h>

ALBERT_LOGGING_CATEGORY("contacts_kde")

using namespace albert;
using namespace std;

static auto icon = {QStringLiteral("qsp:SP_MessageBoxWarning")};

// const QStringList Plugin::icon_urls = {"xdg:contacts"};
Plugin *Plugin::instance_ = nullptr;

Plugin::Plugin() {
  instance_ = this;
  // auto s = settings();

  connect(this, &Plugin::collectionsChanged, this, &Plugin::updateIndexItems);

  updateCollectionList();
}

// Plugin::~Plugin() {}
Plugin *Plugin::instance() { return instance_; }

void Plugin::updateIndexItems() {

  vector<IndexItem> items;

  for (const auto &collection : collections_)
    if (collection.isChecked())
      collection.createIndexItems(items);

  INFO << QString("Indexed %1 items").arg(items.size());
}

QWidget *Plugin::buildConfigWidget() { return new ConfigWidget; }

const vector<CollectionItem> &Plugin::collections() const {
  return collections_;
}

void Plugin::updateCollectionList() {

  INFO << "Fetching contact collections from akonadi";
  //
  // Create a fetch job to list all collections
  Akonadi::CollectionFetchJob *fetchJob = new Akonadi::CollectionFetchJob(
      Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive);

  QObject::connect(
      fetchJob, &Akonadi::CollectionFetchJob::result, this, [this](KJob *job) {
        if (job->error()) {
          CRIT << tr("Error fetching collections: %1").arg(job->errorString());
        }

        collections_.clear();
        Akonadi::CollectionFetchJob *fetchJob =
            qobject_cast<Akonadi::CollectionFetchJob *>(job);
        const Akonadi::Collection::List collections = fetchJob->collections();
        QList<Akonadi::Collection> contactCollections;
        for (const Akonadi::Collection &collection : collections) {
          if (collection.contentMimeTypes().contains(
                  QString::fromUtf8("text/directory"))) {
            collections_.emplace_back(collection.displayName(),
                                      collection.name(),
                                      QString::number(collection.id()));
            DEBG << tr("Got collection: %1").arg(collection.name());
          }
        }
        emit collectionsChanged();
      });
}

void Plugin::error(const QString &msg, QWidget *modal_parent) {
  WARN << msg;
  QMessageBox::warning(modal_parent, qApp->applicationDisplayName(), msg);
}
