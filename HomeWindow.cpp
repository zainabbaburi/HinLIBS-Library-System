#include "HomeWindow.h"
#include "ui_HomeWindow.h"


HomeWindow::HomeWindow(CatalogueList& catalogueRef,
                       Patron& currentUser,
                       FIFO& holdQueueRef,
                       QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::HomeWindow)
    , catalogue(catalogueRef)
    , user(currentUser)
    , holdQueue(holdQueueRef)
{
    ui->setupUi(this);
    QTimer::singleShot(300, this, [this]() { checkReadyHolds(); });
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

// Trim helper
static std::string trim(const std::string& s) {
    auto wsfront = std::find_if_not(s.begin(), s.end(), ::isspace);
    auto wsback  = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

void HomeWindow::checkReadyHolds()
{
    // Skip the check if the patron has no holds
      if (user.getNumHolds() == 0)
          return;

    std::string currentUserId = user.getId();
    auto queue = holdQueue.getQueue();

    QStringList readyTitles;

    // Walk through every hold in the global FIFO
    for (const auto& record : queue) {
        std::string holdPatronId = trim(record.first);
        std::string holdItemId   = trim(record.second);

        // Only consider holds placed by this user
        if (holdPatronId != currentUserId)
            continue;

        // Check if this user is first in the queue for that item
        int position = holdQueue.getPosition(holdPatronId, holdItemId);
        if (position != 1)
            continue;

        // Find the actual item in the catalogue
        std::shared_ptr<CatalogueItem> item = nullptr;
        for (size_t i = 0; i < catalogue.size(); ++i) {
            if (trim(catalogue.get(i)->getId()) == holdItemId) {
                item = catalogue.get(i);
                break;
            }
        }
        if (!item)
            continue;

        // Only notify if the item is actually available again
        if (!item->isAvailable())
            continue;

        readyTitles << QString::fromStdString(item->getTitle());
    }

    // If nothing is ready, do nothing
    if (readyTitles.isEmpty())
        return;

    // Show a single popup listing all ready items
    QMessageBox::information(
        this,
        "Items Ready for Pickup",
        QString("The following items you placed on hold are now available for you to checkout:\n\n%1")
            .arg(readyTitles.join("\n"))
    );
}

